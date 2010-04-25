/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Instance_The_Eye
SD%Complete: 100
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "def_the_eye.h"

#define ENCOUNTERS 5

/* The Eye encounters:
0 - Kael'thas event
1 - Al' ar event
2 - Solarian Event
3 - Void Reaver event
*/

#define ENTRY_RAID_DOOR_1           184327
#define ENTRY_RAID_DOOR_2           184329

#define ENTRY_SOLARIAN_DOOR         184324
#define ENTRY_VOIDREAVER_DOOR       184325

#define ENTRY_KAEL_DOOR_1           184326
#define ENTRY_KAEL_DOOR_2           184328

#define ENTRY_TEMPEST_KEEP_FENCES   184069


struct TRINITY_DLL_DECL instance_the_eye : public ScriptedInstance
{
    instance_the_eye(Map *map) : ScriptedInstance(map) {Initialize();};

    uint64 ThaladredTheDarkener;
    uint64 LordSanguinar;
    uint64 GrandAstromancerCapernian;
    uint64 MasterEngineerTelonicus;
    uint64 Kaelthas;
    uint64 Astromancer;
    uint64 Alar;

    uint64 KaelRaidDoor_1;
    uint64 KaelRaidDoor_2;
    uint64 SolarianDoor;
    uint64 VoidreaverDoor;
    uint64 KaelEncountDoor_1;
    uint64 KaelEncountDoor_2;
    uint64 TempestKeepFence;

    uint8 KaelthasEventPhase;
    uint8 AlarEventPhase;

    uint32 AlarEventDone;
    uint32 VoidReaverEventDone;
    uint32 SolarianEventDone;
    uint32 KaelthasEventDone;

    bool Encounters[ENCOUNTERS];

    void Initialize()
    {
        ThaladredTheDarkener = 0;
        LordSanguinar = 0;
        GrandAstromancerCapernian = 0;
        MasterEngineerTelonicus = 0;
        Kaelthas = 0;
        Astromancer = 0;
        Alar = 0;

        KaelRaidDoor_1 = 0;
        KaelRaidDoor_2 = 0;
        SolarianDoor = 0;
        VoidreaverDoor = 0;
        KaelEncountDoor_1 = 0;
        KaelEncountDoor_2 = 0;
        TempestKeepFence = 0;

        KaelthasEventPhase = 0;
        AlarEventPhase = 0;

        AlarEventDone = NOT_STARTED;
        VoidReaverEventDone = NOT_STARTED;
        SolarianEventDone = NOT_STARTED;
        KaelthasEventDone = NOT_STARTED;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = false;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i]) return true;

        return false;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
            case 20064: ThaladredTheDarkener = creature->GetGUID(); break;
            case 20063: MasterEngineerTelonicus = creature->GetGUID(); break;
            case 20062: GrandAstromancerCapernian = creature->GetGUID(); break;
            case 20060: LordSanguinar = creature->GetGUID(); break;
            case 19622: Kaelthas = creature->GetGUID(); break;
            case 18805: Astromancer = creature->GetGUID(); break;
            case 19514: Alar = creature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject *go)
    {

        switch(go->GetEntry())
        {
            case ENTRY_RAID_DOOR_1:
                KaelRaidDoor_1 = go->GetGUID();
                if(SolarianEventDone == DONE && VoidReaverEventDone == DONE && AlarEventDone == DONE)
                    go->SetGoState(0);
                else
                    go->SetGoState(1);
                break;
            case ENTRY_RAID_DOOR_2:
                KaelRaidDoor_2 = go->GetGUID();
                if(SolarianEventDone == DONE && VoidReaverEventDone == DONE && AlarEventDone == DONE)
                    go->SetGoState(0);
                else
                    go->SetGoState(1);
                break;
            case ENTRY_SOLARIAN_DOOR:
                SolarianDoor = go->GetGUID();
                break;
            case ENTRY_VOIDREAVER_DOOR:
                VoidreaverDoor = go->GetGUID();
                break;
            case ENTRY_KAEL_DOOR_1:
                KaelEncountDoor_1 = go->GetGUID();
                break;
            case ENTRY_KAEL_DOOR_2:
                KaelEncountDoor_2 = go->GetGUID();
                break;
            case ENTRY_TEMPEST_KEEP_FENCES:
                go->setActive(true);
                TempestKeepFence = go->GetGUID();
                break;
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_THALADREDTHEDARKENER:         return ThaladredTheDarkener;
            case DATA_LORDSANGUINAR:                return LordSanguinar;
            case DATA_GRANDASTROMANCERCAPERNIAN:    return GrandAstromancerCapernian;
            case DATA_MASTERENGINEERTELONICUS:      return MasterEngineerTelonicus;
            case DATA_KAELTHAS:                     return Kaelthas;
            case DATA_ASTROMANCER:                  return Astromancer;
            case DATA_ALAR:                         return Alar;
        }
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case DATA_ALAREVENT:
                AlarEventPhase = data;
                Encounters[0] = data;
                if(data == DONE)
                {
                    AlarEventDone = DONE;
                    if(SolarianEventDone == DONE && VoidReaverEventDone == DONE)
                    {
                        if(GameObject *Door = instance->GetGameObjectInMap(KaelRaidDoor_1))
                            Door->SetGoState(0);
                        if(GameObject *Door = instance->GetGameObjectInMap(KaelRaidDoor_2))
                            Door->SetGoState(0);
                    }
                }
                break;
            case DATA_HIGHASTROMANCERSOLARIANEVENT:
                Encounters[1] = data;
                if(data == DONE)
                {
                    SolarianEventDone = DONE;
                    if(AlarEventDone == DONE && VoidReaverEventDone == DONE)
                    {
                        if(GameObject *Door = instance->GetGameObjectInMap(KaelRaidDoor_1))
                            Door->SetGoState(0);
                        if(GameObject *Door = instance->GetGameObjectInMap(KaelRaidDoor_2))
                            Door->SetGoState(0);
                    }
                }
                if(data == IN_PROGRESS)
                {
                    if(GameObject *Door = instance->GetGameObjectInMap(SolarianDoor))
                        Door->SetGoState(1);
                }else
                {
                    if(GameObject *Door = instance->GetGameObjectInMap(SolarianDoor))
                        Door->SetGoState(0);
                }
                break;
            case DATA_VOIDREAVEREVENT:
                Encounters[2] = data;
                if(data == DONE)
                {
                    VoidReaverEventDone = DONE;
                    if(SolarianEventDone == DONE && AlarEventDone == DONE)
                    {
                        if(GameObject *Door = instance->GetGameObjectInMap(KaelRaidDoor_1))
                            Door->SetGoState(0);
                        if(GameObject *Door = instance->GetGameObjectInMap(KaelRaidDoor_2))
                            Door->SetGoState(0);
                    }
                }
                if(data == IN_PROGRESS)
                {
                    if(GameObject *Door = instance->GetGameObjectInMap(VoidreaverDoor))
                        Door->SetGoState(1);
                }else
                {
                    if(GameObject *Door = instance->GetGameObjectInMap(VoidreaverDoor))
                        Door->SetGoState(0);
                }
                break;
            case DATA_KAELTHASEVENT:
                KaelthasEventPhase = data;
                Encounters[3] = data;
                if(data == DONE)
                {
                    KaelthasEventDone = DONE;
                }
                if(data == IN_PROGRESS)
                {
                    if(GameObject *Door = instance->GetGameObjectInMap(KaelEncountDoor_1))
                        Door->SetGoState(1);
                    if(GameObject *Door = instance->GetGameObjectInMap(KaelEncountDoor_2))
                        Door->SetGoState(1);
                }else
                {
                    if(GameObject *Door = instance->GetGameObjectInMap(KaelEncountDoor_1))
                        Door->SetGoState(0);
                    if(GameObject *Door = instance->GetGameObjectInMap(KaelEncountDoor_2))
                        Door->SetGoState(0);
                }
                break;
        }

        SaveToDB();
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case DATA_ALAREVENT:    return AlarEventPhase;
            case DATA_HIGHASTROMANCERSOLARIANEVENT: return Encounters[1];
            case DATA_VOIDREAVEREVENT:  return Encounters[2];
            case DATA_KAELTHASEVENT:    return Encounters[3];
        }
        return 0;
    }

    const char* Save()
    {
        OUT_SAVE_INST_DATA;
        std::ostringstream stream;
        stream << "T E " << AlarEventDone << " " << SolarianEventDone << " " << VoidReaverEventDone << " " << KaelthasEventDone;
        char* out = new char[stream.str().length() + 1];
        strcpy(out, stream.str().c_str());
        if(out)
        {
            OUT_SAVE_INST_DATA_COMPLETE;
            return out;
        }
        return NULL;
    }

    void Load(const char* in)
    {
        if(!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }
        OUT_LOAD_INST_DATA(in);
        std::istringstream stream(in);

        char dataHead1, dataHead2;
        uint16 data0,data1,data2,data3;

        stream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3;

        if(dataHead1 == 'T' && dataHead2 == 'E')
        {
             AlarEventDone = data0;
             SolarianEventDone = data1;
             VoidReaverEventDone = data2;
             KaelthasEventDone = data3;
        }else OUT_LOAD_INST_DATA_FAIL;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_the_eye(Map* map)
{
    return new instance_the_eye(map);
}

void AddSC_instance_the_eye()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_the_eye";
    newscript->GetInstanceData = &GetInstanceData_instance_the_eye;
    newscript->RegisterSelf();
}

