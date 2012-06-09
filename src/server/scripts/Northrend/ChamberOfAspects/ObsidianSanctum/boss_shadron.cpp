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
    SAY_SHADRON_AGGRO                           = 0,
    SAY_SHADRON_SLAY                            = 1,
    SAY_SHADRON_DEATH                           = 2,
    SAY_SHADRON_BREATH                          = 3,
    SAY_SHADRON_RESPOND                         = 4,
    SAY_SHADRON_SPECIAL                         = 5,
    SAY_SHADRON_OPEN_PORTAL                     = 6,
    SAY_SHADRON_DISCIPLE                        = 7,
};

enum Spells
{
    SPELL_POWER_OF_SHADRON                      = 58105,
    SPELL_GIFT_OF_TWILIGHT_SHA                  = 57835,
    SPELL_GIFT_OF_TWILIGHT_SAR                  = 58766,
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
    EVENT_SHADOW_BREATH                         = 0,
    EVENT_SHADOW_FISSURE                        = 1,
    EVENT_SUMMON_ACOLYTE                        = 2,
};

class boss_shadron : public CreatureScript
{
    public:
        boss_shadron() : CreatureScript("boss_shadron") { }

        struct boss_shadronAI : public BossAI
        {
            boss_shadronAI(Creature* creature) : BossAI(creature, DATA_SHADRON) {}

            void Reset()
            {
                _Reset();

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT);

                if (me->HasAura(SPELL_GIFT_OF_TWILIGHT_SHA))
                    me->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGHT_SHA);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_SHADRON_DEATH);

                Creature* sartharion = Unit::GetCreature(*me, instance->GetData64(DATA_SARTHARION));

                if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS)
                    DoCast(sartharion,SPELL_TWILIGHT_REVENGE);
                else
                    instance->SetData(DATA_SHADRON_KILLED, true);

                instance->SetBossState(DATA_SHADRON, DONE);
            }

            void EnterCombat(Unit* target)
            {
                _EnterCombat();
                Talk(SAY_SHADRON_AGGRO);

                events.ScheduleEvent(EVENT_SHADOW_FISSURE, 5000);
                events.ScheduleEvent(EVENT_SHADOW_BREATH, 20000);
                events.ScheduleEvent(EVENT_SUMMON_ACOLYTE, 60000);

                instance->SetBossState(DATA_SHADRON, IN_PROGRESS);

                // This won't work here
                // if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS)
                   // DoCast(me,SPELL_POWER_OF_SHADRON); // Perhaps needs to be cast on players in the raid
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                instance->SetBossState(DATA_SHADRON, FAIL);
            }

            void EnterEvadeMode()
            {
                _EnterEvadeMode();
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(SAY_SHADRON_SLAY);
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                if (summon->GetEntry() == NPC_ACOLYTE_OF_SHADRON)
                {
                    instance->DoRemoveAurasDueToSpellOnPlayers (SPELL_TWILIGHT_SHIFT);
                    instance->DoCastSpellOnPlayers(SPELL_TWILIGHT_RESIDUE);

                    if (GameObject* portal = GameObject::GetGameObject(*me, DATA_TWILIGHT_PORTAL))
                    {
                        portal->Delete();
                        me->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGHT_SHA);
                    }
                }
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
                        Talk(SAY_SHADRON_BREATH);
                        IsHeroic() ? DoCast(SPELL_SHADOW_BREATH_H) : DoCast(SPELL_SHADOW_BREATH);
                        events.ScheduleEvent(EVENT_SHADOW_BREATH,urand(15000, 25000));
                        break;
                    case EVENT_SHADOW_FISSURE:
                        IsHeroic() ? DoCast(SPELL_SHADOW_FISSURE_H) : DoCast(SPELL_SHADOW_FISSURE);
                        events.ScheduleEvent(EVENT_SHADOW_FISSURE,urand(10000, 15000));
                        break;
                    case EVENT_SUMMON_ACOLYTE:
                        if (Creature* acolyte = me->FindNearestCreature(NPC_ACOLYTE_OF_SHADRON, 50.0f))
                            if (!acolyte)
                                if (instance->GetBossState(DATA_SARTHARION) != IN_PROGRESS)
                                    me->SummonCreature(NPC_ACOLYTE_OF_SHADRON, AcolytesPos[0].GetPositionX(), AcolytesPos[0].GetPositionY(), AcolytesPos[0].GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                                else
                                    me->SummonCreature(NPC_ACOLYTE_OF_SHADRON, AcolytesPos[1].GetPositionX(), AcolytesPos[1].GetPositionY(), AcolytesPos[1].GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                        events.ScheduleEvent(EVENT_SUMMON_ACOLYTE,60000);
                        break;
                    }
                }
            }

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetObsidianSanctumAI<boss_shadronAI>(creature);
        }

};

class mob_acolyte_of_shadron : public CreatureScript
{
    public:
        mob_acolyte_of_shadron() : CreatureScript("mob_acolyte_of_shadron") { }

        struct mob_acolyte_of_shadronAI : public ScriptedAI
        {
            mob_acolyte_of_shadronAI(Creature* creature) : ScriptedAI(creature) {}

            Creature* shadron;
            Creature* sartharion;

            void Reset()
            {
                shadron = Unit::GetCreature(*me, instance->GetData64(DATA_SHADRON));
                sartharion = Unit::GetCreature(*me, instance->GetData64(DATA_SARTHARION));

                if (shadron)
                    shadron->AI()->JustSummoned(me);

                if (instance->GetBossState(DATA_SARTHARION) != IN_PROGRESS)
                {
                    shadron->AI()->Talk(SAY_SHADRON_OPEN_PORTAL);
                    //me->SummonGameObject(GO_TWILIGHT_PORTAL,x,y,z,0,0,0,0,0,999999);
                    DoCast(shadron, SPELL_GIFT_OF_TWILIGHT_SHA);
                }
                else
                {
                    shadron->AI()->Talk(SAY_SHADRON_OPEN_PORTAL);
                    //me->SummonGameObject(GO_TWILIGHT_PORTAL,x1,y1,z1,0,0,0,0,0,999999);
                    DoCast(sartharion, SPELL_GIFT_OF_TWILIGHT_SAR);
                }

                shadron->AI()->Talk(SAY_SHADRON_DISCIPLE);
            }

        protected:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_acolyte_of_shadronAI(creature);
        }
};

void AddSC_boss_shadron()
{
    new boss_shadron();
    new mob_acolyte_of_shadron();
};
