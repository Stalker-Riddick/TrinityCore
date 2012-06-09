/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "obsidian_sanctum.h"

enum Texts
{
    SAY_TENEBRON_AGGRO                          = 0,
    SAY_TENEBRON_SLAY                           = 1,
    SAY_TENEBRON_DEATH                          = 2,
    SAY_TENEBRON_BREATH                         = 3,
    SAY_TENEBRON_RESPOND                        = 4,
    SAY_TENEBRON_SPECIAL                        = 5,
    SAY_TENEBRON_HATCH                          = 6,
};

enum Spells
{
    SPELL_POWER_OF_TENEBRON                     = 61248,
    SPELL_SUMMON_TWILIGHT_WHELP                 = 58035,    // doesn't work, will spawn NPC_TWILIGHT_WHELP
    SPELL_SUMMON_SARTHARION_TWILIGHT_WHELP      = 58826,    // doesn't work, will spawn NPC_SHARTHARION_TWILIGHT_WHELP
    SPELL_HATCH_EGGS                            = 58542,
    SPELL_HATCH_EGGS_EFFECT_H                   = 59190,
    SPELL_HATCH_EGGS_EFFECT                     = 58685,
    SPELL_FADE_ARMOR                            = 60708,
    SPELL_SHADOW_BREATH                         = 57570,
    SPELL_SHADOW_BREATH_H                       = 59126,
    SPELL_SHADOW_FISSURE                        = 57579,
    SPELL_SHADOW_FISSURE_H                      = 59127,
    SPELL_TWILIGHT_RESIDUE                      = 61885,
    SPELL_TWILIGHT_REVENGE                      = 60639,
    SPELL_TWILIGHT_SHIFT                        = 57620,
};

enum Events
{
    EVENT_SHADOW_FISSURE                        = 1,
    EVENT_SHADOW_BREATH                         = 2,
    EVENT_SPAWN_EGGS                            = 3,
    EVENT_OPEN_PORTAL                           = 4,
    EVENT_HATCH_EGGS                            = 5,
};

class boss_tenebron : public CreatureScript
{
    public:
        boss_tenebron() : CreatureScript("boss_tenebron") { }

        struct boss_tenebronAI : public BossAI
        {
            boss_tenebronAI(Creature* creature) : BossAI(creature, DATA_TENEBRON) {}

            void Reset()
            {
                _Reset();

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT);

                // Despawn all eggs perhaps ?
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_TENEBRON_DEATH);

                Creature* sartharion = Unit::GetCreature(*me, instance->GetData64(DATA_SARTHARION));

                if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS)
                {
                    DoCast(sartharion, SPELL_TWILIGHT_REVENGE);
                }
                else
                    instance->SetData(DATA_TENEBRON_KILLED, true);

                instance->SetBossState(DATA_TENEBRON, DONE);

                // Despawn all eggs ?
            }

            void EnterCombat(Unit* target)
            {
                _EnterCombat();

                events.ScheduleEvent(EVENT_SHADOW_FISSURE, 5000);
                events.ScheduleEvent(EVENT_SHADOW_BREATH, 20000);
                events.ScheduleEvent(EVENT_HATCH_EGGS, 30000);

                instance->SetBossState(DATA_TENEBRON, IN_PROGRESS);

                   // This won't work here
                // if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS) // Perhaps needs to be moved to UpdateAI ?
                    // DoCast(me,SPELL_POWER_OF_TENEBRON); // Perhaps needs to be cast on players in the raid
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                instance->SetBossState(DATA_TENEBRON, FAIL);
            }

            void EnterEvadeMode()
            {
                _EnterEvadeMode();
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(SAY_TENEBRON_SLAY);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_SHADOW_BREATH:
                        Talk(SAY_TENEBRON_BREATH);
                        IsHeroic() ? DoCast(SPELL_SHADOW_BREATH_H) : DoCast(SPELL_SHADOW_BREATH);
                        events.ScheduleEvent(EVENT_SHADOW_BREATH,urand(15000, 25000));
                        break;
                    case EVENT_SHADOW_FISSURE:
                        IsHeroic() ? DoCast(SPELL_SHADOW_FISSURE_H) : DoCast(SPELL_SHADOW_FISSURE);
                        events.ScheduleEvent(EVENT_SHADOW_FISSURE,urand(10000, 15000));
                        break;
                    case EVENT_SPAWN_EGGS:
                        // Spawn eggs in their location
                        break;
                    case EVENT_OPEN_PORTAL:
                        // Open  the portal to destroy eggs
                        break;
                    case EVENT_HATCH_EGGS:
                        // Hatch remaining eggs
                        break;
                    }
                }
            }

        };

            CreatureAI* GetAI(Creature* creature) const
            {
                return GetObsidianSanctumAI<boss_tenebronAI>(creature);
            }

};

void AddSC_boss_tenebron()
{
    new boss_tenebron();
};
