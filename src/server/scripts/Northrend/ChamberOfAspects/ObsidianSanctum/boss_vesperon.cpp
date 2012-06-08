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
    SAY_VESPERON_AGGRO                          = 0,
    SAY_VESPERON_SLAY                           = 1,
    SAY_VESPERON_DEATH                          = 2,
    SAY_VESPERON_BREATH                         = 3,
    SAY_VESPERON_RESPOND                        = 4,
    SAY_VESPERON_SPECIAL                        = 5,
    SAY_VESPERON_OPEN_PORTAL                    = 6,
    SAY_VESPERON_DISCIPLE                       = 7,
};

enum Spells
{
    SPELL_POWER_OF_VESPERON                     = 61251,
    SPELL_TWILIGHT_TORMENT_VESP                 = 57948,
    SPELL_TWILIGHT_TORMENT_VESP_ACO             = 58853,
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

class boss_vesperon : public CreatureScript
{
    public:
        boss_vesperon() : CreatureScript("boss_vesperon") { }

        struct boss_vesperonAI : public BossAI
        {
            boss_vesperonAI(Creature* creature) : BossAI(creature, DATA_VESPERON) {}

            void Reset()
            {
                _Reset();

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT);

                if (me->HasAura(SPELL_TWILIGHT_TORMENT_VESP))
                    me->RemoveAurasDueToSpell(SPELL_TWILIGHT_TORMENT_VESP);

            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_VESPERON_DEATH);

                Creature* sartharion = Unit::GetCreature(*me, instance->GetData64(DATA_SARTHARION));

                if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS)
                    DoCast(sartharion,SPELL_TWILIGHT_REVENGE);
                else
                    instance->SetData(DATA_SHADRON_KILLED, true);
            }

            void EnterCombat(Unit* target)
            {
                _EnterCombat();
                Talk(SAY_VESPERON_AGGRO);

                events.ScheduleEvent(EVENT_SHADOW_FISSURE,5000);
                events.ScheduleEvent(EVENT_SHADOW_BREATH,20000);
                events.ScheduleEvent(EVENT_SUMMON_ACOLYTE,30000);

                   // This won't work here
                // if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS) // Perhaps needs to be moved to UpdateAI ?
                   // DoCast(me,SPELL_POWER_OF_VESPERON); // Perhaps needs to be cast on players in the raid
            }

            void JustReachedHome()
            {
                _JustReachedHome();
            }

            void EnterEvadeMode()
            {
                _EnterEvadeMode();
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(SAY_VESPERON_SLAY);
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                if (summon->GetEntry() == NPC_ACOLYTE_OF_VESPERON)
                {
                    instance->DoRemoveAurasDueToSpellOnPlayers (SPELL_TWILIGHT_SHIFT);
                    instance->DoCastSpellOnPlayers (SPELL_TWILIGHT_RESIDUE);

                    if (GameObject* portal = GameObject::GetGameObject(*me, DATA_TWILIGHT_PORTAL))
                    {
                        portal->Delete();
                        me->RemoveAurasDueToSpell(SPELL_TWILIGHT_TORMENT_VESP);
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
                        Talk(SAY_VESPERON_BREATH);
                        IsHeroic() ? DoCast(SPELL_SHADOW_BREATH_H) : DoCast(SPELL_SHADOW_BREATH);
                        events.ScheduleEvent(EVENT_SHADOW_BREATH,urand(15000, 25000));
                        break;
                    case EVENT_SHADOW_FISSURE:
                        IsHeroic() ? DoCast(SPELL_SHADOW_FISSURE_H) : DoCast(SPELL_SHADOW_FISSURE);
                        events.ScheduleEvent(EVENT_SHADOW_FISSURE,urand(10000, 15000));
                        break;
                    case EVENT_SUMMON_ACOLYTE:
                        if (Creature* acolyte = me->FindNearestCreature(NPC_ACOLYTE_OF_VESPERON, 50.0f))
                            if (!acolyte)
                                if (instance->GetBossState(DATA_SARTHARION) != IN_PROGRESS)
                                    me->SummonCreature(NPC_ACOLYTE_OF_VESPERON, AcolytesPos[2].GetPositionX(), AcolytesPos[2].GetPositionY(), AcolytesPos[2].GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 15000);
                                else
                                    me->SummonCreature(NPC_ACOLYTE_OF_VESPERON, AcolytesPos[3].GetPositionX(), AcolytesPos[3].GetPositionY(), AcolytesPos[3].GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 15000);
                        events.ScheduleEvent(EVENT_SUMMON_ACOLYTE,30000);
                        break;
                    }
                }
            }

        };

            CreatureAI* GetAI(Creature* creature) const
            {
                return GetObsidianSanctumAI<boss_vesperonAI>(creature);
            }

};

class mob_acolyte_of_vesperon : public CreatureScript
{
    public:
        mob_acolyte_of_vesperon() : CreatureScript("mob_acolyte_of_vesperon") { }

        struct mob_acolyte_of_vesperonAI : public ScriptedAI
        {
            mob_acolyte_of_vesperonAI(Creature* creature) : ScriptedAI(creature) {}

            Creature* vesperon;
            Creature* sartharion;

            void Reset()
            {
                vesperon = Unit::GetCreature(*me, instance->GetData64(DATA_VESPERON));
                sartharion = Unit::GetCreature(*me, instance->GetData64(DATA_SARTHARION));

                if (vesperon)
                    vesperon->AI()->JustSummoned(me);

                if (instance->GetBossState(DATA_SARTHARION) != IN_PROGRESS)
                {
                    vesperon->AI()->Talk(SAY_VESPERON_OPEN_PORTAL);
                    //me->SummonGameObject(GO_TWILIGHT_PORTAL,x,y,z,0,0,0,0,0,999999);
                    DoCast(vesperon,SPELL_TWILIGHT_TORMENT_VESP);
                    DoCast(me,SPELL_TWILIGHT_TORMENT_VESP_ACO);
                }
                else
                {
                    vesperon->AI()->Talk(SAY_VESPERON_OPEN_PORTAL);
                    //me->SummonGameObject(GO_TWILIGHT_PORTAL,x,y,z,0,0,0,0,0,999999);
                    DoCast(vesperon,SPELL_TWILIGHT_TORMENT_VESP);
                    DoCast(me,SPELL_TWILIGHT_TORMENT_VESP_ACO);
                }

                vesperon->AI()->Talk(SAY_VESPERON_DISCIPLE);
            }

        protected:
            InstanceScript* instance;

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_acolyte_of_vesperonAI(creature);
        }
};

void AddSC_boss_vesperon()
{
    new boss_vesperon();
    new mob_acolyte_of_vesperon();
};
