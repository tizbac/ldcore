/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Instance_Serpent_Shrine
SD%Complete: 100
SDComment: Instance Data Scripts and functions to acquire mobs and set encounter status for use in various Serpent Shrine Scripts
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "def_serpent_shrine.h"

#define ENCOUNTERS 7
//Lurker Event defines
#define SCALDING_WATER                 37284
#define MOB_COILFANG_FRENZY            21508
#define MOB_COILFANG_FRENZY_CORPSE     21689
#define MAX_HONOR_GUAR = 6;

/* Serpentshrine cavern encounters:
0 - Hydross The Unstable event
1 - Leotheras The Blind Event
2 - The Lurker Below Event
3 - Fathom-Lord Karathress Event
4 - Morogrim Tidewalker Event
5 - Lady Vashj Event
6 - Lurker Event (platform)
*/

bool GOHello_go_bridge_console(Player *player, GameObject* go)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)go->GetInstanceData();

    if(!pInstance)
        return false;

    if (pInstance)
        pInstance->SetData(DATA_CONTROL_CONSOLE, DONE);

    return true;
}

struct TRINITY_DLL_DECL instance_serpentshrine_cavern : public ScriptedInstance
{
    instance_serpentshrine_cavern(Map *map) : ScriptedInstance(map) {Initialize();};

    uint64 LurkerBelow;
    uint64 Sharkkis;
    uint64 Tidalvess;
    uint64 Caribdis;
    uint64 LadyVashj;
    uint64 Karathress;
    uint64 KarathressEvent_Starter;
    uint64 LeotherasTheBlind;
    uint64 LeotherasEventStarter;

    uint64 ControlConsole;
    uint64 BridgePart[3];
    uint64 StrangePool;

    bool ShieldGeneratorDeactivated[4];
    uint32 Encounters[ENCOUNTERS];

    std::set<uint64> m_uiHonorGuardGUID;
    uint32 WaterTimer;
    uint32 EventTimer;

    void Initialize()
    {
        LurkerBelow = 0;
        Sharkkis = 0;
        Tidalvess = 0;
        Caribdis = 0;
        LadyVashj = 0;
        Karathress = 0;
        KarathressEvent_Starter = 0;
        LeotherasTheBlind = 0;
        LeotherasEventStarter = 0;

        ControlConsole = 0;
        BridgePart[0] = 0;
        BridgePart[1] = 0;
        BridgePart[2] = 0;
        StrangePool = 0;

        ShieldGeneratorDeactivated[0] = false;
        ShieldGeneratorDeactivated[1] = false;
        ShieldGeneratorDeactivated[2] = false;
        ShieldGeneratorDeactivated[3] = false;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = NOT_STARTED;

        //Lurker Event
        WaterTimer = 1000;
        EventTimer = 5000;
        m_uiHonorGuardGUID.clear();
    }

    void CheckConsole()
    {
        if (GameObject* pConsole = instance->GetGameObjectInMap(ControlConsole))
        {
            bool bridge = true;
            for(uint8 i = 0; i < 5; i++)
            {
                if (Encounters[i] != DONE)
                    bridge = false;
            }
            if(bridge)
                pConsole->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
            else
                pConsole->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
        }
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case 184568:
                ControlConsole = go->GetGUID();
                go->setActive(true);
                CheckConsole();
            break;

            case 184203:
                BridgePart[0] = go->GetGUID();
                go->setActive(true);
                CheckConsole();
            break;

            case 184204:
                BridgePart[1] = go->GetGUID();
                go->setActive(true);
                CheckConsole();
            break;

            case 184205:
                BridgePart[2] = go->GetGUID();
                go->setActive(true);
                CheckConsole();
            break;
        }
    }

    void OpenDoor(uint64 DoorGUID, bool open)
    {
        if(GameObject *Door = instance->GetGameObjectInMap(DoorGUID))
            Door->SetUInt32Value(GAMEOBJECT_STATE, open ? 0 : 1);
    }

    void notifyFish()
    {
        Map::PlayerList const& PlayerList = instance->GetPlayers();
        if (PlayerList.isEmpty())
            return;

        for(Map::PlayerList::const_iterator i = PlayerList.begin();i != PlayerList.end(); ++i)
        {
            if(Player* pPlayer = i->getSource())
            {
                pPlayer->GetSession()->SendNotification("The water started to boil..."); 
                pPlayer->GetSession()->SendNotification("...The Lurker Below is waiting...");
            }
        }
    }

    void Update(uint32 diff)
    {
        if(GetData(DATA_THELURKERBELOWEVENT) == DONE) // se lurker ucciso, non faccio nulla
            return;

        if (EventTimer < diff) //lurker platform check  
        {
            if (!m_uiHonorGuardGUID.empty() && GetData(TYPE_PLATFORM_EVENT) != DONE)
            {
                uint32 Dead=0;
                for(std::set<uint64>::iterator i = m_uiHonorGuardGUID.begin(); i != m_uiHonorGuardGUID.end(); ++i)
                    if (Creature* HonorGuard = instance->GetCreatureInMap(*i))
                        if (HonorGuard->isDead())
                            Dead++;

                if (Dead == 6) 
                {
                    SetData(TYPE_PLATFORM_EVENT, DONE);
                    notifyFish();
                }
                else if (Dead==0)
                    SetData(TYPE_PLATFORM_EVENT, NOT_STARTED);
            }
            EventTimer = 5000;
        }else EventTimer -= diff;
        
        if (WaterTimer < diff)
        {
            Map::PlayerList const& PlayerList = instance->GetPlayers();
            if (PlayerList.isEmpty())
                return;
            
            for(Map::PlayerList::const_iterator i = PlayerList.begin();i != PlayerList.end(); ++i)
            {
                if(Player* pPlayer = i->getSource())
                {
                    if (pPlayer->IsInWater())   //se il pg è in acqua
                    {
                        if(GetData(TYPE_PLATFORM_EVENT) != DONE) // se l'evento piattaforme non è stato fatto summono i piranha
                        {  
                            Creature* Fishy = pPlayer->SummonCreature(MOB_COILFANG_FRENZY,pPlayer->GetPositionX(),pPlayer->GetPositionY(),pPlayer->GetPositionZ(),pPlayer->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,15000);
                            if (Fishy) 
                                Fishy->AI()->AttackStart(pPlayer);
                        }
                        else if (!pPlayer->HasAura(SCALDING_WATER,0))             //se il pg è in acqua ma l'evento piattaforme è fatto 
                            pPlayer->CastSpell(pPlayer,SCALDING_WATER,true);  // e il pg non ha l'aura dell'acqua che bolle gliela casto
                    }
                    else if (pPlayer->HasAura(SCALDING_WATER,0) && !pPlayer->HasUnitMovementFlag(MOVEMENTFLAG_JUMPING)) // se il pg non è in acqua e ha l'aura la tolgo
                         pPlayer->RemoveAurasDueToSpell(SCALDING_WATER);                                                                                                // ma solo se non sta saltando fuori dall'acqua         
                }
            }
            WaterTimer = 1000; 
        }else WaterTimer -= diff;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
            case 21212: LadyVashj = creature->GetGUID();            break;
            case 21214: Karathress = creature->GetGUID();           break;
            case 21966: Sharkkis = creature->GetGUID();             break;
            case 21217: LurkerBelow = creature->GetGUID();          break;
            case 21965: Tidalvess = creature->GetGUID();            break;
            case 21964: Caribdis = creature->GetGUID();             break;
            case 21218: m_uiHonorGuardGUID.insert(creature->GetGUID()); break;
            case 21215: LeotherasTheBlind = creature->GetGUID();    break;}
    }

    void SetData64(uint32 type, uint64 data)
    {
        if(type == DATA_KARATHRESSEVENT_STARTER)
            KarathressEvent_Starter = data;
        if(type == DATA_LEOTHERAS_EVENT_STARTER)
            LeotherasEventStarter = data;
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_THELURKERBELOW:           return LurkerBelow;
            case DATA_SHARKKIS:                 return Sharkkis;
            case DATA_TIDALVESS:                return Tidalvess;
            case DATA_CARIBDIS:                 return Caribdis;
            case DATA_LADYVASHJ:                return LadyVashj;
            case DATA_KARATHRESS:               return Karathress;
            case DATA_KARATHRESSEVENT_STARTER:  return KarathressEvent_Starter;
            case DATA_LEOTHERAS:                return LeotherasTheBlind;
            case DATA_LEOTHERAS_EVENT_STARTER:  return LeotherasEventStarter;
        }
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
        case DATA_CONTROL_CONSOLE:
            if(data = DONE)
            {
                OpenDoor(BridgePart[0], true);
                OpenDoor(BridgePart[1], true);
                OpenDoor(BridgePart[2], true);
            }
            ControlConsole = data;
        case DATA_HYDROSSTHEUNSTABLEEVENT:
            if(Encounters[0] != DONE)
                Encounters[0] = data;
            break;
        case DATA_LEOTHERASTHEBLINDEVENT:
            if(Encounters[1] != DONE)
                Encounters[1] = data;
            break;
        case DATA_THELURKERBELOWEVENT:
            if(Encounters[2] != DONE)
                Encounters[2] = data;
            break;
        case DATA_KARATHRESSEVENT:
            if(Encounters[3] != DONE)
                Encounters[3] = data;
            break;
        case DATA_MOROGRIMTIDEWALKEREVENT:
            if(Encounters[4] != DONE)
                Encounters[4] = data;
            break;
            //Lady Vashj
        case DATA_LADYVASHJEVENT:
            if(Encounters[5] == DONE)
                break;
            if(data == NOT_STARTED)
            {
                ShieldGeneratorDeactivated[0] = false;
                ShieldGeneratorDeactivated[1] = false;
                ShieldGeneratorDeactivated[2] = false;
                ShieldGeneratorDeactivated[3] = false;
            }
            Encounters[5] = data;
            break;
        case DATA_SHIELDGENERATOR1:ShieldGeneratorDeactivated[0] = (data) ? true : false;   break;
        case DATA_SHIELDGENERATOR2:ShieldGeneratorDeactivated[1] = (data) ? true : false;   break;
        case DATA_SHIELDGENERATOR3:ShieldGeneratorDeactivated[2] = (data) ? true : false;   break;
        case DATA_SHIELDGENERATOR4:ShieldGeneratorDeactivated[3] = (data) ? true : false;   break;
        case TYPE_PLATFORM_EVENT: Encounters[6] = data; break;
        }

        CheckConsole();
        if(data = DONE)
            SaveToDB();
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case DATA_HYDROSSTHEUNSTABLEEVENT:  return Encounters[0];
            case DATA_LEOTHERASTHEBLINDEVENT:   return Encounters[1];
            case DATA_THELURKERBELOWEVENT:      return Encounters[2];
            case DATA_KARATHRESSEVENT:          return Encounters[3];
            case DATA_MOROGRIMTIDEWALKEREVENT:  return Encounters[4];
                //Lady Vashj
            case DATA_LADYVASHJEVENT:           return Encounters[5];
            case TYPE_PLATFORM_EVENT:           return Encounters[6];
            case DATA_SHIELDGENERATOR1:         return ShieldGeneratorDeactivated[0];
            case DATA_SHIELDGENERATOR2:         return ShieldGeneratorDeactivated[1];
            case DATA_SHIELDGENERATOR3:         return ShieldGeneratorDeactivated[2];
            case DATA_SHIELDGENERATOR4:         return ShieldGeneratorDeactivated[3];
            case DATA_CANSTARTPHASE3:
                if(ShieldGeneratorDeactivated[0] && ShieldGeneratorDeactivated[1] && ShieldGeneratorDeactivated[2] && ShieldGeneratorDeactivated[3])return 1;break;
        }
        return 0;
    }
    const char* Save()
    {
        OUT_SAVE_INST_DATA;
        std::ostringstream stream;
        stream << Encounters[0] << " " << Encounters[1] << " " << Encounters[2] << " "
            << Encounters[3] << " " << Encounters[4] << " " << Encounters[5] << " " << Encounters[6];
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
        stream >> Encounters[0] >> Encounters[1] >> Encounters[2] >> Encounters[3]
        >> Encounters[4] >> Encounters[5] >> Encounters[6];
        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            if(Encounters[i] == IN_PROGRESS)                // Do not load an encounter as "In Progress" - reset it instead.
                Encounters[i] = NOT_STARTED;
        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_serpentshrine_cavern(Map* map)
{
    return new instance_serpentshrine_cavern(map);
}

void AddSC_instance_serpentshrine_cavern()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "instance_serpent_shrine";
    newscript->GetInstanceData = &GetInstanceData_instance_serpentshrine_cavern;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="go_bridge_console";
    newscript->pGOHello = &GOHello_go_bridge_console;
    newscript->RegisterSelf();
}

