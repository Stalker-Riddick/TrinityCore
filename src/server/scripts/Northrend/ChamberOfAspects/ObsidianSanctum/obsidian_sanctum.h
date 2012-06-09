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
    DATA_TENEBRON_KILLED              = 1,
    DATA_SHADRON_KILLED               = 2,
    DATA_VESPERON_KILLED              = 3,
    DATA_SARTHARION                   = 4,
    DATA_TENEBRON                     = 5,
    DATA_SHADRON                      = 6,
    DATA_VESPERON                     = 7,
    DATA_TWILIGHT_PORTAL              = 8,

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

const Position TwilightEggs[] =
{
    {3219.28f, 669.121f, 88.5549f},
    {3221.55f, 682.852f, 90.5361f},
    {3239.77f, 685.94f, 90.3168f},
    {3250.33f, 669.749f, 88.7637f},
    {3246.6f, 642.365f, 84.8752f},
    {3233.68f, 653.117f, 85.7051f}
};
const Position TwilightEggsSarth[] =
{
    {3252.73f, 515.762f, 58.5501f},
    {3256.56f, 521.119f, 58.6061f},
    {3255.63f, 527.513f, 58.7568f},
    {3264.90f, 525.865f, 58.6436f},
    {3264.26f, 516.364f, 58.8011f},
    {3257.54f, 502.285f, 58.2077f}
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
