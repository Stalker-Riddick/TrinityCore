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
    SAY_SARTHARION_AGGRO                = 0,
    SAY_SARTHARION_BERSERK              = 1,
    SAY_SARTHARION_BREATH               = 2,
    SAY_SARTHARION_CALL_SHADRON         = 3,
    SAY_SARTHARION_CALL_TENEBRON        = 4,
    SAY_SARTHARION_CALL_VESPERON        = 5,
    SAY_SARTHARION_DEATH                = 6,
    SAY_SARTHARION_LAVA_STRIKE          = 7,
    SAY_SARTHARION_LAVA_WALL            = 8,
    SAY_SARTHARION_SLAY                 = 9,
    SAY_SARTHARION_LAVA_CHURN           = 10,
};

enum Spells
{
    SPELL_BERSERK                       = 61632, // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
    SPELL_CLEAVE                        = 56909, // Inflicts 35% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
    SPELL_FLAME_BREATH                  = 56908, // Inflicts 8750 to 11250 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_25M              = 58956, // Inflicts 10938 to 14062 Fire damage to enemies in a cone in front of the caster.
    SPELL_TAIL_LASH                     = 56910, // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TAIL_LASH_25M                 = 58957, // A sweeping tail strike hits all enemies behind the caster, inflicting 4375 to 5625 damage and stunning them for 2 sec.
    SPELL_WILL_OF_SARTHARION            = 61254, // Sartharion's presence bolsters the resolve of the Twilight Drakes, increasing their total health by 25%. This effect also increases Sartharion's health by 25%.
    SPELL_LAVA_STRIKE                   = 57571, // (Real spell casted should be 57578) 57571 then trigger visual missile, then summon Lava Blaze on impact(spell 57572)
    SPELL_TWILIGHT_REVENGE              = 60639, // Increased Sartharions damage by 25% when an unnatural death occurs for one of the drakes.
    SPELL_PYROBUFFET                    = 56916, // currently used for hard enrage after 15 minutes
    SPELL_PYROBUFFET_RANGE              = 58907, // possibly used when player get too far away from dummy creatures (2x Creature entry 30494)

    // Powers of the Drakes need to be here
    // Because Sartharion casts them when they fly over for tea and biscuts.
    SPELL_POWER_OF_VESPERON             = 61251, // Vesperon's presence decreases the maximum health of all enemies by 25%.
    SPELL_POWER_OF_SHADRON              = 58105, // Shadron's presence increases Fire damage taken by all enemies by 100%.
    SPELL_POWER_OF_TENEBRON             = 61248, // Tenebron's presence increases Shadow damage taken by all enemies by 100%.

    // Flame Tsunami
    SPELL_FLAME_TSUNAMI                 = 57494,    // the visual dummy
    SPELL_FLAME_TSUNAMI_LEAP            = 60241,    // SPELL_EFFECT_138 some leap effect, causing caster to move in direction
    SPELL_FLAME_TSUNAMI_DMG_AURA        = 57491,    // periodic damage, npc has this aura
    SPELL_FLAME_TSUNAMI_BUFF            = 60430,
};


enum Paths
{
    TENEBRON_PATH                       = 12641400,
    SHADRON_PATH                        = 12641500,
    VESPERON_PATH                       = 12641300,
};

enum Events
{
    EVENT_FLAME_BREATH                  = 0,
    EVENT_TAIL_LASH                     = 1,
    EVENT_CLEAVE                        = 2,
    EVENT_LAVA_STRIKE                   = 3,
    EVENT_LAVA_CHURN                    = 4,
    EVENT_FLAME_TSUNAMI                 = 5,
    EVENT_CALL_FIRST_DRAKE              = 6,
    EVENT_CALL_SECOND_DRAKE             = 7,
    EVENT_CALL_THIRD_DRAKE              = 8,
    EVENT_PYROBUFFET                    = 9,
};


class boss_sartharion : public CreatureScript
{
    public:
        boss_sartharion() : CreatureScript("boss_sartharion") { }

        struct boss_sartharionAI : public BossAI
        {
            boss_sartharionAI(Creature* creature) : BossAI(creature, DATA_SARTHARION) {}

            void Reset()
            {
                _Reset();

                instance->SetBossState(DATA_SARTHARION, NOT_STARTED);

                if (me->HasAura(SPELL_TWILIGHT_REVENGE))
                    me->RemoveAurasDueToSpell(SPELL_TWILIGHT_REVENGE);

                ResetDrakes();

            }

            void ResetDrakes()
            {
                Creature* tenebron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TENEBRON));
                Creature* shadron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SHADRON));
                Creature* vesperon = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VESPERON));

                if (tenebron && instance->GetBossState(DATA_TENEBRON) != DONE)
                {
                    if (tenebron->isAlive())
                        tenebron->AI()->EnterEvadeMode();
                    else
                    {
                        tenebron->Respawn();
                        tenebron->AI()->EnterEvadeMode();
                    }
                }

                if (shadron && instance->GetBossState(DATA_SHADRON) != DONE)
                {
                    if (shadron->isAlive())
                        shadron->AI()->EnterEvadeMode();
                    else
                    {
                        shadron->Respawn();
                        shadron->AI()->EnterEvadeMode();
                    }
                }

                if (vesperon && instance->GetBossState(DATA_VESPERON) != DONE)
                {
                    if (vesperon->isAlive())
                        vesperon->AI()->EnterEvadeMode();
                    else
                    {
                        vesperon->Respawn();
                        vesperon->AI()->EnterEvadeMode();
                    }
                }

            }

            void DespawnDrakes()
            {
                if (instance->GetBossState(DATA_SARTHARION) != DONE)
                    return;

                if (Creature* tenebron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TENEBRON)))
                    tenebron->DisappearAndDie();
                if (Creature* shadron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SHADRON)))
                    shadron->DisappearAndDie();
                if (Creature* vesperon = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VESPERON)))
                    vesperon->DisappearAndDie();

            }

            void DamageTaken(Unit* /*who*/, uint32& /*damage*/)
            {
                Creature* tenebron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TENEBRON));
                Creature* shadron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SHADRON));
                Creature* vesperon = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VESPERON));

                if (HealthBelowPct(35) && shadron->isAlive() || tenebron->isAlive() || vesperon->isAlive())
                    DoCast(SPELL_BERSERK);

                if (tenebron->isAlive() || shadron->isAlive() || vesperon->isAlive())
                    DoCast(me, SPELL_WILL_OF_SARTHARION);
            }

            void JustDied()
            {
                _JustDied();

                Talk(SAY_SARTHARION_DEATH);

                instance->SetBossState(DATA_SARTHARION, DONE);

                DespawnDrakes();

            }

            void LavaStrike(Unit* target)
            {
                std::list<Creature*> fireCyclones;
                me->GetCreatureListWithEntryInGrid(fireCyclones, NPC_FIRE_CYCLONE, 200.0f);

                if (fireCyclones.empty)
                    return;

                if (Creature* fireCyclone = Trinity::Containers::SelectRandomContainerElement(fireCyclones))
                    fireCyclone->CastSpell(target, SPELL_LAVA_STRIKE, true);
                
            }

            void EnterCombat()
            {
                _EnterCombat();

                Talk(SAY_SARTHARION_AGGRO);

                instance->SetBossState(DATA_SARTHARION, IN_PROGRESS);

                events.ScheduleEvent(EVENT_FLAME_BREATH, 20000);
                events.ScheduleEvent(EVENT_TAIL_LASH, 20000);
                events.ScheduleEvent(EVENT_CLEAVE, 7000);
                events.ScheduleEvent(EVENT_LAVA_STRIKE, 5000);
                events.ScheduleEvent(EVENT_LAVA_CHURN, 25000);
                events.ScheduleEvent(EVENT_CALL_FIRST_DRAKE, 30000);
                events.ScheduleEvent(EVENT_CALL_SECOND_DRAKE, 75000);
                events.ScheduleEvent(EVENT_CALL_THIRD_DRAKE, 120000);
                events.ScheduleEvent(EVENT_PYROBUFFET, 900000);
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                instance->SetBossState(DATA_SARTHARION, NOT_STARTED);
            }

            void EnterEvadeMode()
            {
                _EnterEvadeMode();
            }

            void KilledUnit()
            {
                Talk(SAY_SARTHARION_SLAY);
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
                    case EVENT_FLAME_BREATH:
                        Talk(SAY_SARTHARION_BREATH);
                        Is25ManRaid() ? DoCast(me->getVictim(), SPELL_FLAME_BREATH_25M) : DoCast(me->getVictim(), SPELL_FLAME_BREATH);
                        events.ScheduleEvent(EVENT_FLAME_BREATH, urand(25000, 35000));
                        break;
                    case EVENT_TAIL_LASH:
                        Is25ManRaid() ? DoCast(me->getVictim(), SPELL_TAIL_LASH_25M) : DoCast(me->getVictim(), SPELL_TAIL_LASH);
                        events.ScheduleEvent(EVENT_TAIL_LASH, urand(15000, 20000));
                        break;
                    case EVENT_CLEAVE:
                        DoCast(me->getVictim(), SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(7000, 10000));
                        break;
                    case EVENT_LAVA_STRIKE:
                        {
                        Talk(SAY_SARTHARION_LAVA_STRIKE);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            LavaStrike(target);

                        // Soft Enrage
                        if (HealthBelowPct(10))
                            events.ScheduleEvent(1400,2000);
                        else
                            events.ScheduleEvent(5000,20000);
                        break;
                        }
                    case EVENT_LAVA_CHURN:
                        Talk(SAY_SARTHARION_LAVA_CHURN);
                        events.ScheduleEvent(EVENT_LAVA_CHURN, 25000);
                        events.ScheduleEvent(EVENT_FLAME_TSUNAMI, 5000);
                        break;
                    case EVENT_FLAME_TSUNAMI:
                        Talk(SAY_SARTHARION_LAVA_WALL);

                        switch (urand(0, 1))
                        {
                        // Right side
                        case 0: 
                            {
                            Creature* Right1 = me->SummonCreature(NPC_FLAME_TSUNAMI, FlameTsunami[0].GetPositionX(), FlameTsunami[0].GetPositionY(), FlameTsunami[0].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12000);
                            Creature* Right2 = me->SummonCreature(NPC_FLAME_TSUNAMI, FlameTsunami[2].GetPositionX(), FlameTsunami[2].GetPositionY(), FlameTsunami[2].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12000);
                            Creature* Right3 = me->SummonCreature(NPC_FLAME_TSUNAMI, FlameTsunami[4].GetPositionX(), FlameTsunami[4].GetPositionY(), FlameTsunami[4].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12000);
                            Right1->GetMotionMaster()->MovePoint(0, FlameTsunami[1].GetPositionX(), FlameTsunami[1].GetPositionY(), FlameTsunami[1].GetPositionZ());
                            Right2->GetMotionMaster()->MovePoint(0, FlameTsunami[3].GetPositionX(), FlameTsunami[3].GetPositionY(), FlameTsunami[3].GetPositionZ());
                            Right3->GetMotionMaster()->MovePoint(0, FlameTsunami[5].GetPositionX(), FlameTsunami[5].GetPositionY(), FlameTsunami[5].GetPositionZ());
                            break;
                            }
                        // Left Side
                        case 1:
                            {
                            Creature* Left1 = me->SummonCreature(NPC_FLAME_TSUNAMI, FlameTsunami[6].GetPositionX(), FlameTsunami[6].GetPositionY(), FlameTsunami[6].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12000);
                            Creature* Left2 = me->SummonCreature(NPC_FLAME_TSUNAMI, FlameTsunami[8].GetPositionX(), FlameTsunami[8].GetPositionY(), FlameTsunami[8].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 12000);
                            Left1->GetMotionMaster()->MovePoint(0, FlameTsunami[7].GetPositionX(), FlameTsunami[7].GetPositionY(), FlameTsunami[7].GetPositionZ());
                            Left2->GetMotionMaster()->MovePoint(0, FlameTsunami[9].GetPositionX(), FlameTsunami[9].GetPositionY(), FlameTsunami[9].GetPositionZ());
                            break;
                            }
                        }

                    case EVENT_CALL_FIRST_DRAKE:
                        {
                        Talk(SAY_SARTHARION_CALL_TENEBRON);
                        Creature* tenebron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TENEBRON));
                        tenebron->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        tenebron->GetMotionMaster()->MovePath(TENEBRON_PATH,false);
                        tenebron->AI()->DoCast(tenebron, SPELL_POWER_OF_TENEBRON);
                        break;
                        }
                    case EVENT_CALL_SECOND_DRAKE:
                        {
                        Talk(SAY_SARTHARION_CALL_SHADRON);
                        Creature* shadron = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SHADRON));
                        shadron->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        shadron->GetMotionMaster()->MovePath(SHADRON_PATH,false);
                        shadron->AI()->DoCast(shadron, SPELL_POWER_OF_SHADRON);
                        break;
                        }
                    case EVENT_CALL_THIRD_DRAKE:
                        {
                        Talk(SAY_SARTHARION_CALL_VESPERON);
                        Creature* vesperon = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VESPERON));
                        vesperon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        vesperon->GetMotionMaster()->MovePath(VESPERON_PATH,false);
                        vesperon->AI()->DoCast(vesperon, SPELL_POWER_OF_VESPERON);
                        break;
                        }
                    case EVENT_PYROBUFFET:
                        break;

                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetObsidianSanctumAI<boss_sartharionAI>(creature);
        }

};

void AddSC_boss_sartharion()
{
    new boss_sartharion();
};
