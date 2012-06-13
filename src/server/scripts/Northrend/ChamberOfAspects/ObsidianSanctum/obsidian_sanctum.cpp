/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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

enum Spells
{
    // Twilight Fissure
    SPELL_VOID_BLAST                   = 57581,
    SPELL_VOID_BLAST_25M               = 59128,
    SPELL_VOID_ZONE_PREEFFECT_VISUAL   = 46265, // Pre-Effect Visual, and apparently not the correct one perhaps 50657 or 64470 is the correct one
    SPELL_VOID_ZONE_VISUAL             = 69422,

    // Twilight Portal
    SPELL_TWILIGHT_SHIFT               = 57620,
};

enum Events
{
    // Twilight Fissure
    EVENT_VOID_BLAST                   = 0,
};


class npc_twilight_fissure : public CreatureScript
{
    public:
        npc_twilight_fissure() : CreatureScript("npc_acolyte_of_vesperon") { }

        struct npc_twilight_fissureAI : public Scripted_NoMovementAI
        {
            npc_twilight_fissureAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->AddAura(SPELL_VOID_ZONE_PREEFFECT_VISUAL, me);
                me->AddAura(SPELL_VOID_ZONE_VISUAL, me);
            }

            void UpdateAI (uint32 const diff)
            {
                Events.Update(diff);

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_VOID_BLAST:
                        Is25ManRaid() ? DoCast(SPELL_VOID_BLAST_25M) : DoCast(SPELL_VOID_BLAST);
                        me->DespawnOrUnsummon();
                        break;
                    }
                }
            }

        protected:
            EventMap Events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetObsidianSanctumAI<npc_twilight_fissureAI>(creature);
        }

};


// Next stop, the Twilight Zone!
class go_twilight_portal : public GameObjectScript
{
public:
    go_twilight_portal() : GameObjectScript("go_twilight_portal") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        player->CastSpell(player,SPELL_TWILIGHT_SHIFT,true);
        return true;
    }
};


void AddSC_obsidian_sanctum()
{
    new npc_twilight_fissure();
    new go_twilight_portal();
}
