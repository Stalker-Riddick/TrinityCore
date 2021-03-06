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

enum Data
{
    DATA_SARTHARION                   = 1,
    DATA_TENEBRON                     = 2,
    DATA_SHADRON                      = 3,
    DATA_VESPERON                     = 4,
    DATA_TWILIGHT_PORTAL              = 5,
};

enum Creatures
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
    NPC_TWILIGHT_FISSURE              = 30641,
    NPC_FLAME_TSUNAMI                 = 30616,
    NPC_LAVA_BLAZE                    = 30643,
    NPC_FIRE_CYCLONE                  = 30648,
};

enum GameObjects
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
    {3219.28f, 669.121f, 88.5549f}, // 0
    {3221.55f, 682.852f, 90.5361f}, // 1
    {3239.77f, 685.94f, 90.3168f},  // 2
    {3250.33f, 669.749f, 88.7637f}, // 3
    {3246.6f, 642.365f, 84.8752f},  // 4
    {3233.68f, 653.117f, 85.7051f}, // 5
};
const Position TwilightEggsSarth[] =
{
    {3252.73f, 515.762f, 58.5501f}, // 0
    {3256.56f, 521.119f, 58.6061f}, // 1
    {3255.63f, 527.513f, 58.7568f}, // 2
    {3264.90f, 525.865f, 58.6436f}, // 3
    {3264.26f, 516.364f, 58.8011f}, // 4
    {3257.54f, 502.285f, 58.2077f}, // 5
};

const Position FlameTsunami[] =
{
    {3200.00f, 573.211f, 57.1551f}, // First right spawn (0)
    {3289.28f, 573.211f, 57.1551f}, // First right direction (1)
    {3200.00f, 532.211f, 57.1551f}, // Second right spawn (2)
    {3289.28f, 532.211f, 57.1551f}, // Second right direction (3)
    {3200.00f, 491.211f, 57.1551f}, // Third right spawn (4)
    {3289.28f, 491.211f, 57.1551f}, // Third right direction (5)
    {3289.28f, 511.711f, 57.1551f}, // First left spawn (6)
    {3200.00f, 511.711f, 57.1551f}, // First left direction (7)
    {3289.28f, 552.711f, 57.1551f}, // Second left spawn (8)
    {3200.00f, 552.711f, 57.1551f}, // Second left direction (9)
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
