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
    SPELL_SHADOW_BREATH                         = 57570,
    SPELL_SHADOW_BREATH_25M                     = 59126,
    SPELL_SHADOW_FISSURE                        = 57579,
    SPELL_SHADOW_FISSURE_25M                    = 59127,
    SPELL_TWILIGHT_RESIDUE                      = 61885,
    SPELL_TWILIGHT_REVENGE                      = 60639,
    SPELL_TWILIGHT_SHIFT                        = 57620, // Next stop, the Twilight Zone!

};

enum Events
{
    EVENT_SHADOW_FISSURE                        = 1,
    EVENT_SHADOW_BREATH                         = 2,
    EVENT_SPAWN_EGGS                            = 3,
    EVENT_OPEN_PORTAL                           = 4,

    // Eggs event
    EVENT_HATCH_EGG                             = 5,
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

                RemoveEggs();
            }

            void RemoveEggs()
            {
                std::list<Creature*> EggsList;
                me->GetCreatureListWithEntryInGrid(EggsList, NPC_TWILIGHT_EGG, 100.0f);
                me->GetCreatureListWithEntryInGrid(EggsList, NPC_SARTHARION_TWILIGHT_EGG, 100.0f);
                if (!EggsList.empty())
                    for (std::list<Creature*>::const_iterator itr = EggsList.begin(); itr != EggsList.end(); ++itr)
                        (*itr)->DespawnOrUnsummon();
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_TENEBRON_DEATH);

                Creature* sartharion = Unit::GetCreature(*me, instance->GetData64(DATA_SARTHARION));

                if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS)
                {
                    DoCast(sartharion, SPELL_TWILIGHT_REVENGE);
                    
                    // Sartharion should set this after calling a drake.
                    //instance->SetBossState(DATA_TENEBRON, SPECIAL);
                }
                else
                    instance->SetBossState(DATA_TENEBRON, DONE);

                RemoveEggs();
            }

            void EnterCombat(Unit* target)
            {
                _EnterCombat();

                events.ScheduleEvent(EVENT_SHADOW_FISSURE, 5000);
                events.ScheduleEvent(EVENT_SHADOW_BREATH, 20000);
                events.ScheduleEvent(EVENT_OPEN_PORTAL, 30000);
                events.ScheduleEvent(EVENT_SPAWN_EGGS, 30000);

                instance->SetBossState(DATA_TENEBRON, IN_PROGRESS);

                   // This won't work here
                // if (instance->GetBossState(DATA_SARTHARION) == IN_PROGRESS) // Perhaps needs to be moved to UpdateAI ?
                    // DoCast(me,SPELL_POWER_OF_TENEBRON); // Perhaps needs to be cast on players in the raid
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                instance->SetBossState(DATA_TENEBRON, FAIL);

                RemoveEggs();
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
                        Is25ManRaid() ? DoCast(me->getVictim(), SPELL_SHADOW_BREATH_25M) : DoCast(me->getVictim(), SPELL_SHADOW_BREATH);
                        events.ScheduleEvent(EVENT_SHADOW_BREATH,urand(15000, 25000));
                        break;
                    case EVENT_SHADOW_FISSURE:
                        Is25ManRaid() ? DoCast(SPELL_SHADOW_FISSURE_25M) : DoCast(SPELL_SHADOW_FISSURE);
                        events.ScheduleEvent(EVENT_SHADOW_FISSURE,urand(10000, 15000));
                        break;
                    case EVENT_SPAWN_EGGS:
                        if (instance->GetBossState(DATA_SARTHARION) != IN_PROGRESS)
                            for (uint32 i; i < 6; ++i)
                                me->SummonCreature(NPC_TWILIGHT_EGG, TwilightEggs[i].GetPositionX(), TwilightEggs[i].GetPositionY(), TwilightEggs[i].GetPositionZ(), 0,TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000);
                        else
                            for (uint32 i; i < 6; ++i)
                                me->SummonCreature(NPC_SARTHARION_TWILIGHT_EGG, TwilightEggsSarth[i].GetPositionX(), TwilightEggsSarth[i].GetPositionY(), TwilightEggsSarth[i].GetPositionZ(), 0,TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000);
                        events.ScheduleEvent(EVENT_SPAWN_EGGS, 60000); // 30secs the portal stays open, then 30 secs cooldown
                        break;
                    case EVENT_OPEN_PORTAL:
                       // me->SummonGameObject(GO_TWILIGHT_PORTAL,x,y,z,0,0,0,0,0,30000);
                        events.ScheduleEvent(EVENT_OPEN_PORTAL, 60000); // 30secs the portal stays open, then 30 secs cooldown
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

class npc_twilight_egg : public CreatureScript
{
    public:
        npc_twilight_egg() : CreatureScript("npc_twilight_egg") { }

        struct npc_twilight_eggAI : public Scripted_NoMovementAI
        {
            npc_twilight_eggAI(Creature* creature) : Scripted_NoMovementAI(creature), _instance(creature->GetInstanceScript())
            {
            }

            void Reset()
            {
                if (_instance)
                    me->AddAura(SPELL_TWILIGHT_SHIFT, me);

                _events.ScheduleEvent(EVENT_HATCH_EGG, 30000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_HATCH_EGG:
                        {
                        if (_instance->GetBossState(DATA_SARTHARION) != IN_PROGRESS)
                        {
                            Creature* temp = me->SummonCreature(NPC_TWILIGHT_WHELP, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
                            me->DealDamage(temp, (me->GetMaxHealth - me->GetHealth));
                            temp->RemoveAllAuras();
                            me->RemoveAllAuras();
                            temp->SetInCombatWithZone();
                            me->DespawnOrUnsummon();
                        }
                        else
                        {
                            Creature* temp = me->SummonCreature(NPC_SARTHARION_TWILIGHT_WHELP, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
                            me->DealDamage(temp, (me->GetMaxHealth() - me->GetHealth()));
                            temp->RemoveAllAuras();
                            me->RemoveAllAuras();
                            temp->SetInCombatWithZone();
                            me->DespawnOrUnsummon();
                        }
                        break;
                        }
                    }
                }
            }
        private:
            InstanceScript* _instance;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetObsidianSanctumAI<npc_twilight_eggAI>(creature);
        }
};


void AddSC_boss_tenebron()
{
    new boss_tenebron();
    new npc_twilight_egg();
};
