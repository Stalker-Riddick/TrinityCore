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

#ifndef DEF_OBSIDIAN_SANCTUM_H
#define DEF_OBSIDIAN_SANCTUM_H

#include "Map.h"
#include "Creature.h"

#define OSScriptName "instance_obsidian_sanctum"

enum DataTypes
{
    TYPE_SARTHARION_EVENT             = 1,
    TYPE_TENEBRON_PREKILLED           = 2,
    TYPE_SHADRON_PREKILLED            = 3,
    TYPE_VESPERON_PREKILLED           = 4,
    DATA_SARTHARION                   = 5,
    DATA_TENEBRON                     = 6,
    DATA_SHADRON                      = 7,
    DATA_VESPERON                     = 8,
    DATA_TWILIGHT_PORTAL              = 9,

};

enum CreaturesIds
{
    NPC_SARTHARION                    = 28860,
    NPC_TENEBRON                      = 30452,
    NPC_SHADRON                       = 30451,
    NPC_VESPERON                      = 30449,
    NPC_ACOLYTE_OF_VESPERON           = 31219,
    NPC_ACOLYTE_OF_SHADRON            = 31218,
    NPC_TWILIGHT_WHELP                = 30890,
    NPC_SARTHARION_TWILIGHT_WHELP     = 31214,
    NPC_TWILIGHT_EGG                  = 30882,
    NPC_SARTHARION_TWILIGHT_EGG       = 31204,

};

enum GameObjectsIds
{
    GO_TWILIGHT_PORTAL                = 193988,
};

const Position AcolytesPos[]=
{
    {3363.92f, 534.703f, 97.2683f}, // 0 - Shadron Acolyte location if Sartharion is not engaged.
    {3246.57f, 551.263f, 58.6164f}, // 1 - Shadron Acolyte location if Sartharion is engaged.
    {3145.68f, 520.71f, 89.7f},     // 2 - Vesperon Acolyte location if Sartharion is not engaged.
    {3246.57f, 551.263f, 58.6164f}, // 3 - Vesperon Acolyte location if Sartharion is engaged.
};

//const Position PortalPos[]=
//{
//
//};


template<class AI>
CreatureAI* GetObsidianSanctumAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(OSScriptName))
                return new AI(creature);
    return NULL;
}

#endif
