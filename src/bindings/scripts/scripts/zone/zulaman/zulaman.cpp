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
SDName: Zulaman
SD%Complete: 90
SDComment: Forest Frog will turn into different NPC's. Workaround to prevent new entry from running this script
SDCategory: Zul'Aman
EndScriptData */

/* ContentData
npc_forest_frog
EndContentData */

#include "precompiled.h"
#include "def_zulaman.h"

/*######
## npc_forest_frog
######*/

#define SPELL_REMOVE_AMANI_CURSE    43732
#define SPELL_PUSH_MOJO             43923
#define ENTRY_FOREST_FROG           24396

struct TRINITY_DLL_DECL npc_forest_frogAI : public ScriptedAI
{
    npc_forest_frogAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance *pInstance;

    void Reset() { }

    void EnterCombat(Unit *who) { }

    void DoSpawnRandom()
    {
        if( pInstance )
        {
            uint32 cEntry = 0;
            switch(rand()%11)
            {
                case 0: cEntry = 24024; break;          //Kraz
                case 1: cEntry = 24397; break;          //Mannuth
                case 2: cEntry = 24403; break;          //Deez
                case 3: cEntry = 24404; break;          //Galathryn
                case 4: cEntry = 24405; break;          //Adarrah
                case 5: cEntry = 24406; break;          //Fudgerick
                case 6: cEntry = 24407; break;          //Darwen
                case 7: cEntry = 24445; break;          //Mitzi
                case 8: cEntry = 24448; break;          //Christian
                case 9: cEntry = 24453; break;          //Brennan
                case 10: cEntry = 24455; break;         //Hollee
            }

            if( !pInstance->GetData(TYPE_RAND_VENDOR_1) )
                if(rand()%10 == 1) cEntry = 24408;      //Gunter
            if( !pInstance->GetData(TYPE_RAND_VENDOR_2) )
                if(rand()%10 == 1) cEntry = 24409;      //Kyren

            if( cEntry ) m_creature->UpdateEntry(cEntry);

            if( cEntry == 24408) pInstance->SetData(TYPE_RAND_VENDOR_1,DONE);
            if( cEntry == 24409) pInstance->SetData(TYPE_RAND_VENDOR_2,DONE);
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if( spell->Id == SPELL_REMOVE_AMANI_CURSE && caster->GetTypeId() == TYPEID_PLAYER && m_creature->GetEntry() == ENTRY_FOREST_FROG )
        {
            //increase or decrease chance of mojo?
            if( rand()%99 == 50 ) DoCast(caster,SPELL_PUSH_MOJO,true);
            else DoSpawnRandom();
        }
    }
};
CreatureAI* GetAI_npc_forest_frog(Creature *_Creature)
{
    return new npc_forest_frogAI (_Creature);
}

/*######
## npc_zulaman_hostage
######*/

#define GOSSIP_HOSTAGE1        "I am glad to help you."

static uint32 HostageEntry[] = {23790, 23999, 24024, 24001};
static uint32 ChestEntry[] = {186648, 187021, 186667, 186672};

struct TRINITY_DLL_DECL npc_zulaman_hostageAI : public ScriptedAI
{
    npc_zulaman_hostageAI(Creature *c) : ScriptedAI(c) {IsLoot = false;}
    bool IsLoot;
    uint64 PlayerGUID;
    void Reset() {}
    void EnterCombat(Unit *who) {}
    void JustDied(Unit *)
    {
        Player* player = Unit::GetPlayer(PlayerGUID);
        if(player) player->SendLoot(m_creature->GetGUID(), LOOT_CORPSE);
    }
    void UpdateAI(const uint32 diff)
    {
        if(IsLoot) m_creature->CastSpell(m_creature, 7, false);
    }
};

bool GossipHello_npc_zulaman_hostage(Player* player, Creature* _Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_HOSTAGE1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_zulaman_hostage(Player* player, Creature* _Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_INFO_DEF + 1)
        player->CLOSE_GOSSIP_MENU();

    if(!_Creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
        return true;
    _Creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

    ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());
    if(pInstance)
    {
        //uint8 progress = pInstance->GetData(DATA_CHESTLOOTED);
        uint32 entry = _Creature->GetEntry();
        pInstance->SetData(DATA_CHESTLOOTED, entry);
        float x, y, z;
        _Creature->GetPosition(x, y, z);
        for(uint8 i = 0; i < 4; ++i)
        {
            if(HostageEntry[i] == entry)
            {
                _Creature->SummonGameObject(ChestEntry[i], x-2, y, z, 0, 0, 0, 0, 0, 0);
                break;
            }
        }
/*
        Creature* summon = _Creature->SummonCreature(HostageInfo[progress], x-2, y, z, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
        if(summon)
        {
            ((npc_zulaman_hostageAI*)summon->AI())->PlayerGUID = player->GetGUID();
            ((npc_zulaman_hostageAI*)summon->AI())->IsLoot = true;
            summon->SetDisplayId(10056);
            summon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            summon->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
*/
    }
    return true;
}

CreatureAI* GetAI_npc_zulaman_hostage(Creature *_Creature)
{
    return new npc_zulaman_hostageAI(_Creature);
}

/*######
## npc_harrison_jones
######*/

#define GOSSIP_JONES "Open your doors to Zul'Aman [PH]"

bool GossipHello_npc_zulaman_harrison_jones(Player* player, Creature* _Creature)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());
    if(pInstance)
    {
        if(pInstance->GetData(DATA_TIMERSTART) == 0)
        {
            player->ADD_GOSSIP_ITEM(0, GOSSIP_JONES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
        }
    }
    return true;
}

bool GossipSelect_npc_zulaman_harrison_jones(Player* player, Creature* _Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        _Creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        
        ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());
        if(pInstance)
        {
            if(_Creature->GetMap()->IsDungeon())   
                ((InstanceMap*)_Creature->GetMap())->PermBindAllPlayers(player);
            pInstance->SetData(DATA_TIMERSTART,IN_PROGRESS);
        }
    }
    return true;
}

/*#################################
## Akilzon Gaundlet
###################################
    // Point 1 -- Scout
    208.711 1475.133 25.915231
###################################
###############
#Involved Adds:
###############
    Amani'shi Warrior - 24225   // spawns from start
    Amani'shi Lookout - 24175   // start gauntlet
    Amani'shi Tempest - 24549   // end gauntlet
    Amani Eagle       - 24159   // spawns from end
###################################
#SQL:
#####
    update creature_template set scriptname = 'mob_amanishi_lookout' where entry = 24175;
    update creature_template set scriptname = 'mob_amanishi_warrior' where entry = 24225
    update creature_template set scriptname = 'mob_amanishi_tempest' where entry = 24549
    update creature_template set scriptname = 'mob_amani_eagle'      where entry = 24159
#################################*/

float GauntletWaypoints[8][3] = 
{
    {227.510, 1459.576, 25.920},// Gauntled -- Point 1 Start
    {227.684, 1438.195, 26.271},// Gauntled -- Point 2 RampStart
    {229.847, 1380.534, 45.460},// Gauntled -- Point 3 RampEnd
    {248.919, 1370.393, 49.322},// Gauntled -- Point 4 secound flor
    {282.367, 1379.441, 49.323},// Gauntled -- Point 5 Stares begin
    {301.168, 1385.222, 57.678},// Gauntled -- Point 6 Stares middle 1
    {313.466, 1388.549, 57.401},// Gauntled -- Point 7 Stares middle 2
    {335.775, 1395.879, 74.581} // Gauntled -- Point 8 Stares end
};
/*######
## Amani'shi Lookout
######*/

#define YELL_START              "Akil/'zon, the invaders approach!"

struct TRINITY_DLL_DECL mob_amanishi_lookoutAI : public ScriptedAI
{
    mob_amanishi_lookoutAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
    uint32 MoveWait_Timer;
    uint32 NextWaypoint;
    bool GauntletStarted;

    
    void AttackStart(Unit* who)
    {
        if(!who || who == m_creature)
            return;

        if(!m_creature->isAlive() || !who->isAlive())
            return;
        
        if(who->GetTypeId()==TYPEID_PLAYER )
        {
            if(((Player*)who)->isGameMaster())
                return;

             EnterCombat(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if(m_creature->getVictim() || !m_creature->IsHostileTo(who) || !who->isInAccessiblePlaceFor(m_creature))
            return;

        if(!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
            return;

        if(!m_creature->IsWithinDistInMap(who, m_creature->GetAttackDistance(who)) || !m_creature->IsWithinLOSInMap(who))
            return;

        //if(m_creature->canAttack(who))
        EnterCombat(who);
    }

    void Reset()
    {
        if(pInstance)
        {
            GauntletStarted = pInstance->GetData(DATA_AKILZONEVENT) == DONE;
            pInstance->SetData(DATA_GAUNTLET,NOT_STARTED);
        }

        m_creature->SetSpeed(MOVE_RUN,2);
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);

        MoveWait_Timer = 0;
    }
    void EnterCombat(Unit *who)
    {
        if(GauntletStarted)
            return;

        DoYell(YELL_START,LANG_UNIVERSAL, NULL);
        if(pInstance)
            pInstance->SetData(DATA_GAUNTLET,IN_PROGRESS);

        NextWaypoint = 0;
        MoveWait_Timer = 2000;
        GauntletStarted = true;
    }
    void MovementInform(uint32 type, uint32 id)
    {
        if(id < 0)
            return;

        if(type != POINT_MOTION_TYPE)
            return;

        if(!GauntletStarted)
            return;

        if( id < 7)
        {
            MoveWait_Timer = 1;
            NextWaypoint++;
        }
        else
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
            //m_creature->DealDamage(m_creature,m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            //m_creature->RemoveCorpse();
        }
    }
    void UpdateAI(const uint32 diff)
    {
        if(!GauntletStarted)
            return;

        if(MoveWait_Timer)
            if(MoveWait_Timer < diff)
            {
                (*m_creature).GetMotionMaster()->MovePoint(NextWaypoint,GauntletWaypoints[NextWaypoint][0],GauntletWaypoints[NextWaypoint][1],GauntletWaypoints[NextWaypoint][2]);
                MoveWait_Timer = 0;
            }else MoveWait_Timer -= diff;
    }
};

CreatureAI* GetAI_mob_amanishi_lookout(Creature *_Creature)
{
    return new mob_amanishi_lookoutAI(_Creature);
}

/*######
## Amani'shi Warrior
######*/

#define SPELL_ANANISHI_WARRIOR_KICK                 43518
#define SPELL_ANANISHI_WARRIOR_CHARGE               43519

struct TRINITY_DLL_DECL mob_amanishi_warriorAI : public ScriptedAI
{
    mob_amanishi_warriorAI(Creature *c) : ScriptedAI(c)
    {
        NextWaypoint = 0;
        Reset();
    }

    uint32 MoveWait_Timer;
    uint32 NextWaypoint;

    uint32 Charge_Timer;
    uint32 Kick_Timer;

    void Reset()
    {
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
        MoveWait_Timer = 1;

        Charge_Timer = 1;
        Kick_Timer = 10000;
    }
    void EnterCombat(Unit *who){}
    void MovementInform(uint32 type, uint32 id)
    {
        if(id < 0)
            return;

        if(type != POINT_MOTION_TYPE)
            return;

        if( id < 7)
        {
            MoveWait_Timer = 1;
            NextWaypoint++;
        }
    }
    void UpdateAI(const uint32 diff)
    {
        if(!UpdateVictim())
        {
            if(MoveWait_Timer)
                if(MoveWait_Timer < diff)
                {
                    (*m_creature).GetMotionMaster()->MovePoint(NextWaypoint,GauntletWaypoints[NextWaypoint][0],GauntletWaypoints[NextWaypoint][1],GauntletWaypoints[NextWaypoint][2]);
                    MoveWait_Timer = 0;
                }else MoveWait_Timer -= diff;
            return;
        }
        else
        {
            if(Charge_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_ANANISHI_WARRIOR_CHARGE);
                Charge_Timer = 20000;
            }else Charge_Timer -= diff;

            if(Kick_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_ANANISHI_WARRIOR_KICK);
                Charge_Timer = 20000;
            }else Charge_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_mob_amanishi_warrior(Creature *_Creature)
{
    return new mob_amanishi_warriorAI(_Creature);
}

/*######
## Amani Eagle
######*/

#define SPELL_ANANI_EAGLE_TALON             43517

struct TRINITY_DLL_DECL mob_amani_eagleAI : public ScriptedAI
{
    mob_amani_eagleAI(Creature *c) : ScriptedAI(c)
    {
        NextWaypoint = 5;
        Reset();
    }

    uint32 MoveWait_Timer;
    uint32 NextWaypoint;

    uint32 Talon_Timer;

    void Reset()
    {
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
        MoveWait_Timer = 1;
        Talon_Timer = 10000;
    }
    void EnterCombat(Unit *who){}
    void MovementInform(uint32 type, uint32 id)
    {
        if(id < 0)
            return;

        if(type != POINT_MOTION_TYPE)
            return;

        if( id > 0)
        {
            MoveWait_Timer = 1;
            NextWaypoint--;
        }
    }
    void UpdateAI(const uint32 diff)
    {
        
        if(!UpdateVictim())
        {
            if(MoveWait_Timer)
                if(MoveWait_Timer < diff)
                {
                    (*m_creature).GetMotionMaster()->MovePoint(NextWaypoint,GauntletWaypoints[NextWaypoint][0],GauntletWaypoints[NextWaypoint][1],GauntletWaypoints[NextWaypoint][2]);
                    MoveWait_Timer = 0;
                }else MoveWait_Timer -= diff;
            return;
        }
        else
        {
            if(Talon_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_ANANI_EAGLE_TALON);
                Talon_Timer = 20000;
            }else Talon_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_mob_amani_eagle(Creature *_Creature)
{
    return new mob_amani_eagleAI(_Creature);
}

/*######
## Amani'shi Tempest
######*/

#define SPELL_AMANISHI_TEMPEST_THUNDERCLAP              44033
#define MOB_AMANISHI_WARRIOR                            24225
#define MOB_AMANI_EAGLE                                 24159

// float EagleSpawnPos[3] = {376.352, 1407.339, 75.532};
float WarriorSpawnPos[3] = {247.396, 1464.760, 26.266};
float EagleSpawnPos[3] = {301.168, 1385.222, 57.678};

struct TRINITY_DLL_DECL mob_amanishi_tempestAI : public ScriptedAI
{
    mob_amanishi_tempestAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
    ScriptedInstance *pInstance;
    uint32 Thunderclap_Timer;

    uint32 phase;
    uint32 EagleSpawn_Timer;
    uint32 EagleCounter;
    uint32 WarriorSpawn_Timer;
    uint32 WarriorCounter;

    void Reset()
    {
        EagleCounter = 5;
        WarriorCounter = 2;
        EagleSpawn_Timer = 30000;
        WarriorSpawn_Timer = 30000;
        Thunderclap_Timer = 10000;
    }
    void EnterCombat(Unit *who)
    {
        if(pInstance)
            pInstance->SetData(DATA_GAUNTLET,DONE);
    }
   
    void UpdateAI(const uint32 diff)
    {
        if(pInstance && pInstance->GetData(DATA_GAUNTLET) == IN_PROGRESS)
        {
            if(m_creature->isDead())
            {
                pInstance->SetData(DATA_GAUNTLET,DONE);
            }
            
            if(WarriorSpawn_Timer < diff)
            {
                if(WarriorCounter > 0)
                {
                    m_creature->SummonCreature(MOB_AMANISHI_WARRIOR,WarriorSpawnPos[0],WarriorSpawnPos[1],WarriorSpawnPos[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,60000);
                    WarriorSpawn_Timer = 500;
                    WarriorCounter--;
                }
                else
                {
                    WarriorCounter = 2;
                    WarriorSpawn_Timer = 59000;
                }
            }else WarriorSpawn_Timer -= diff;

            if(EagleSpawn_Timer < diff)
            {
                if(EagleCounter > 0)
                {
                    m_creature->SummonCreature(MOB_AMANI_EAGLE,EagleSpawnPos[0],EagleSpawnPos[1],EagleSpawnPos[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,60000);
                    EagleSpawn_Timer = 500;
                    EagleCounter--;
                }
                else
                {
                    EagleCounter = 5;
                    EagleSpawn_Timer = 27500;
                }
            }else EagleSpawn_Timer -= diff;
        }

        if(!UpdateVictim())
            return;
      
        if(Thunderclap_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_AMANISHI_TEMPEST_THUNDERCLAP);
            Thunderclap_Timer = 20000;
        }else Thunderclap_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_amanishi_tempest(Creature *_Creature)
{
    return new mob_amanishi_tempestAI(_Creature);
}

/*######
## npc_amanishi_scout
######*/

/* Drumms Pos */
/*
-195.533 1357.579 0.427896
-185.469 1292.650 1.963680
-173.463 1212.119 0.174226
-127.524 1167.949 0.760725
*/

#define SPELL_ALERT_DRUMS                   42177
#define SPELL_MULTI_SHOT                    43205
#define SPELL_SHOOT                         16496
#define MOB_AMANISHI_REINFORCEMENTS         23587

float DrumPositions[4][3] =
{
    {-195.533, 1357.579, 0.4279},
    {-185.469, 1292.650, 1.9637},
    {-173.463, 1212.119, 0.1743},
    {-127.524, 1167.949, 0.7608}
};

float ReinforcementPositions[4][3] =
{
    {-178.348, 1350.270, -0.107},
    {-177.929, 1274.980, 1.5715},
    {-166.024, 1191.630, 0.9356},
    {-112.116, 1159.570, 0.0933}
};

struct TRINITY_DLL_DECL mob_amanishi_scoutAI : public ScriptedAI
{
    mob_amanishi_scoutAI(Creature *c) : ScriptedAI(c)
    {
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
        ReinforcementPhase = 0;
        Reset();
    }

    uint32 shoot_timer;
    uint32 multi_shoot_timer;
    uint32 wait_timer;
    uint32 summon_timer;
    uint32 summons;
    uint32 ReinforcementPhase; // 0 - never moved, 1 - moving,2 - reinformcment called
    uint64 trigger_guid;
    uint32 point;

    void Reset()
    {
        ReinforcementPhase = 0;
        trigger_guid = 0;

        shoot_timer = 1000;
        multi_shoot_timer = 15000;
        wait_timer = 0;
        point = 0;
    }

    uint32 GetNextDrumPos()
    {
        uint32 next;
        float distance = 99999999.9f;

        for(int i = 0; i < 4; i++)
        {
            float t_distance = m_creature->GetDistance(DrumPositions[i][0],DrumPositions[i][1],DrumPositions[i][2]);
            if(distance > t_distance)
            {
                next = (uint32)i;
                distance = t_distance;
            }
        }
        return next;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type != POINT_MOTION_TYPE)
            return;

        if(id == 1 && ReinforcementPhase == 1)
        {
            wait_timer = 5000;
            summons = 2;
            summon_timer = 500;
            (*m_creature).GetMotionMaster()->MoveIdle();
            ReinforcementPhase = 2;
        }
    }
       
    void AttackStart(Unit* who)
    {
        if(ReinforcementPhase == 0)
        {
            if(!who || who == m_creature)
            return;

            if(!m_creature->isAlive() || !who->isAlive())
                return;
            
            if(who->GetTypeId()==TYPEID_PLAYER )
            {
                if(((Player*)who)->isGameMaster())
                    return;

                 CallReinforcements(who);
            }
        }
        else if(ReinforcementPhase == 3)
        {
            ScriptedAI::AttackStart(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if(ReinforcementPhase == 0)
        {
            if(m_creature->getVictim() || !m_creature->IsHostileTo(who) || !who->isInAccessiblePlaceFor(m_creature))
                return;

            if(!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            if(!m_creature->IsWithinDistInMap(who, m_creature->GetAttackDistance(who)) || !m_creature->IsWithinLOSInMap(who))
                return;
        
            if(m_creature->canAttack(who))
                CallReinforcements(who);
        }
        else if(ReinforcementPhase == 3)
        {
            ScriptedAI::MoveInLineOfSight(who);
        }
    }

    void CallReinforcements(Unit *who)
    {
        trigger_guid = who->GetGUID();
        DoYell("Invaders! Sound the Alarm!",LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,12104);
        ReinforcementPhase = 1;
        point = GetNextDrumPos();
        (*m_creature).GetMotionMaster()->Clear();
        (*m_creature).GetMotionMaster()->MovePoint(1,DrumPositions[point][0],DrumPositions[point][1],DrumPositions[point][2]);
    }

    void EnterCombat(Unit *who){}
    void UpdateAI(const uint32 diff)
    {
        if(ReinforcementPhase == 2)
        {
            if(summons > 0)
            {
                if(summon_timer < diff)
                {
                    Unit* target = Unit::GetUnit((*m_creature),trigger_guid);
                    if(target)
                    {
                        Creature* summon = m_creature->SummonCreature(MOB_AMANISHI_REINFORCEMENTS,ReinforcementPositions[point][0],ReinforcementPositions[point][1],ReinforcementPositions[point][2], 0, TEMPSUMMON_DEAD_DESPAWN,60000);
                        summon->SetNoCallAssistance(true);
                        summon->AI()->AttackStart(target);
                    }
                    DoCast(m_creature,SPELL_ALERT_DRUMS);
                    summons--;
                    summon_timer = 500;
                }else summon_timer -= diff;
            }

            if(wait_timer > 0)
                if(wait_timer < diff)
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    ReinforcementPhase = 3;
                    (*m_creature).GetMotionMaster()->Clear();

                    Unit* target = Unit::GetUnit((*m_creature),trigger_guid);
                    if(target)
                    {
                        AttackStart(target);
                    }

                    wait_timer = 0;
                }else wait_timer -= diff;
        }

        if (!UpdateVictim())
            return;

        if(multi_shoot_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MULTI_SHOT);
            multi_shoot_timer = 15000;
        }else multi_shoot_timer -= diff;

        if((*m_creature).GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
        {
            if(m_creature->IsWithinDistInMap(m_creature->getVictim(), 7))
            {
                DoMeleeAttackIfReady();
            }
            else
            {
                if(m_creature->IsWithinDistInMap(m_creature->getVictim(), 20))
                {
                    (*m_creature).GetMotionMaster()->Clear();
                    (*m_creature).GetMotionMaster()->MoveIdle();
                }
            }
        }
        else if((*m_creature).GetMotionMaster()->GetCurrentMovementGeneratorType() == IDLE_MOTION_TYPE)
        {
            if(!m_creature->IsWithinDistInMap(m_creature->getVictim(), 5) && m_creature->IsWithinDistInMap(m_creature->getVictim(), 20))
            {
                if(shoot_timer < diff)
                {
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM,0),SPELL_SHOOT);
                    shoot_timer = 2000;
                }else shoot_timer -= diff;
            }
            else
            {
                (*m_creature).GetMotionMaster()->Clear();
                (*m_creature).GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
        }
    }
};

CreatureAI* GetAI_mob_amanishi_scout(Creature *_Creature)
{
    return new mob_amanishi_scoutAI(_Creature);
}

#define SPELL_AMANI_PROTECTIVE_WARD_PASSIVE_1 42479
#define SPELL_AMANI_PROTECTIVE_WARD_PASSIVE_2 42481

struct TRINITY_DLL_DECL mob_amani_protective_wardAI : public NullCreatureAI
{
    mob_amani_protective_wardAI(Creature *c) : NullCreatureAI(c) {Reset();}
    void Reset()
    {
        DoCast(m_creature,SPELL_AMANI_PROTECTIVE_WARD_PASSIVE_2); 
    }
    void UpdateAI(const uint32 diff)
    {
        m_creature->RemoveAurasDueToSpell(SPELL_AMANI_PROTECTIVE_WARD_PASSIVE_1);
    }
};

CreatureAI* GetAI_mob_amani_protective_ward(Creature *_Creature)
{
    return new mob_amani_protective_wardAI(_Creature);
}


struct TRINITY_DLL_DECL mob_animal_spiritAI : public NullCreatureAI
{
    mob_animal_spiritAI(Creature *c) : NullCreatureAI(c) {}
    void Reset(){}
    void UpdateAI(const uint32 diff) {}
};

CreatureAI* GetAI_mob_animal_spirit(Creature *_Creature)
{
    return new mob_animal_spiritAI(_Creature);
}


void AddSC_zulaman()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_forest_frog";
    newscript->GetAI = GetAI_npc_forest_frog;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_zulaman_hostage";
    newscript->GetAI = GetAI_npc_zulaman_hostage;
    newscript->pGossipHello = GossipHello_npc_zulaman_hostage;
    newscript->pGossipSelect = GossipSelect_npc_zulaman_hostage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_zulaman_harrison_jones";
    newscript->pGossipHello = GossipHello_npc_zulaman_harrison_jones;
    newscript->pGossipSelect = GossipSelect_npc_zulaman_harrison_jones;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_amanishi_lookout";
    newscript->GetAI = GetAI_mob_amanishi_lookout;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_amanishi_warrior";
    newscript->GetAI = GetAI_mob_amanishi_warrior;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_amani_eagle";
    newscript->GetAI = GetAI_mob_amani_eagle;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_amanishi_tempest";
    newscript->GetAI = GetAI_mob_amanishi_tempest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_amanishi_scout";
    newscript->GetAI = GetAI_mob_amanishi_scout;
    newscript->RegisterSelf();  

    newscript = new Script;
    newscript->Name = "mob_amani_protective_ward";
    newscript->GetAI = GetAI_mob_amani_protective_ward;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_animal_spirit";
    newscript->GetAI = GetAI_mob_animal_spirit;
    newscript->RegisterSelf();
}