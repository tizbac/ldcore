#include "precompiled.h"
#include <cstring>
#include "../../../../game/World.h"
#include "../../../../game/Object.h"
#include <pthread.h>
#include <vector>
#include "escort_ai.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <Language.h>
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
class LuaAI;
void luadebug ( std::string msg )
{
    std::cout << "[LUA DEBUG]:" << msg << std::endl;
}

struct luavec3
{
    luavec3 ( float x,float y,float z ) : x ( x ), y ( y ), z ( z ) {}
    float x;
    float y;
    float z;
};
std::map< uint64, std::string > luaglobal;
//Se è 32 Bit va definito questo tipo
namespace luabind
{

template <>
struct default_converter<uint64> : native_converter_base<uint64>
{
    static int compute_score ( lua_State* L, int index )
    {
        return lua_type ( L, index ) == LUA_TUSERDATA ? 0 : -1;
    }

    uint64 from ( lua_State* L, int index )
    {
        uint64 val;

        unsigned char * v = ( unsigned char * ) lua_touserdata ( L, index );
        memcpy ( &val,v,8 );
        return val;
    }

    void to ( lua_State* L, uint64 value )
    {
        unsigned char * raw = ( unsigned char* ) lua_newuserdata ( L,8 );
        memcpy ( raw,&value,8 );
    }
};

template <>
struct default_converter<uint64 const> : default_converter<uint64>
    {};

template <>
struct default_converter<uint64 const&> : default_converter<uint64>
    {};

}
ScriptedAI * L_CastToScriptedAI ( LuaAI * ai )
{
    return ( ScriptedAI* ) ai;

}
LuaAI * L_CastToLuaAI ( ScriptedAI * ai )
{
    return ( LuaAI* ) ai;

}

CreatureAI * L_CastToCreatureAI ( ScriptedAI * ai )
{
    return ( CreatureAI* ) ai;

}
Player* L_Unit2Player ( Unit* u )
{
    if ( ! u->GetTypeId() == TYPEID_PLAYER )
        return NULL;
    else
        return ( Player* ) u;

}
WorldObject * L_CastToWorldObject( Unit * u)
{
  return (WorldObject*)u;
}
std::string L_GetLuaDir()
{
  return sWorld.GetDataPath()+"ai/";
  
  
  
}
ScriptedInstance * L_InstanceData2ScriptedInstance( InstanceData * i )
{
  return (ScriptedInstance*)i;
}
std::string GetLuaError ( lua_State* L )
{
    if ( !lua_isstring ( L, lua_gettop ( L ) ) )
        return "no error";

    std::string str = std::string ( lua_tostring ( L, lua_gettop ( L ) ) );
    lua_pop ( L, 1 );

    return str;
}

uint64 NullGuid()
{
    return 0;
}
void lua_sysmsg(const char * message)
{
  Trinity::Singleton<World>::Instance().SendWorldText(LANG_SYSTEMMESSAGE,message);
  
}
#define LOCK pthread_mutex_lock(&lock)
#define UNLOCK pthread_mutex_unlock(&lock)
#define RESET_ENTER resetting = true;
#define RESET_LEAVE resetting = false;
struct TRINITY_DLL_DECL LuaAI : public ScriptedAI
{
public:
    lua_State * L;
    pthread_mutex_t lock;
    bool lockcreated;
    bool resetting;
    LuaAI ( Creature *c ) : ScriptedAI ( c )
    {
        L = NULL;
        lockcreated = false;
        if ( pthread_mutex_init ( &lock, NULL ) != 0 )
        {
            sLog.outError ( "Impossibile creare il lock su LuaAI" );
            abort();
        }
        else
            lockcreated = true;
        resetting = false;
        SetCombatMovement(true);
        ReloadLUA();
    }

    ~LuaAI()
    {
        if ( L )
            lua_close ( L );
        if ( lockcreated )
            pthread_mutex_destroy ( &lock );

    }
    void CallLuaFunctionOnOtherAI( Creature * c,std::string function, std::string param )
    {
      if ( !c )
      {
        return;
      }
      if ( c == m_creature )
      {
        return;
      }
      CreatureAI * cAI = c->AI();
      if ( c->GetScriptName() == "luaai" )
      {
        LuaAI * lAI = (LuaAI*)cAI;
        if ( ! lAI->L )
        {
          sLog.outError("Il lua sull'npc '%s' ( GUID : %llu , Entry : %d ) non è inizializzato, impossibile chiamare la funzione",c->GetName(),c->GetGUID(),c->GetEntry());
          return;
        }
        try{
          luabind::call_function<void> (lAI->L,function.c_str(),param);
        }catch ( luabind::error e )
        {
            sLog.outError("Impossibile chiamare la funzione '%s' sull'AI Lua dell'npc '%s' con GUID %llu con entry %d : %s",function.c_str(),c->GetName(),c->GetGUID(),c->GetEntry(),GetLuaError ( lAI->L )); 
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
        
      }else{
        sLog.outError("L'npc '%s' ( GUID : %llu , Entry : %d ) non ha una AI 'luaai' ma una ai '%s', impossibile chiamare la funzione",c->GetName(),c->GetGUID(),c->GetEntry(),c->GetAIName());
      }
      
      
    }
    void ReloadLUA()
    {
        EnterEvadeMode();
        LOCK;
        if ( L )
            lua_close ( L );

        L = lua_open();
        if ( not L )
        {
            sLog.outError ( "ERRORE GRAVE: Impossibile creare il contesto lua per l'AI di %s, Ci sarà un crash fra poco",m_creature->GetName() );
            return;
        }
        luaL_openlibs ( L );
        luabind::open ( L );
        luabind::module ( L )
        [
            luabind::def ( "logdebug",luadebug ),
            luabind::def ( "NullGuid",NullGuid ),
            luabind::class_<uint64> ( "uint64" ),
            luabind::class_<UnitAI> ( "UnitAI" ),
            luabind::class_<CreatureAI,UnitAI> ( "CreatureAI" )
            .def ( "DoZoneInCombat", &CreatureAI::DoZoneInCombat )
            .def ( "DoSpellAttackIfReady", &CreatureAI::DoSpellAttackIfReady )
            .def ( "DoCast" , ( void ( CreatureAI::* ) ( Unit*,uint32,bool ) ) &CreatureAI::DoCast )
            .def ( "DoCastAOE" , &CreatureAI::DoCastAOE )
            .def ( "AttackStart" , ( void ( ScriptedAI::* ) ( Unit*,bool ) ) &ScriptedAI::AttackStart )
            .def ( "DoGetSpellMaxRange", &CreatureAI::DoGetSpellMaxRange )
            .def ( "DoAction", &CreatureAI::DoAction )
            .def ( "AttackStartCaster" , &CreatureAI::AttackStartCaster )
            .def ( "DoMeleeAttackIfReady" , &CreatureAI::DoMeleeAttackIfReady )
            .def ( "GetData", &CreatureAI::GetData )
            .def ( "FillAISpellInfo" , &CreatureAI::FillAISpellInfo )
            .def ( "GetGUID" , &CreatureAI::GetGUID )
            .def ( "EnterEvadeMode", &CreatureAI::EnterEvadeMode )
            ,
            
            luabind::class_<CleanDamage> ("CleanDamage"),
            luabind::class_<ScriptedAI,CreatureAI> ( "ScriptedAI" )
            .def ( "DoTeleportPlayer",&ScriptedAI::DoTeleportPlayer )
            .def ( "DoTeleportAll",&ScriptedAI::DoTeleportAll )
            .def ( "DoTeleportTo", ( void ( ScriptedAI::* ) ( float,float,float,uint32 ) ) &ScriptedAI::DoTeleportTo )
            .def ( "DoSpawnCreature" , &ScriptedAI::DoSpawnCreature )
            .def ( "DoResetThreat", &ScriptedAI::DoResetThreat )
            .def ( "DoFindFriendlyCC", &ScriptedAI::DoFindFriendlyCC )
            .def ( "DoGetThreat", &ScriptedAI::DoGetThreat )
            .def ( "DoTextEmote", &ScriptedAI::DoTextEmote )
            .def ( "DoWhisper", &ScriptedAI::DoWhisper )
            .def ( "DoYell", &ScriptedAI::DoYell )
            .def ( "DoSay", &ScriptedAI::DoSay )
            .def ( "DoPlaySoundToSet", &ScriptedAI::DoPlaySoundToSet )
            .def ( "DoStopAttack", &ScriptedAI::DoStopAttack )
            .def ( "DoZoneInCombat", &ScriptedAI::DoZoneInCombat )
            .def ( "DoSpellAttackIfReady", &ScriptedAI::DoSpellAttackIfReady )
            .def ( "DoCast" , ( void ( ScriptedAI::* ) ( Unit*,uint32,bool ) ) &ScriptedAI::DoCast )
            .def ( "DoCastAOE" , &ScriptedAI::DoCastAOE )
            .def ( "AttackStart" , ( void ( ScriptedAI::* ) ( Unit*,bool ) ) &ScriptedAI::AttackStart )
            .def ( "DoGetSpellMaxRange", &ScriptedAI::DoGetSpellMaxRange )
            .def ( "DoAction", &ScriptedAI::DoAction )
            .def ( "AttackStartCaster" , &ScriptedAI::AttackStartCaster )
            .def ( "DoMeleeAttackIfReady" , &ScriptedAI::DoMeleeAttackIfReady )
            .def ( "GetData", &ScriptedAI::GetData )
            .def ( "FillAISpellInfo" , &ScriptedAI::FillAISpellInfo )
            .def ( "GetGUID" , &ScriptedAI::GetGUID )
            .def ( "DoModifyThreatPercent", &ScriptedAI::DoModifyThreatPercent )
            ,
            luabind::class_<HostilReference> ("HostilReference" )
              .def ( "getUnitGuid", &HostilReference::getUnitGuid )
              ,
            luabind::class_<LuaAI,ScriptedAI> ( "LuaAI" )
            .def ( "SelectUnit", &LuaAI::L_SelectUnit )
            .def ( "SelectUnit2", &LuaAI::L_SelectUnit2 )
            .def ( "UpdateVictim" , &LuaAI::L_UpdateVictim )
            .def ( "CallLuaFunctionOnOtherAI" , &LuaAI::CallLuaFunctionOnOtherAI)
            .def ( "EnterEvadeMode" , &LuaAI::L_EnterEvadeMode ),
            luabind::class_<Item> ( "Item" ),
            luabind::class_<Aura> ( "Aura" ),
            luabind::class_<Map> ( "Map" )
            .def ( "IsHeroic", &Map::IsHeroic )
            .def ( "IsBattleArena" , &Map::IsBattleArena )
            .def ( "IsBattleGroundOrArena" , &Map::IsBattleGroundOrArena )
            .def ( "IsDungeon" , &Map::IsDungeon )
            .def ( "IsInWater" , &Map::IsInWater )
            .def ( "IsUnderWater" , &Map::IsUnderWater )
            ,
            luabind::class_<InstanceData> ( "InstanceData" )
            .def ( "GetData64" , &InstanceData::GetData64 )
            .def ( "SetData64" , &InstanceData::SetData64 )
            .def ( "IsEncounterInProgress" , &InstanceData::IsEncounterInProgress )
            .def ( "SetData" , &InstanceData::SetData )
            .def ( "GetData" , &InstanceData::GetData )
            ,
            luabind::class_<ScriptedInstance,InstanceData> ( "ScriptedInstance" )
              .def ( "GetData64" , &ScriptedInstance::GetData64 )
              .def ( "SetData64" , &ScriptedInstance::SetData64 )
              .def ( "IsEncounterInProgress" , &ScriptedInstance::IsEncounterInProgress )
              .def ( "SetData" , &ScriptedInstance::SetData )
              .def ( "GetData" , &ScriptedInstance::GetData )
            ,
            luabind::class_<SpellEntry> ( "SpellEntry" )
              .def_readonly ( "Id", &SpellEntry::Id )
              ,
            luabind::class_<Object> ( "Object" ),
            luabind::class_<WorldLocation> ( "WorldLocation" )
            .def_readwrite ( "mapid",&WorldLocation::mapid )
            .def_readwrite ( "x",&WorldLocation::x )
            .def_readwrite ( "y",&WorldLocation::y )
            .def_readwrite ( "z",&WorldLocation::z )
            .def_readwrite ( "o",&WorldLocation::o ),
            luabind::class_<WorldObject,Object,WorldLocation> ( "WorldObject" )
            .def ( "GetPositionX",&WorldObject::GetPositionX )
            .def ( "GetPositionY",&WorldObject::GetPositionY )
            .def ( "GetPositionZ",&WorldObject::GetPositionZ )
            .def ( "GetMapId",&WorldObject::GetMapId ),
            luabind::class_<luavec3> ( "Vec3" )
            .def ( luabind::constructor<float,float,float>() )
            .def_readwrite ( "x",&luavec3::x )
            .def_readwrite ( "y",&luavec3::y )
            .def_readwrite ( "z",&luavec3::z ),
            luabind::class_<Unit,WorldObject> ( "Unit" )
            .def ( "GetName",&Unit::GetName )
            .def ( "CastSpell", ( void ( Unit::* ) ( Unit*,uint32,bool,Item*,Aura*,uint64 ) ) &Unit::CastSpell )
            .def ( "GetHealth",&Unit::GetHealth )
            .def ( "GetMaxHealth",&Unit::GetMaxHealth )
            .def ( "SetHealth",&Unit::SetHealth )
            .def ( "SetMaxHealth",&Unit::SetMaxHealth )
            .def ( "Mount",&Unit::Mount )
            .def ( "GetGUID",&Unit::GetGUID )
            .def ( "DealDamage",(uint32(Unit::*)(Unit*,uint32,const CleanDamage*,int,int,const SpellEntry*,bool))&Unit::DealDamage )
            .def ( "Kill",&Unit::Kill )
            .def ( "HasAuraType",&Unit::HasAuraType )
            .def ( "HasAura",&Unit::HasAura )
            .def ( "GetDistance2d", (float(WorldObject::*)(const WorldObject*)const)&WorldObject::GetDistance2d )
            .def ( "isAlive" , &Unit::isAlive )
            .def ( "setFaction",&Unit::setFaction )
            .def ( "SetFlag",&Unit::SetFlag )
            .def ( "RemoveAurasDueToSpell" , &Unit::RemoveAurasDueToSpell)
            .def ( "RemoveAllAuras", &Unit::RemoveAllAuras)
            .def ( "HasFlag",&Unit::HasFlag )
            .def ( "GetTypeID", &Unit::GetTypeId )
            .def ( "ToggleFlag", ( void ( Unit::* ) ( uint16,uint32 ) ) &Unit::ToggleFlag )
            .def ( "RemoveFlag",&Unit::RemoveFlag )
            .def ( "RemoveAura", ( void ( Unit::* ) ( uint32,uint32,Aura* ) ) &Unit::RemoveAura ),
            luabind::class_<QuestMenu> ( "QuestMenu" ),
            luabind::class_<ThreatManager> ( "ThreatManager" )
              .def ( "getThreatList" , &ThreatManager::getThreatList, luabind::return_stl_iterator)
              ,
            luabind::class_<WorldSession> ( "WorldSession" ),
            luabind::class_<MotionMaster> ( "MotionMaster" )
            .def ( "MoveCharge" , &MotionMaster::MoveCharge )
            .def ( "MoveChase" , &MotionMaster::MoveChase  )
            .def ( "MoveConfused" , &MotionMaster::MoveConfused )
            .def ( "MoveDistract" , &MotionMaster::MoveDistract )
            .def ( "MoveFall" , &MotionMaster::MoveFall )
            .def ( "MoveFleeing" , &MotionMaster::MoveFleeing )
            .def ( "MoveFollow" , &MotionMaster::MoveFollow )
            .def ( "MoveIdle" , &MotionMaster::MoveIdle )
            .def ( "MoveJump" , &MotionMaster::MoveJump )
            .def ( "MoveJumpTo" , &MotionMaster::MoveJumpTo )
            .def ( "MovePath" , &MotionMaster::MovePath )
            .def ( "MovePoint" , &MotionMaster::MovePoint )
            .def ( "MoveRandom" , &MotionMaster::MoveRandom )
            .def ( "MoveRotate" , &MotionMaster::MoveRotate )
            .def ( "MoveSeekAssistance" , &MotionMaster::MoveSeekAssistance )
            .def ( "MoveSeekAssistanceDistract" , &MotionMaster::MoveSeekAssistanceDistract )
            .def ( "MoveTargetedHome" , &MotionMaster::MoveTargetedHome )
            .def ( "MoveTaxiFlight" , &MotionMaster::MoveTaxiFlight )
            .def ( "Clear" , &MotionMaster::Clear )

            ,
            luabind::class_<PlayerMenu> ( "PlayerMenu" )
            .def ( "GetGossipMenu",&PlayerMenu::GetGossipMenu )
            .def ( "SendGossipMenu",&PlayerMenu::SendGossipMenu )
            .def ( "CloseGossip",&PlayerMenu::CloseGossip ),
            luabind::class_<GossipMenu> ( "GossipMenu" )
            .def ( "AddMenuItem", ( void ( GossipMenu::* ) ( uint8 , char const* , uint32 , uint32, char const*, uint32 , bool ) ) &GossipMenu::AddMenuItem )
            .def ( "ClearMenu",&GossipMenu::ClearMenu ),
            luabind::class_<Player,Unit> ( "Player" )
            .def ( "GetGUID",&Player::GetGUID )
            .def_readwrite ( "PlayerTalkClass",&Player::PlayerTalkClass )
            .def ( "GetName",&Player::GetName )
            .def ( "CastSpell", ( void ( Player::* ) ( Unit*,uint32,bool,Item*,Aura*,uint64 ) ) &Player::CastSpell )
            .def ( "GetHealth",&Player::GetHealth )
            .def ( "GetMaxHealth",&Player::GetMaxHealth )
            .def ( "SetHealth",&Player::SetHealth )
            .def ( "SetMaxHealth",&Player::SetMaxHealth )
            .def ( "Mount",&Player::Mount )
            .def ( "GetLevel",&Player::getLevel)
            .def ( "DealDamage",&Player::DealDamage )
            .def ( "Kill",&Player::Kill )
            .def ( "HasAuraType",&Player::HasAuraType )
            .def ( "HasAura",&Player::HasAura )
            .def ( "setFaction",&Player::setFaction )
            .def ( "SetFlag",&Player::SetFlag )
            .def ( "HasFlag",&Player::HasFlag )
            .def ( "ToggleFlag", ( void ( Player::* ) ( uint16,uint32 ) ) &Player::ToggleFlag )
            .def ( "RemoveFlag",&Player::RemoveFlag )
            .def ( "RemoveAura", ( void ( Player::* ) ( uint32,uint32,Aura* ) ) &Player::RemoveAura )
            .def ( "TeleportTo", ( bool ( Player::* ) ( uint32,float,float,float,float,uint32 ) ) &Player::TeleportTo )
            .def ( "BuildPlayerRepop",&Player::BuildPlayerRepop )
            .def ( "RepopAtGraveyard",&Player::RepopAtGraveyard )
            .def ( "GetTeam",&Player::GetTeam )
            .def ( "PrepareQuestMenu",&Player::PrepareQuestMenu )
            .def ( "GetQuestStatus",&Player::GetQuestStatus )
            .def ( "getStandState",&Player::getStandState )
            .def ( "RemoveAllAuras", &Player::RemoveAllAuras)
            .def ( "GetQuestRewardStatus",&Player::GetQuestRewardStatus )
            .def ( "AreaExploredOrEventHappens",&Player::AreaExploredOrEventHappens )
            .def ( "GroupEventHappens" , &Player::GroupEventHappens )
            .def ( "RemoveAurasDueToSpell" , &Player::RemoveAurasDueToSpell )
            .def ( "GiveXP" , &Player::GiveXP ),
            //.def("CastSpell",(void(Unit::*)(Unit* , uint32 , bool , Item *, Aura* , uint64 ))Unit::CastSpell),
            luabind::class_<Creature,Unit> ( "Creature" )
            .def ( "Say", ( void ( Creature::* ) ( const char*,uint32,uint64 ) ) &Creature::Say )
            .def ( "Yell", ( void ( Creature::* ) ( const char*,uint32,uint64 ) ) &Creature::Yell )
            .def ( "SummonCreature",&Creature::SummonCreature )
            .def ( "SetFlag",&Creature::SetFlag )
            .def ( "GetGUID",&Creature::GetGUID )
            .def ( "GetNpcTextId",&Creature::GetNpcTextId )
            .def ( "GetAI",&Creature::AI )
            .def ( "GetName",&Creature::GetName )
            .def ( "CastSpell", ( void ( Creature::* ) ( Unit*,uint32,bool,Item*,Aura*,uint64 ) ) &Unit::CastSpell )
            .def ( "GetHealth",&Creature::GetHealth )
            .def ( "HandleEmoteCommand", &Creature::HandleEmoteCommand )
            .def ( "GetMaxHealth",&Creature::GetMaxHealth )
            .def ( "SetHealth",&Creature::SetHealth )
            .def ( "SetMaxHealth",&Creature::SetMaxHealth )
            .def ( "GetMap", &Creature::GetMap )
            .def ( "GetDistance2d", (float(WorldObject::*)(const WorldObject*)const)&WorldObject::GetDistance2d )
            .def ( "isAlive" , &Creature::isAlive )
            .def ( "Mount",&Creature::Mount )
            .def ( "DealDamage",&Creature::DealDamage )
            .def ( "Kill",&Creature::Kill )
            .def ( "Relocate", (void ( Creature::* )(float,float,float,float))&Creature::Relocate )
            .def ( "StopMoving" , &Creature::StopMoving )
            .def ( "GetMotionMaster", &Creature::GetMotionMaster )
            .def ( "GetHomePosition", &Creature::GetHomePosition )
            .def ( "HasAuraType",&Creature::HasAuraType )
            .def ( "HasAura",&Creature::HasAura )
            .def ( "setFaction",&Creature::setFaction )
            .def ( "getFaction",&Creature::getFaction )
            .def ( "SetFlag",&Creature::SetFlag )
            .def ( "HasFlag",&Creature::HasFlag )
            .def ( "ForcedDespawn", &Creature::ForcedDespawn )
            .def ( "Respawn" , &Creature::Respawn )
            .def ( "RemoveAllAuras", &Creature::RemoveAllAuras)
            .def ( "ToggleFlag", ( void ( Creature::* ) ( uint16,uint32 ) ) &Creature::ToggleFlag )
            .def ( "RemoveFlag",&Creature::RemoveFlag )
            .def ( "RemoveAura", ( void ( Creature::* ) ( uint32,uint32,Aura* ) ) &Creature::RemoveAura )
            .def ( "GetInstanceData" , &Creature::GetInstanceData )
            .def ( "IsNonMeleeSpellCasted" , &Creature::IsNonMeleeSpellCasted )
            .def ( "IsWithinMeleeRange" , &Creature::IsWithinMeleeRange )
            .def ( "TauntApply" , &Creature::TauntApply )
            .def ( "getThreatManager" , &Creature::getThreatManager )
            .def ( "isQuestGiver",&Creature::isQuestGiver )
            .def ( "getVictim", &Creature::getVictim )
            .def ( "RemoveAurasDueToSpell" , &Creature::RemoveAurasDueToSpell )
            .def ( "SetDisplayId", &Creature::SetDisplayId )
            .def ( "ResetPlayerDamageReq", &Creature::ResetPlayerDamageReq )
            .def ( "SummonGameObject" , &Creature::SummonGameObject )
            .def ( "MonsterTextEmote",(void(Creature::*)(const char *,uint64,bool))&Creature::MonsterTextEmote),
            luabind::def ( "GetScriptedAI",&L_CastToScriptedAI ),
            luabind::def ( "GetLuaAI",&L_CastToLuaAI ),
            luabind::def ( "ToPlayer",&L_Unit2Player ),
            luabind::def ( "GetCreatureAI", &L_CastToCreatureAI ),
            luabind::def ( "GetScriptedInstance" , &L_InstanceData2ScriptedInstance),
            luabind::def ( "DoScriptText" , &DoScriptText),
            luabind::def ( "GetUnit" , &Unit::GetUnit),
            luabind::def ( "SystemMessage", &lua_sysmsg),
            luabind::def ( "ToWorldObject",&L_CastToWorldObject),
            luabind::def ( "GetLuaDir", &L_GetLuaDir ),
            luabind::def ( "GetClosestCreatureWithEntry" , &GetClosestCreatureWithEntry),
            //luabind::def ( "GetClosestDeadCreature" , &GetClosestDeadCreature),
            luabind::def ( "GetCreature" , &ObjectAccessor::GetCreature ),
            luabind::def ( "GetGameObjt" , &ObjectAccessor::GetGameObject ),
            luabind::def ( "GetPlayer" , &ObjectAccessor::GetPlayer ),
            luabind::def ( "FindPlayerByName" , &ObjectAccessor::FindPlayerByName ),
            luabind::def ( "GetUnit" , &ObjectAccessor::GetUnit )
            //def("DoZoneInCombat", &func)
        ];
       
        std::stringstream luaname;
        luaname << sWorld.GetDataPath() << "ai/" << m_creature->GetEntry() << ".lua";
        std::cout << "Caricamento file lua: " << luaname.str() << std::endl;
        luaL_dofile ( L,luaname.str().c_str() );
        if ( lua_isstring ( L,-1 ) )
        {
            if ( strlen ( lua_tostring ( L, -1 ) ) > 0 )
                HandleLuaError ( "ReloadLUA" );
        }
        RESET_ENTER
        try
        {
            luabind::call_function<void> ( L,"Reset",m_creature,this );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "Reset" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
        RESET_LEAVE
        UNLOCK;


    }
    void HandleLuaError ( std::string func, bool killcreature = true )
    {
        std::cout << "Errore Lua (" << func << "):" << std::endl;
        std::cout << GetLuaError ( L ) << std::endl;
        m_creature->Say ( ( std::string ( "|c00ff0000Errore Lua("+func+"):" ) +GetLuaError ( L ) +"|r" ).c_str(),LANG_UNIVERSAL,0 );
        if ( killcreature )
        {
            m_creature->Kill ( m_creature,false );
            m_creature->SetLootRecipient ( NULL ); // Dovrebbe impedire free loot
        }
    }

    void DamageTaken ( Unit* pDone_by, uint32& uiDamage )
    {
        try
        {
            uiDamage = luabind::call_function<int> ( L,"DamageTaken",pDone_by,uiDamage );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "DamageTaken" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void WaypointReached ( uint32 i )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"WaypointReached",i );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "WaypointReached" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    /*void MoveInLineOfSight ( Unit * u )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"MoveInLineOfSight",u );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "MoveInLineOfSight" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }*/

    /*void EnterEvadeMode()
    {
        try
        {
            luabind::call_function<void> ( L,"EnterEvadeMode" );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "EnterEvadeMode" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }*/ // Non serviva a un cazzo
    void L_EnterEvadeMode()
    {
        if ( resetting )
        {
            m_creature->Say ( "|c00ff0000Errore LUA: Chiamata a EnterEvadeMode da Reset(), causerebbe un loop infinito|r",0,0 );
            return;
        }
        EnterEvadeMode();
    }
    void EnterCombat ( Unit* u )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"EnterCombat",u );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "EnterCombat" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void KilledUnit ( Unit * u )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"KilledUnit",u );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "KilledUnit",false );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void JustSummoned ( Creature * c )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"JustSummoned",c );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "JustSummoned" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void IsSummonedBy ( Unit* u )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"IsSummonedBy",u );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "IsSummonedBy" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void SummonedCreatureDespawn ( Creature * c )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"SummonedCreatureDespawn",c );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "SummonedCreatureDespawn" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void SpellHit ( Unit* u, const SpellEntry* s )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"SpellHit",u,s );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "SpellHit" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void MovementInform ( uint32 mt, uint32 data )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"MovementInform",mt,data );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "MovementInform" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void SpellHitTarget ( Unit* target, const SpellEntry* s )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"SpellHitTarget",target,s );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "SpellHitTarget" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }

    void UpdateAI ( const uint32 diff )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"UpdateAI",diff );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "UpdateAI" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }
    Unit* L_SelectUnit ( SelectAggroTarget t,int p )
    {
        return SelectUnit ( t,p );
    }
    Unit* L_SelectUnit2 ( SelectAggroTarget t,int p,float dist,bool po )
    {
        return SelectUnit ( t,p,dist,po );
    }

    bool L_UpdateVictim()
    {
        return UpdateVictim();

    }
    void Reset()
    {
        if ( ! L )
            return;
        RESET_ENTER
        LOCK;
        sLog.outDebug ( "C++: LuaAI: Reset()" );
        try
        {
            luabind::call_function<void> ( L,"Reset",m_creature,this );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "Reset" );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
        UNLOCK;
        RESET_LEAVE
    }

    void JustDied ( Unit* killer )
    {
        if ( ! L )
            return;
        try
        {
            luabind::call_function<void> ( L,"JustDied",killer );
        }
        catch ( luabind::error e )
        {
            HandleLuaError ( "JustDied",false );
        }
        catch ( luabind::cast_failed e )
        {
            std::cout << "ERRORE LUA: " << e.what() << std::endl;
        }
    }
};

bool GossipHello_luaai ( Player *player, Creature *_Creature )
{
    std::cout << "LUAAI: GossipHello" << std::endl;
    if ( ( ( LuaAI* ) ( _Creature->AI() ) )->L )
    {
        lua_State*L = ( ( LuaAI* ) ( _Creature->AI() ) )->L;
        if ( ! L )
            return false;
        try
        {
            luabind::call_function<void> ( L,"GossipHello",player );
        }
        catch ( luabind::error e )
        {
            ( ( LuaAI* ) ( _Creature->AI() ) )->HandleLuaError ( "GossipHello" );
            return false;
        }
    }
    else
    {
        std::cout << "Impossibile trovare il contesto LUA sull'AI" << std::endl;
    }
    return true;
}

bool GossipSelect_luaai ( Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if ( ( ( LuaAI* ) ( _Creature->AI() ) )->L )
    {
        lua_State*L = ( ( LuaAI* ) ( _Creature->AI() ) )->L;
        if ( ! L )
            return false;
        try
        {
            luabind::call_function<void> ( L,"GossipSelect",player,sender,action );
        }
        catch ( luabind::error e )
        {
            ( ( LuaAI* ) ( _Creature->AI() ) )->HandleLuaError ( "GossipSelect" );
            return false;
        }
    }
    return true;
}

bool GossipSelectWithCode_luaai ( Player *player, Creature *_Creature, uint32 sender, uint32 action, const char* sCode )
{
    if ( ( ( LuaAI* ) ( _Creature->AI() ) )->L )
    {
        lua_State*L = ( ( LuaAI* ) ( _Creature->AI() ) )->L;
        if ( ! L )
            return false;
        try
        {
            luabind::call_function<void> ( L,"GossipSelectWithCode",player,sender,action,sCode );
        }
        catch ( luabind::error e )
        {
            ( ( LuaAI* ) ( _Creature->AI() ) )->HandleLuaError ( "GossipSelectWithCode" );
            return false;
        }
    }
    return true;
}

CreatureAI* GetAI_LuaAI ( Creature* pCreature )
{
    LuaAI * AI = new LuaAI ( pCreature );
    return ( CreatureAI* ) AI;
}

void AddSC_LuaAI()
{
    Script * newscript;
    newscript = new Script;
    newscript->Name="luaai";
    newscript->GetAI=&GetAI_LuaAI;
    newscript->pGossipHello = &GossipHello_luaai;
    newscript->pGossipSelect = &GossipSelect_luaai;
    newscript->pGossipSelectWithCode = &GossipSelectWithCode_luaai;
    newscript->RegisterSelf();
}

void _LUARELOAD ( CreatureAI * ai )
{
    ( ( LuaAI* ) ai )->ReloadLUA();


}
