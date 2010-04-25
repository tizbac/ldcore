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
SDName: Boss_Headless_Horseman
SD%Complete:
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"
#include "SpellMgr.h"
#include "def_scarlet_monastery.h"

#define SAY_ENTRANCE                -1189001
#define SAY_REJOINED                -1189002
#define SAY_LOST_HEAD               -1189003
#define SAY_CONFLAGRATION           -1189004
#define SAY_SPROUTING_PUMPKINS      -1189005
#define SAY_PLAYER_DEATH            -1189006
#define SAY_DEATH                   -1189007

uint32 RandomLaugh[] = {11965, 11975, 11976};

    // Entryes
#define HH_MOUNTED                  23682
#define HH_UNHORSED                 23800
#define HEAD                        23775
#define PULSING_PUMPKIN             23694
#define PUMPKIN_FIEND               23545
#define HELPER                      23686
#define WISP_INVIS                  24034

    //Spells
#define SPELL_CLEAVE                42587
#define SPELL_CONFLAGRATION         42380       //Phase 2, can't find real spell(Dim Fire?)
//#define SPELL_CONFL_SPEED         22587       //8% increase speed, value 22587 from SPELL_CONFLAGRATION mains that spell?
#define SPELL_SUMMON_PUMPKIN        42394

#define SPELL_WHIRLWIND             43116
#define SPELL_IMMUNE                42556
#define SPELL_BODY_REGEN            42403
#define SPELL_CONFUSE               43105

#define SPELL_FLYING_HEAD           42399       //visual flying head
#define SPELL_HEAD                  42413       //visual buff, "head"
#define SPELL_HEAD_IS_DEAD          42428       //at killing head, Phase 3

#define SPELL_PUMPKIN_AURA          42280
#define SPELL_PUMPKIN_AURA_GREEN    42294
#define SPELL_SQUASH_SOUL           42514
#define SPELL_SPROUTING             42281
#define SPELL_SPROUT_BODY           42285

    //Effects
#define SPELL_RHYME_BIG             42909
//#define SPELL_RHYME_SMALL         42910
#define SPELL_HEAD_SPEAKS           43129
#define SPELL_HEAD_LANDS            42400
#define SPELL_BODY_FLAME            42074
#define SPELL_HEAD_FLAME            42971
//#define SPELL_ENRAGE_VISUAL       42438       // he uses this spell?
#define SPELL_WISP_BLUE             42821
#define SPELL_WISP_FLIGHT_PORT      42818
//#define SPELL_WISP_INVIS          42823
#define SPELL_SMOKE                 42355
#define SPELL_DEATH                 42566       //not correct spell

struct Locations
{
    float x, y, z;
};

static Locations FlightPoint[]=
{
    {1754.00,1346.00,17.50},
    {1765.00,1347.00,19.00},
    {1784.00,1346.80,25.40},
    {1803.30,1347.60,33.00},
    {1824.00,1350.00,42.60},
    {1838.80,1353.20,49.80},
    {1852.00,1357.60,55.70},
    {1861.30,1364.00,59.40},
    {1866.30,1374.80,61.70},
    {1864.00,1387.30,63.20},
    {1854.80,1399.40,64.10},
    {1844.00,1406.90,64.10},
    {1824.30,1411.40,63.30},
    {1801.00,1412.30,60.40},
    {1782.00,1410.10,55.50},
    {1770.50,1405.20,50.30},
    {1765.20,1400.70,46.60},
    {1761.40,1393.40,41.70},
    {1759.10,1386.70,36.60},
    {1757.80,1378.20,29.00},
    {1758.00,1367.00,19.51}
};

static Locations Spawn[]=
{
    {1776.27,1348.74,19.20},    //spawn point for pumpkin shrine mob
    {1765.28,1347.46,17.55}     //spawn point for smoke
};

struct Summon
{
    const std::string text;
};

static Summon Text[]=
{
    {"Horseman rise..."},
    {"Your time is nigh..."},
    {"You felt death once..."},
    {"Now, know demise!"}
};

struct TRINITY_DLL_DECL mob_wisp_invisAI : public ScriptedAI
{
    mob_wisp_invisAI(Creature *c) : ScriptedAI(c)
    {
        Creaturetype = delay = spell = spell2 = 0;
        //that's hack but there are no info about range of this spells in dbc
        SpellEntry *wisp = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_WISP_BLUE);
        if (wisp)
            wisp->rangeIndex = 6;

        SpellEntry *port = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_WISP_FLIGHT_PORT);
        if (port)
            port->rangeIndex = 6;
    }

    uint32 Creaturetype;
    uint32 delay;
    uint32 spell;
    uint32 spell2;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetDisplayId(21908);    //Display dell'head of horseman che in pratica è invisibile.
    }
    void EnterCombat(Unit *who){}
    void SetType(uint32 _type)
    {
        Creaturetype = _type;
        switch(Creaturetype)
        {
        case 1:
            spell = SPELL_PUMPKIN_AURA_GREEN; break;
        case 2:
            delay = 15000;  spell = SPELL_BODY_FLAME;spell2 = SPELL_DEATH; break;
        case 3:
            delay = 120000;  spell = SPELL_SMOKE; break;
        case 4:
            m_creature->RemoveAurasDueToSpell(SPELL_SMOKE);
            delay = 7000;   spell2 = SPELL_WISP_BLUE; break;
        }
        if (spell)DoCast(m_creature,spell);
    }

    void SpellHit(Unit* caster, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_WISP_FLIGHT_PORT && Creaturetype == 4)
            m_creature->SummonCreature(23904,0,0,0,0,TEMPSUMMON_TIMED_DESPAWN,600000);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || Creaturetype != 1 || !who->isTargetableForAttack())
            return;

        if (m_creature->GetDistance2d(who) < 0.1 && !who->HasAura(SPELL_SQUASH_SOUL,0))
            DoCast(who,SPELL_SQUASH_SOUL);
    }

    void UpdateAI(const uint32 diff)
    {
        if(delay)
        {
            if (delay <= diff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SMOKE);
                if(spell2)
                    DoCast(m_creature,spell2);
                delay = 0;
            }else delay -= diff;
        }
    }
};

struct TRINITY_DLL_DECL mob_headAI : public ScriptedAI
{
    mob_headAI(Creature *c) : ScriptedAI(c){}

    
    uint64 uBodyGUID;
    uint32 uPhase;
    uint32 uDieTimer;
    uint32 uLaughTimer;

    bool   bHaveBody;
    bool   bIsDied;
    bool   bEventDone;

    void Reset()
    {
        uBodyGUID   = 0;
        uPhase      = 0;
        uDieTimer   = 2000;
        uLaughTimer = 4000;
        bHaveBody   = false;
        bIsDied     = false;
        bEventDone  = false;

        //Di base l'npc è invisibile ma selezionabile basta non farlo selezionare per renderlo invisibile.
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); 
    }

    void EnterCombat(Unit *who) {}

    void SpellHit(Unit *caster, const SpellEntry* spell)
    {
        //Ricevuto hit dal lancio testa
        if (spell->Id == SPELL_FLYING_HEAD)
        {
            bHaveBody = false;
            uBodyGUID = ((Creature*)caster)->GetGUID();
            m_creature->RemoveAllAuras();
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(m_creature,SPELL_HEAD_LANDS, true);
            DoCast(m_creature,SPELL_HEAD, true);
            DoCast(m_creature,SPELL_HEAD_SPEAKS, true);
            DoScriptText(SAY_LOST_HEAD, m_creature, 0);
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveFleeing(caster->getVictim());
        }
    }

    void DamageTaken(Unit* done_by,uint32 &damage)
    {
        //Se la testa ha il corpo o è morta non prende danni (In genere i dot rimasti)
        if(bHaveBody || bIsDied)
            damage = 0;

        //Comportamento testa in base alla phase
        switch(uPhase)
        {
            case 0:
                if(((m_creature->GetHealth() - damage)*100)/m_creature->GetMaxHealth() < 67)
                {
                    damage = 0;
                    JoinBody();
                }
                break;
            case 1:
                if(((m_creature->GetHealth() - damage)*100)/m_creature->GetMaxHealth() < 34)
                {
                    damage = 0;
                    JoinBody();
                }
                break;
            case 2:
                if (damage >= m_creature->GetHealth() && !bIsDied)
                {
                    damage = 0;
                    uDieTimer = 2000;
                    bIsDied   = true;
                    HeadDie();
                }
        }
    }

    //Funzione dichiarate esternamente alla struct per poter accedere all'AI dell'horseman.
    void JoinBody();
    void HeadDie();

    void UpdateAI(const uint32 diff)
    {
        //Evento concluso
        if(bEventDone)
            return;

        //Quando la testa muore lanciamo un timer prima di despawnarla
        //in modo da lasciar completare l'animazione di morte.
        if(bIsDied)
        {
            if(uDieTimer < diff)
            {
                if(uBodyGUID)
                {
                    //Allo scadere del tier uccidiamo pure il corpo.
                    Creature *pBody = Unit::GetCreature((*m_creature), uBodyGUID);
                    if(pBody && pBody->isAlive())
                    {
                        pBody->RemoveAurasDueToSpell(SPELL_IMMUNE);
                        pBody->Kill(pBody);
                    }
                }

                bEventDone = true;
                uDieTimer = 2000;
                return;
            }
            else uDieTimer-=diff;
        }

        //Testa staccata dal corpo risata random.
        if(!bHaveBody)
        {
            if(uLaughTimer < diff)
            {
                DoPlaySoundToSet(m_creature, RandomLaugh[rand()%3]);
                DoCast(m_creature, SPELL_HEAD_SPEAKS, true);
                DoTextEmote("laughs",NULL);
                uLaughTimer = rand()%1000 + 10000; 
            }
            else uLaughTimer-=diff;
        }
    }
};

struct TRINITY_DLL_DECL boss_headless_horsemanAI : public ScriptedAI
{
    boss_headless_horsemanAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = NULL;
    }

    ScriptedInstance *pInstance;

    uint64 uHeadGUID;
    uint64 uPlayerGUID;
    uint32 uPhase;
    uint32 uCleaveTimer;
    uint32 uWhirlwindTimer;
    uint32 uRegenTimer;
    uint32 uConflagrationTimer;
    uint32 uSummonPumpkinTimer;
    uint32 uLaughTimer;
    uint32 uSayTimer;
    uint32 uSayCount;
    uint32 uFlyPoint;
    uint32 uDieTimer;
    uint32 uPumpkinBurnTimer;
    bool   bHaveHead;
    bool   bIsHeadInFly;
    bool   bIsHeadDied;
    bool   bIsIntroActive;
    bool   bIsFlyActive;
    bool   bFlyPointReached;
    bool   bPumpkinBurned;
    bool   bHeadSpellCasted;

    void Reset()
    {
        uPlayerGUID         = 0;
        uPhase              = 0;
        uCleaveTimer        = 2000;
        uWhirlwindTimer     = 4000;
        uRegenTimer         = 1000;
        uConflagrationTimer = 15000;
        uSummonPumpkinTimer = 5000;
        uLaughTimer         = 4000;
        uSayTimer           = 3000;
        uSayCount           = 0;
        uFlyPoint           = 0;
        uDieTimer           = 5000;
        uPumpkinBurnTimer   = 6000;
        bHaveHead           = true;
        bIsHeadInFly        = false;
        bIsHeadDied         = false;
        bIsIntroActive      = false;
        bIsFlyActive        = false;
        bFlyPointReached    = false;
        bPumpkinBurned      = false;
        bHeadSpellCasted    = false;

        if(uHeadGUID)
        {
            Creature *pHead = Unit::GetCreature((*m_creature), uHeadGUID);
            if(pHead)
            {
                pHead->setDeathState(JUST_DIED);
                pHead->RemoveCorpse();
            }
        }
        uHeadGUID = 0;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if(pInstance)
            pInstance->SetData(DATA_HORSEMAN_EVENT, NOT_STARTED);
    }

    void EnterCombat(Unit *who)
    {
        if(pInstance)
            pInstance->SetData(DATA_HORSEMAN_EVENT, IN_PROGRESS);
    }

    void MoveInLineOfSight(Unit *who) {}

    void KilledUnit(Unit *plr)
    {
        //Frasi da die alla morte di un player.
        if(bHaveHead)
        {
            DoScriptText(SAY_PLAYER_DEATH, m_creature, 0);
        }
        else
        {
            if(uHeadGUID)
            {
                Creature *pHead = Unit::GetCreature((*m_creature), uHeadGUID);
                if(pHead)
                    DoScriptText(SAY_PLAYER_DEATH, pHead, 0);
            }
        }

        uLaughTimer += 4000;
    }

    void JustDied(Unit* killer)
    {
        m_creature->StopMoving();
        DoScriptText(SAY_DEATH, m_creature, 0);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        //Despawniamo il summon dell'head
        if(uHeadGUID)
        {
            Creature *pHead = Unit::GetCreature((*m_creature), uHeadGUID);
            if(pHead)
            {
                pHead->setDeathState(JUST_DIED);
                pHead->RemoveCorpse();
            }
            pHead = NULL;
        }

        //Fiamme sul corpo
        Creature *pBodyFlame = DoSpawnCreature(HELPER,0,0,0,0,TEMPSUMMON_TIMED_DESPAWN,60000);
        if(pBodyFlame)
            pBodyFlame->CastSpell(pBodyFlame,SPELL_BODY_FLAME,false);

        //Wisp dell'horseman
        Creature *pWisp = DoSpawnCreature(WISP_INVIS,0,0,0,0,TEMPSUMMON_TIMED_DESPAWN,60000);
        if(pWisp)
            ((mob_wisp_invisAI*)pWisp->AI())->SetType(4);

        if(pInstance)
            pInstance->SetData(DATA_HORSEMAN_EVENT, DONE);
    }

    void SpellHit(Unit *caster, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_FLYING_HEAD)
        {
            JoinHead();
        }
    }

    void SpellHitTarget(Unit* unit, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_CONFLAGRATION)
            if (unit->HasAura(SPELL_CONFLAGRATION,0))
                DoScriptText(SAY_PLAYER_DEATH, m_creature, unit);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        //Se il corpo non ha la testa e la testa è ancora in vita il corpo non deve prendere danni.
        if(!bHaveHead && !bIsHeadDied)
        {
            damage = 0;
            return;
        }

        //15.000 Range di sicurezza per evitare una morte accidentale che interrompe l'evento
        if ( (damage+15000) >= m_creature->GetHealth() && bHaveHead)
        {
            damage = 0;

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveAllAuras();
            m_creature->InterruptNonMeleeSpells(false);
            m_creature->SetName("Headless Horseman, Unhorsed");

            //Nessuna Head. Necessita Spawn
            if(!uHeadGUID)
            {
                uHeadGUID = (DoSpawnCreature(HEAD,rand()%6,rand()%6,0,0,TEMPSUMMON_DEAD_DESPAWN,0))->GetGUID();
            }

            Creature* pHead = Unit::GetCreature((*m_creature), uHeadGUID);
            if(pHead)
            {
                //Lanciamo la testa al npc head ed aggiorniamo la sua phase
                DoCast(pHead, SPELL_FLYING_HEAD,true);
                ((mob_headAI*)pHead->AI())->uPhase = uPhase;
            }

            DoCast(m_creature,SPELL_IMMUNE,true);
            DoCast(m_creature,SPELL_BODY_REGEN,true);
            DoCast(m_creature,SPELL_CONFUSE,false);

            bHaveHead       = false;
            uWhirlwindTimer = 5000;
            uRegenTimer     = 1000;
        }
    }

    //Funzione chiamata dal corpo per ricongiungersi alla testa
    void JoinHead()
    {
        m_creature->InterruptNonMeleeSpells(false);
        m_creature->RemoveAllAuras();
        m_creature->SetName("Headless Horseman");
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoCast(m_creature, SPELL_HEAD, true);
        DoScriptText(SAY_REJOINED, m_creature, 0);
        bHaveHead    = true;
        bIsHeadInFly = false;
        uPhase++;

        if(uPhase > 2)
            uPhase = 2;
    }

    //Funzione chiamata per iniziare l'animazione.
    void StartIntro()
    {
        m_creature->SetVisibility(VISIBILITY_OFF);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->AddUnitMovementFlag(MOVEMENTFLAG_FLYING2);
        m_creature->SetSpeed(MOVE_WALK,5.0f,true);
        bIsIntroActive   = true;
        bFlyPointReached = false;
        bIsFlyActive     = false;
        uFlyPoint = 0;
        uSayCount = 0;
    }

    //Funzione di aggiornamento movimento.
    void MovementInform(uint32 type, uint32 i)
    {
        if(type != POINT_MOTION_TYPE || !bIsFlyActive)
            return;

        if(i != uFlyPoint)
            return;

        bFlyPointReached = true;
        switch(uFlyPoint)
        {
            case 0:
                m_creature->SetVisibility(VISIBILITY_ON);
                break;

            case 19:
                m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING2);
                break;

            case 20:
            {
                bIsFlyActive     = false;
                bIsIntroActive   = false;
                bFlyPointReached = false;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                if(uPlayerGUID)
                {
                    Unit *pPlayer = Unit::GetUnit((*m_creature), uPlayerGUID);
                    if (pPlayer)
                        AttackStart(pPlayer);
                }

                m_creature->SetSpeed(MOVE_WALK,2.0f,true);
                m_creature->SetSpeed(MOVE_RUN,2.0f,true);
            }
            break;
        }
        uFlyPoint++;
    }

    void UpdateAI(const uint32 diff)
    {
        //Testa morta. Ogni tanto si bugga il corpo e non muore
        //In tal caso dopo un tot tempo bisogna fare un kill
        if(bIsHeadDied)
        {
            if(uDieTimer < diff)
            {
                if(m_creature && m_creature->isAlive())
                {
                    m_creature->Kill(m_creature);
                }
            }
            else uDieTimer-=diff;
            return;
        }

        //Hack per far apparire l'head al reset.
        if(!bHeadSpellCasted && bHaveHead && m_creature->isAlive())
        {
            DoCast(m_creature, SPELL_HEAD, true);
            bHeadSpellCasted = true;
        }

        //Animazione Iniziale e volo.
        if(bIsIntroActive)
        {
            if(!bIsFlyActive)
            {
                if(uSayTimer < diff)
                {
                    if(uPlayerGUID)
                    {
                        Player *pPlayer = (Player*)Unit::GetUnit((*m_creature), uPlayerGUID);
                        if(!pPlayer || !pPlayer->isAlive() || !pPlayer->GetSession())
                            return;

                        if(uSayCount < 4)
                        {
                            Creature *pSmoke = m_creature->SummonCreature(HELPER,Spawn[1].x+rand()%5,Spawn[1].y+rand()%5,Spawn[1].z,0,TEMPSUMMON_TIMED_DESPAWN,40000);
                            if(pSmoke)
                            {
                                //Effetto Fumo + Terremoto
                                ((mob_wisp_invisAI*)pSmoke->AI())->SetType(3);
                                ((mob_wisp_invisAI*)pSmoke->AI())->DoCast(pSmoke,SPELL_RHYME_BIG);
                            }

                            pPlayer->Say(Text[uSayCount].text,0);
                        }

                        if(uSayCount == 3)
                        {
                            pPlayer->HandleEmoteCommand(ANIM_EMOTE_SHOUT);
                            bIsFlyActive     = true;
                            bFlyPointReached = true;
                        }
                    }

                    uSayTimer = 3000;
                    uSayCount++;
                }
                else uSayTimer-=diff;
                return;
            }

            //Horseman ha iniziato il volo aggiorniamo i punti di volta in volta.
            if(bFlyPointReached)
            {
                bFlyPointReached = false;
                m_creature->GetMotionMaster()->Clear(false);
                m_creature->GetMotionMaster()->MovePoint(uFlyPoint,FlightPoint[uFlyPoint].x,FlightPoint[uFlyPoint].y,FlightPoint[uFlyPoint].z);
            }
            return;
        }

        //Effetto grafico per far scoppiare la zucca all'arrivo dell'Headless
        if(!bPumpkinBurned)
        {
            if(uPumpkinBurnTimer < diff)
            {
                Creature *pPumpkinFlame = m_creature->SummonCreature(HELPER,Spawn[0].x,Spawn[0].y,Spawn[0].z,0,TEMPSUMMON_TIMED_DESPAWN,120000);
                if(pPumpkinFlame)
                    ((mob_wisp_invisAI*)pPumpkinFlame->AI())->SetType(2);

                bPumpkinBurned = true;
            }
            else uPumpkinBurnTimer-=diff;
        }

        //Sezione Encounter iniziato
        if(!UpdateVictim())
            return;

        //L'Headless ha la testa.
        if(bHaveHead)
        {
            //Il cleave viene fatto in tutte le fasi.
            if(uCleaveTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                uCleaveTimer = rand()%2000 + 2000;
            }
            else uCleaveTimer-=diff;

            //In Fase 1 Oltre al cleave lancia il conflagrate
            if(uPhase == 1)
            {
                if(uConflagrationTimer < diff)
                {

                    Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1);
                    if(!pTarget)
                        pTarget = m_creature->getVictim();

                    if(pTarget)
                    {
                        int32 damage;
                        damage = pTarget->GetMaxHealth()/10;
                        m_creature->CastCustomSpell(pTarget,SPELL_CONFLAGRATION,&damage,NULL,NULL,false);
                    }

                    uConflagrationTimer = rand()%5000 + 10000;
                }
                else uConflagrationTimer-=diff;
            }

            //In fase 2 oltre al cleave ci sta il summon delle zucche
            if(uPhase == 2)
            {
                if(uSummonPumpkinTimer < diff)
                {
                    DoCast(m_creature,SPELL_SUMMON_PUMPKIN);
                    DoScriptText(SAY_SPROUTING_PUMPKINS, m_creature, 0);
                    uSummonPumpkinTimer = rand()%5000 + 20000;
                }
                else uSummonPumpkinTimer-=diff;
            }

            //Risata Random
            if(uLaughTimer < diff)
            {
                DoTextEmote("laughs",NULL);
                DoPlaySoundToSet(m_creature, RandomLaugh[rand()%3]);
                uLaughTimer = rand()%2000 + 10000;
            }
            else uLaughTimer -= diff;

            //Colpo Bianco
            DoMeleeAttackIfReady();
        }
        //Sezione Encounter senza testa
        else
        {
            //Rigenerazione
            if(uRegenTimer < diff)
            {
                if (m_creature->GetHealth()/m_creature->GetMaxHealth() == 1)
                {
                    if(uHeadGUID)
                    {
                        Creature *pHead = Unit::GetCreature((*m_creature), uHeadGUID);
                        if(pHead && !bIsHeadInFly)
                        {
                            bIsHeadInFly = true;
                            ((mob_headAI*)pHead->AI())->JoinBody();
                        }
                    }
                }
                uRegenTimer = 1000;
            }
            else uRegenTimer-=diff;

            //Random Whirlwind durante disorienting
            if(uWhirlwindTimer < diff)
            {
                if(rand()%2)
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_CONFUSE);
                    DoCast(m_creature, SPELL_WHIRLWIND, true);
                    DoCast(m_creature, SPELL_CONFUSE);
                }
                else
                    m_creature->RemoveAurasDueToSpell(SPELL_WHIRLWIND);

                uWhirlwindTimer = 5000;
            }
            else uWhirlwindTimer-=diff;
        }
    }
};

//Morte dell'Head
void mob_headAI::HeadDie()
{
    //Impostiamo la testa non selezionabile in modo da renderla invisibile
    //e far vedere solo l'animazione della morte
    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    m_creature->RemoveAllAuras();
    if(uBodyGUID)
    {
        Creature *pBody = Unit::GetCreature((*m_creature), uBodyGUID);
        if(pBody)
        {
            //Leviamo la spell di immunità potrebbe dare problemi col kill del corpo
            pBody->RemoveAurasDueToSpell(SPELL_IMMUNE);
            ((boss_headless_horsemanAI*)pBody->AI())->bIsHeadDied = true;
        }
    }

    //Animazione morte della testa
    DoCast(m_creature,SPELL_HEAD_IS_DEAD, true);
}

//Riunione tra testa e corpo
void mob_headAI::JoinBody()
{
    if(bHaveBody)
        return;

    if(uBodyGUID)
    {
        Creature *pBody = Unit::GetCreature((*m_creature), uBodyGUID);
        if(pBody && pBody->isAlive())
        {
            bHaveBody = true;

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveAllAuras();
            pBody->RemoveAurasDueToSpell(SPELL_IMMUNE);
            DoCast(pBody,SPELL_FLYING_HEAD, true);
            m_creature->GetMotionMaster()->Clear(false);
        }
    }
}

struct TRINITY_DLL_DECL mob_pulsing_pumpkinAI : public ScriptedAI
{
    mob_pulsing_pumpkinAI(Creature *c) : ScriptedAI(c) {}

    bool sprouted;
    uint64 debuffGUID;

    void Reset()
    {
        float x, y, z;
        m_creature->GetPosition(x, y, z);   //this visual aura some under ground
        m_creature->Relocate(x,y,z + 0.35f);
        Despawn();
        Creature *debuff = DoSpawnCreature(HELPER,0,0,0,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,14500);
        if(debuff)
        {
            debuff->SetDisplayId(m_creature->GetDisplayId());
            debuff->CastSpell(debuff,SPELL_PUMPKIN_AURA_GREEN,false);
            ((mob_wisp_invisAI*)debuff->AI())->SetType(1);
            debuffGUID = debuff->GetGUID();
        }
        sprouted = false;
        DoCast(m_creature,SPELL_PUMPKIN_AURA,true);
        DoCast(m_creature,SPELL_SPROUTING);
        m_creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_DISABLE_ROTATE);
    }

    void EnterCombat(Unit *who){}

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_SPROUTING)
        {
            sprouted = true;
            m_creature->RemoveAllAuras();
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_DISABLE_ROTATE);
            DoCast(m_creature,SPELL_SPROUT_BODY,true);
            m_creature->UpdateEntry(PUMPKIN_FIEND);
            DoStartMovement(m_creature->getVictim());
        }
    }

    void Despawn()
    {
        if (!debuffGUID) return;
        Unit *debuff = Unit::GetUnit((*m_creature),debuffGUID);
        if(debuff)
            debuff->SetVisibility(VISIBILITY_OFF);
            debuffGUID = 0;
    }

    void JustDied(Unit *killer) {if(!sprouted) Despawn();}

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || !who->isTargetableForAttack() || !m_creature->IsHostileTo(who) || m_creature->getVictim())
            return;

        m_creature->AddThreat(who,0.0f);
        if(sprouted)
            DoStartMovement(who);
    }

    void UpdateAI(const uint32 diff)
    {
        if (sprouted)
            if (UpdateVictim())
                DoMeleeAttackIfReady();
    }
};

bool GOHello_go_loosely_turned_soil(Player *plr, GameObject* soil)
{
        if(soil->isActiveObject() || plr->getLevel() < 64)
            return false;

        plr->AreaExploredOrEventHappens(11405);
        Creature *horseman = soil->SummonCreature(HH_MOUNTED,FlightPoint[20].x,FlightPoint[20].y,FlightPoint[20].z,0,TEMPSUMMON_MANUAL_DESPAWN,0);
        horseman->setActive(true);
        if(horseman)
        {
            soil->setActive(true);
            ((boss_headless_horsemanAI*)horseman->AI())->uPlayerGUID = plr->GetGUID();
            ((boss_headless_horsemanAI*)horseman->AI())->StartIntro();
        }

    return true;
}

bool GOReward_go_loosely_turned_soil(Player *plr, GameObject* soil, Quest const* quest, uint32 iot)
{
    if(quest->GetQuestId() == 11392 || quest->GetQuestId() == 11405)
    {
        Creature *pHorseman = soil->SummonCreature(HH_MOUNTED,FlightPoint[20].x,FlightPoint[20].y,FlightPoint[20].z,0,TEMPSUMMON_MANUAL_DESPAWN,0);
        if(pHorseman)
        {
            ((boss_headless_horsemanAI*)pHorseman->AI())->uPlayerGUID = plr->GetGUID();
            ((boss_headless_horsemanAI*)pHorseman->AI())->StartIntro();
            soil->SetUInt32Value(GAMEOBJECT_FACTION,14);
        }
    }
    return true;
}

CreatureAI* GetAI_mob_head(Creature *_Creature)
{
    return new mob_headAI (_Creature);
}

CreatureAI* GetAI_boss_headless_horseman(Creature *_Creature)
{
    return new boss_headless_horsemanAI (_Creature);
}

CreatureAI* GetAI_mob_pulsing_pumpkin(Creature *_Creature)
{
    return new mob_pulsing_pumpkinAI (_Creature);
}

CreatureAI* GetAI_mob_wisp_invis(Creature *_Creature)
{
    return new mob_wisp_invisAI (_Creature);
}

void AddSC_boss_headless_horseman()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_headless_horseman";
    newscript->GetAI = &GetAI_boss_headless_horseman;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_head";
    newscript->GetAI = &GetAI_mob_head;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_pulsing_pumpkin";
    newscript->GetAI = &GetAI_mob_pulsing_pumpkin;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_wisp_invis";
    newscript->GetAI = &GetAI_mob_wisp_invis;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_loosely_turned_soil";
    newscript->pGOChooseReward = &GOReward_go_loosely_turned_soil;
    newscript->RegisterSelf();
}

