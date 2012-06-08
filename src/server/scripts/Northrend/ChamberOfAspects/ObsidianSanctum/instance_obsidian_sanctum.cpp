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

#include "ScriptPCH.h"
#include "obsidian_sanctum.h"

#define MAX_ENCOUNTER     1

/* Obsidian Sanctum encounters:
0 - Sartharion
*/

class instance_obsidian_sanctum : public InstanceMapScript
{
public:
    instance_obsidian_sanctum() : InstanceMapScript("instance_obsidian_sanctum", 615) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_obsidian_sanctum_InstanceMapScript(map);
    }

    struct instance_obsidian_sanctum_InstanceMapScript : public InstanceScript
    {
        instance_obsidian_sanctum_InstanceMapScript(Map* map) : InstanceScript(map) {}

        uint32 Encounter[MAX_ENCOUNTER];
        uint64 SartharionGUID;
        uint64 TenebronGUID;
        uint64 ShadronGUID;
        uint64 VesperonGUID;
        uint64 TwilightPortalGUID;

        bool TenebronKilled;
        bool ShadronKilled;
        bool VesperonKilled;

        void Initialize()
        {
            memset(&Encounter, 0, sizeof(Encounter));

            SartharionGUID      = 0;
            TenebronGUID        = 0;
            ShadronGUID         = 0;
            VesperonGUID        = 0;
            TwilightPortalGUID  = 0;

            TenebronKilled = false;
            ShadronKilled = false;
            VesperonKilled = false;
        }

        bool IsEncounterInProgress() const
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (Encounter[i] == IN_PROGRESS)
                    return true;

            return false;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_SARTHARION:
                    SartharionGUID = creature->GetGUID();
                    break;
                //three dragons below set to active state once created.
                //we must expect bigger raid to encounter main boss, and then three dragons must be active due to grid differences
                case NPC_TENEBRON:
                    TenebronGUID = creature->GetGUID();
                    creature->setActive(true);
                    break;
                case NPC_SHADRON:
                    ShadronGUID = creature->GetGUID();
                    creature->setActive(true);
                    break;
                case NPC_VESPERON:
                    VesperonGUID = creature->GetGUID();
                    creature->setActive(true);
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_TWILIGHT_PORTAL:
                    TwilightPortalGUID = go->GetGUID();
                    break;
            }
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
            case DATA_SARTHARION:
                break;
            case DATA_SHADRON:
                break;
            case DATA_VESPERON:
                break;
            case DATA_TENEBRON:
                break;
            }

            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            if (type == DATA_SARTHARION_EVENT)
                Encounter[0] = data;
            else if (type == DATA_TENEBRON_KILLED)
                TenebronKilled = true;
            else if (type == DATA_SHADRON_KILLED)
                ShadronKilled = true;
            else if (type == DATA_VESPERON_KILLED)
                VesperonKilled = true;
        }

        uint32 GetData(uint32 type)
        {
            if (type == DATA_SARTHARION_EVENT)
                return Encounter[0];
            else if (type == DATA_TENEBRON_KILLED)
                return TenebronKilled;
            else if (type == DATA_SHADRON_KILLED)
                return ShadronKilled;
            else if (type == DATA_VESPERON_KILLED)
                return VesperonKilled;

            return 0;
        }

        uint64 GetData64(uint32 data)
        {
            switch (data)
            {
                case DATA_SARTHARION:
                    return SartharionGUID;
                case DATA_TENEBRON:
                    return TenebronGUID;
                case DATA_SHADRON:
                    return ShadronGUID;
                case DATA_VESPERON:
                    return VesperonGUID;
                case DATA_TWILIGHT_PORTAL:
                    return TwilightPortalGUID;
            }
            return 0;
        }
    };

};

void AddSC_instance_obsidian_sanctum()
{
    new instance_obsidian_sanctum();
}
