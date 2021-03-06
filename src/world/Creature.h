/*
 * FrostwingCore <https://gitlab.com/cronicx321/AscEmu>
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 * Copyright (C) 2005-2007 Ascent Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WOWSERVER_CREATURE_H
#define _WOWSERVER_CREATURE_H

#include "Map.h"

class CreatureTemplate;
class GossipScript;

#define MAX_CREATURE_INV_ITEMS 150
#define MAX_CREATURE_LOOT 8
#define MAX_CREATURE_PROTO_SPELLS 8
#define MAX_PET_SPELL 4
#define VENDOR_ITEMS_UPDATE_TIME 3600000

enum creatureguardtype
{
    GUARDTYPE_NONE,
    GUARDTYPE_CITY,
    GUARDTYPE_NEUTRAL
};

struct CreatureItem
{
    uint32 itemid;
    uint32 amount;                              /// stack amount.
    uint32 available_amount;
    uint32 max_amount;
    uint32 incrtime;
    ItemExtendedCostEntry* extended_cost;
};

enum CreatureAISpellFlags
{
    CREATURE_AI_FLAG_NONE               = 0x00,
    CREATURE_AI_FLAG_RANDOMCAST         = 0x01,
    CREATURE_AI_FLAG_CASTOUTOFCOMBAT    = 0x02,
    CREATURE_AI_FLAG_PLAYERGCD          = 0x04
};

enum VendorRestrictionFlag
{
    RESTRICTION_CHECK_ALL           = 0x00,     /// this checks for all possible values in table
    RESTRICTION_CHECK_MOUNT_VENDOR  = 0x01      /// this one check for race, if race dont match checks for reputation
};

struct NpcMonsterSay
{
    float Chance;
    uint32 Language;
    uint32 Type;
    const char* MonsterName;

    uint32 TextCount;
    const char** Texts;
};

enum MONSTER_SAY_EVENTS
{
    MONSTER_SAY_EVENT_ENTER_COMBAT      = 0,
    MONSTER_SAY_EVENT_RANDOM_WAYPOINT   = 1,
    MONSTER_SAY_EVENT_CALL_HELP         = 2,
    MONSTER_SAY_EVENT_ON_COMBAT_STOP    = 3,
    MONSTER_SAY_EVENT_ON_DAMAGE_TAKEN   = 4,
    MONSTER_SAY_EVENT_ON_DIED           = 5,
    NUM_MONSTER_SAY_EVENTS,
};

SERVER_DECL bool Rand(float chance);
SERVER_DECL bool Rand(uint32 chance);
SERVER_DECL bool Rand(int32 chance);

#pragma pack(push,1)
struct CreatureInfo
{
    uint32 Id;
    char* Name;
    char* SubName;
    char* info_str;
    uint32 Flags1;
    uint32 Type;
    uint32 Family;
    uint32 Rank;
    uint32 Encounter;
    uint32 killcredit[2];
    uint32 Male_DisplayID;
    uint32 Female_DisplayID;
    uint32 Male_DisplayID2;
    uint32 Female_DisplayID2;
    float unkfloat1;
    float unkfloat2;
    uint8  Leader;
    uint32 QuestItems[6];
    uint32 waypointid;

    std::string lowercase_name;
    NpcMonsterSay* MonsterSay[NUM_MONSTER_SAY_EVENTS];

    uint8 GenerateModelId(uint32* des)
    {
        uint32 models[] = { Male_DisplayID, Male_DisplayID2, Female_DisplayID, Female_DisplayID2 };
        if (!models[0] && !models[1] && !models[2] && !models[3])
        {
            // All models are invalid.
            Log.Notice("CreatureSpawn", "All model IDs are invalid for creature %u", Id);
            return 0;
        }

        while(true)
        {
            uint32 res = RandomUInt(3);
            if (models[res])
            {
                *des = models[res];
                return res < 2 ? 0 : 1;
            }
        }
    }
};

struct CreatureProto
{
    uint32 Id;
    uint32 MinLevel;
    uint32 MaxLevel;
    uint32 Faction;
    uint32 MinHealth;
    uint32 MaxHealth;
    uint32 Mana;
    float Scale;
    uint32 NPCFLags;
    uint32 AttackTime;
    uint32 AttackType;
    float MinDamage;
    float MaxDamage;
    uint32 CanRanged;
    uint32 RangedAttackTime;
    float RangedMinDamage;
    float RangedMaxDamage;
    uint32 RespawnTime;
    uint32 Resistances[SCHOOL_COUNT];
    float CombatReach;
    float BoundingRadius;
    char* aura_string;
    uint32 boss;
    uint32 money;
    uint32 invisibility_type;
    float walk_speed;       /// base movement
    float run_speed;        /// most of the time mobs use this
    float fly_speed;
    uint32 extra_a9_flags;
    uint32 AISpells[ MAX_CREATURE_PROTO_SPELLS ];
    uint32 AISpellsFlags;
    uint32 modImmunities;
    uint32 isTrainingDummy;
    uint32 guardtype;
    uint32 summonguard;
    uint32 spelldataid;
    uint32 vehicleid;
    uint32 rooted;

    /* AI Stuff */
    bool m_canRangedAttack;
    bool m_canFlee;
    float m_fleeHealth;
    uint32 m_fleeDuration;
    bool m_canCallForHelp;
    float m_callForHelpHealth;

    std::set<uint32> start_auras;
    std::vector<uint32> castable_spells;
    std::list<AI_Spell*> spells;
};

struct CreatureDifficulty
{
	uint32 entry;
	uint32 difficulty_1;
	uint32 difficulty_2;
	uint32 difficulty_3;
	uint32 difficulty_4;
};

struct VendorRestrictionEntry
{
    uint32 entry;
    int32 racemask;
    int32 classmask;
    uint32 reqrepfaction;
    uint32 reqrepvalue;
    uint32 canbuyattextid;
    uint32 cannotbuyattextid;
    uint32 flags;
};

struct TotemDisplayIdEntry
{
    uint32 DisplayId;       /// male displayid in creature_names
    uint32 DraeneiId;       /// Totem displayid for Draenei
    uint32 TrollId;         /// Totem displayid for Troll
    uint32 OrcId;           /// Totem displayid for Orc
};

#pragma pack(pop)

struct Formation
{
    uint32 fol;
    float ang;
    float dist;
};

enum UNIT_TYPE
{
    UNIT_TYPE_NONE              = 0,
    UNIT_TYPE_BEAST             = 1,
    UNIT_TYPE_DRAGONKIN         = 2,
    UNIT_TYPE_DEMON             = 3,
    UNIT_TYPE_ELEMENTAL         = 4,
    UNIT_TYPE_GIANT             = 5,
    UNIT_TYPE_UNDEAD            = 6,
    UNIT_TYPE_HUMANOID          = 7,
    UNIT_TYPE_CRITTER           = 8,
    UNIT_TYPE_MECHANICAL        = 9,
    UNIT_TYPE_MISC              = 10,
    UNIT_TYPE_TOTEM             = 11,
    UNIT_TYPE_NONCOMBAT_PET     = 12,
    UNIT_TYPE_GAS_CLOUD         = 13,
    UNIT_TYPE_NUM               = 14,
};

enum NPCFlags
{
    UNIT_NPC_FLAG_NONE                  = 0x00000000,
    UNIT_NPC_FLAG_GOSSIP                = 0x00000001,
    UNIT_NPC_FLAG_QUESTGIVER            = 0x00000002,
    UNIT_NPC_FLAG_UNK1                  = 0x00000004,
    UNIT_NPC_FLAG_UNK2                  = 0x00000008,
    UNIT_NPC_FLAG_TRAINER               = 0x00000010,
    UNIT_NPC_FLAG_TRAINER_CLASS         = 0x00000020,
    UNIT_NPC_FLAG_TRAINER_PROF          = 0x00000040,
    UNIT_NPC_FLAG_VENDOR                = 0x00000080,
    UNIT_NPC_FLAG_VENDOR_AMMO           = 0x00000100,
    UNIT_NPC_FLAG_VENDOR_FOOD           = 0x00000200,
    UNIT_NPC_FLAG_VENDOR_POISON         = 0x00000400,
    UNIT_NPC_FLAG_VENDOR_REAGENT        = 0x00000800,
    UNIT_NPC_FLAG_ARMORER               = 0x00001000,
    UNIT_NPC_FLAG_TAXIVENDOR            = 0x00002000,
    UNIT_NPC_FLAG_SPIRITHEALER          = 0x00004000,
    UNIT_NPC_FLAG_SPIRITGUIDE           = 0x00008000,
    UNIT_NPC_FLAG_INNKEEPER             = 0x00010000,
    UNIT_NPC_FLAG_BANKER                = 0x00020000,
    UNIT_NPC_FLAG_ARENACHARTER          = 0x00040000,
    UNIT_NPC_FLAG_TABARDCHANGER         = 0x00080000,
    UNIT_NPC_FLAG_BATTLEFIELDPERSON     = 0x00100000,
    UNIT_NPC_FLAG_AUCTIONEER            = 0x00200000,
    UNIT_NPC_FLAG_STABLEMASTER          = 0x00400000,
    UNIT_NPC_FLAG_GUILD_BANK            = 0x00800000,
    UNIT_NPC_FLAG_SPELLCLICK            = 0x01000000,
    UNIT_NPC_FLAG_PLAYER_VEHICLE        = 0x02000000,
    UNIT_NPC_FLAG_MAILBOX               = 0x04000000
};

enum CreatureFlag1
{
    CREATURE_FLAG1_TAMEABLE             = 0x00000001,       // creature is tameable by hunter
    CREATURE_FLAG1_GHOST                = 0x00000002,       // creature are also visible for dead players.
    CREATURE_FLAG1_BOSS                 = 0x00000004,       // creature is a boss "??"
    CREATURE_FLAG1_UNK3                 = 0x00000008,
    CREATURE_FLAG1_UNK4                 = 0x00000010,
    CREATURE_FLAG1_UNK5                 = 0x00000020,
    CREATURE_FLAG1_UNK6                 = 0x00000040,
    CREATURE_FLAG1S_DEAD_INTERACT       = 0x00000080,       // player can interact with the creature while creature is dead.
    CREATURE_FLAG1_HERBLOOT             = 0x00000100,       // lootable by herbalist
    CREATURE_FLAG1_MININGLOOT           = 0x00000200,       // lootable by miner
    CREATURE_FLAG1_DONT_LOG_DEATH       = 0x00000400,       // death event will not show up in combat log
    CREATURE_FLAG1_FIGHT_MOUNTED        = 0x00000800,       // creature keeps mounted by entering combat
    CREATURE_FLAG1_AID_PLAYERS          = 0x00001000,
    CREATURE_FLAG1_UNK13                = 0x00002000,
    CREATURE_FLAG1_UNK14                = 0x00004000,
    CREATURE_FLAG1_ENGINEERLOOT         = 0x00008000,
    CREATURE_FLAG1_EXOTIC               = 0x00010000,
    CREATURE_FLAG1_UNK17                = 0x00020000,
    CREATURE_FLAG1_UNK18                = 0x00040000,
    CREATURE_FLAG1S_PROJECT_COLL        = 0x00080000,
    CREATURE_FLAG1_UNK20                = 0x00100000,
    CREATURE_FLAG1_UNK21                = 0x00200000,
    CREATURE_FLAG1_UNK22                = 0x00400000,
    CREATURE_FLAG1_UNK23                = 0x00800000,
    CREATURE_FLAG1_UNK24                = 0x01000000,
    CREATURE_FLAG1_UNK25                = 0x02000000,
    CREATURE_FLAG1_PARTY_MEMBER         = 0x04000000,
    CREATURE_FLAG1_UNK27                = 0x08000000,
    CREATURE_FLAG1_UNK28                = 0x10000000,
    CREATURE_FLAG1_UNK29                = 0x20000000,
    CREATURE_FLAG1_UNK30                = 0x40000000,
    CREATURE_FLAG1_UNK31                = 0x80000000
};

enum FAMILY
{
    FAMILY_WOLF             = 1,
    FAMILY_CAT              = 2,
    FAMILY_SPIDER           = 3,
    FAMILY_BEAR             = 4,
    FAMILY_BOAR             = 5,
    FAMILY_CROCOLISK        = 6,
    FAMILY_CARRION_BIRD     = 7,
    FAMILY_CRAB             = 8,
    FAMILY_GORILLA          = 9,
    FAMILY_RAPTOR           = 11,
    FAMILY_TALLSTRIDER      = 12,
    FAMILY_FELHUNTER        = 15,
    FAMILY_VOIDWALKER       = 16,
    FAMILY_SUCCUBUS         = 17,
    FAMILY_DOOMGUARD        = 19,
    FAMILY_SCORPID          = 20,
    FAMILY_TURTLE           = 21,
    FAMILY_IMP              = 23,
    FAMILY_BAT              = 24,
    FAMILY_HYENA            = 25,
    FAMILY_OWL              = 26,
    FAMILY_WIND_SERPENT     = 27,
    FAMILY_REMOTE_CONTROL   = 28,
    FAMILY_FELGUARD         = 29,
    FAMILY_DRAGONHAWK       = 30,
    FAMILY_RAVAGER          = 31,
    FAMILY_WARP_STALKER     = 32,
    FAMILY_SPOREBAT         = 33,
    FAMILY_NETHER_RAY       = 34,
    FAMILY_SERPENT          = 35,
    FAMILY_MOTH             = 37,
    FAMILY_CHIMAERA         = 38,
    FAMILY_DEVILSAUR        = 39,
    FAMILY_GHOUL            = 40,   /// DK's minion
    FAMILY_SILITHID         = 41,
    FAMILY_WORM             = 42,
    FAMILY_RHINO            = 43,
    FAMILY_WASP             = 44,
    FAMILY_CORE_HOUND       = 45,
    FAMILY_SPIRIT_BEAST     = 46
};

enum ELITE
{
    ELITE_NORMAL    = 0,
    ELITE_ELITE     = 1,
    ELITE_RAREELITE = 2,
    ELITE_WORLDBOSS = 3,
    ELITE_RARE      = 4
    // ELITE_UNKNOWN = 5
};
enum TIME_REMOVE_CORPSE
{
    TIME_CREATURE_REMOVE_CORPSE     = 180000,
    TIME_CREATURE_REMOVE_RARECORPSE = 180000 * 3,
    TIME_CREATURE_REMOVE_BOSSCORPSE = 180000 * 5,
};

// THIS IS NOT SAME AS DEATH STATE IN Unit.h
enum CREATURE_DEATH_STATE
{
    CREATURE_STATE_ALIVE        = 0,    /// no special death state
    CREATURE_STATE_APPEAR_DEAD  = 1,    /// these creatures are actually alive but appears as dead for client
    CREATURE_STATE_DEAD         = 2     /// these creatures are dead
};

enum CREATURE_TYPE
{
    CREATURE_TYPE_NONE      = 0,
    CREATURE_TYPE_GUARDIAN  = 1
};

struct PetSpellCooldown
{
    uint32 spellId;
    int32 cooldown;
};

class CreatureAIScript;
class GossipScript;
class AuctionHouse;
struct Trainer;
#define CALL_SCRIPT_EVENT(obj, func) if (obj->IsInWorld() && obj->IsCreature() && TO<Creature*>(obj)->GetScript() != NULL) TO<Creature*>(obj)->GetScript()->func


uint8 get_byte(uint32 buffer, uint32 index);

///////////////////
/// Creature object

class SERVER_DECL Creature : public Unit
{
    public:

        Creature(uint64 guid);
        virtual ~Creature();

        GameEvent* mEvent = nullptr;
        
        /// For derived subclasses of Creature
        bool IsVehicle()
        {
            if (proto->vehicleid != 0)
                return true;
            else
                return false;
        }
        
        void AddVehicleComponent(uint32 creature_entry, uint32 vehicleid);
        void RemoveVehicleComponent();

        bool Load(CreatureSpawn* spawn, uint32 mode, MapInfo* info);
        void Load(CreatureProto* proto_, float x, float y, float z, float o = 0);

		//Custom
		void InstanceCreatureAwardEmblem(Creature* u);
		void InstanceCreatureAwardAchievement(Creature* u, uint32 achid);
		Player* GetPlayersInInstance(Creature* u);

        void AddToWorld();
        void AddToWorld(MapMgr* pMapMgr);
        void RemoveFromWorld(bool addrespawnevent, bool free_guid);
        void RemoveFromWorld(bool free_guid);

        void PrepareForRemove();    /// remove auras, guardians, scripts

        /// Creation
        void Create(const char* creature_name, uint32 mapid, float x, float y, float z, float ang);
        void CreateWayPoint(uint32 WayPointID, uint32 mapid, float x, float y, float z, float ang);

        /// Updates
        void Update(unsigned long time_passed);

        /// Creature inventory
        ARCEMU_INLINE uint32 GetItemIdBySlot(uint32 slot) { return m_SellItems->at(slot).itemid; }
        ARCEMU_INLINE uint32 GetItemAmountBySlot(uint32 slot) { return m_SellItems->at(slot).amount; }

        ARCEMU_INLINE bool HasItems() { return ((m_SellItems != NULL) ? true : false); }
        ARCEMU_INLINE CreatureProto* GetProto() { return proto; }

        bool IsPvPFlagged();
        void SetPvPFlag();
        void RemovePvPFlag();

        bool IsFFAPvPFlagged();
        void SetFFAPvPFlag();
        void RemoveFFAPvPFlag();

		bool _isHarpoonAttackable = false;
		bool IsKilledByScript = false;
		void SetRoot();
		void UnsetRoot();

        bool IsSanctuaryFlagged();
        void SetSanctuaryFlag();
        void RemoveSanctuaryFlag();

        void SetSpeeds(uint8 type, float speed);

        int32 GetSlotByItemId(uint32 itemid)
        {
            uint32 slot = 0;
            for (std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
            {
                if (itr->itemid == itemid)
                    return slot;
                else
                    ++slot;
            }
            return -1;
        }

        uint32 GetItemAmountByItemId(uint32 itemid)
        {
            for (std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
            {
                if (itr->itemid == itemid)
                    return ((itr->amount < 1) ? 1 : itr->amount);
            }
            return 0;
        }

        ARCEMU_INLINE void GetSellItemBySlot(uint32 slot, CreatureItem & ci)
        {
            ci = m_SellItems->at(slot);
        }

        void GetSellItemByItemId(uint32 itemid, CreatureItem & ci)
        {
            for (std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
            {
                if (itr->itemid == itemid)
                {
                    ci = (*itr);
                    return;
                }
            }
            ci.amount = 0;
            ci.max_amount = 0;
            ci.available_amount = 0;
            ci.incrtime = 0;
            ci.itemid = 0;
        }

        ItemExtendedCostEntry* GetItemExtendedCostByItemId(uint32 itemid)
        {
            for (std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
            {
                if (itr->itemid == itemid)
                    return itr->extended_cost;
            }
            return NULL;
        }

        ARCEMU_INLINE std::vector<CreatureItem>::iterator GetSellItemBegin() { return m_SellItems->begin(); }
        ARCEMU_INLINE std::vector<CreatureItem>::iterator GetSellItemEnd()   { return m_SellItems->end(); }
        ARCEMU_INLINE size_t GetSellItemCount() { return m_SellItems->size(); }
        void RemoveVendorItem(uint32 itemid)
        {
            for (std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
            {
                if (itr->itemid == itemid)
                {
                    m_SellItems->erase(itr);
                    return;
                }
            }
        }
        void AddVendorItem(uint32 itemid, uint32 amount, ItemExtendedCostEntry* ec);
        void ModAvItemAmount(uint32 itemid, uint32 value);
        void UpdateItemAmount(uint32 itemid);
        /// Quests
        void _LoadQuests();
        bool HasQuests() { return m_quests != NULL; };
        bool HasQuest(uint32 id, uint32 type)
        {
            if (!m_quests) return false;
            for (std::list<QuestRelation*>::iterator itr = m_quests->begin(); itr != m_quests->end(); ++itr)
            {
                if ((*itr)->qst->id == id && (*itr)->type & type)
                    return true;
            }
            return false;
        }
        void AddQuest(QuestRelation* Q);
        void DeleteQuest(QuestRelation* Q);
        Quest* FindQuest(uint32 quest_id, uint8 quest_relation);
        uint16 GetQuestRelation(uint32 quest_id);
        uint32 NumOfQuests();
        list<QuestRelation*>::iterator QuestsBegin() { return m_quests->begin(); };
        list<QuestRelation*>::iterator QuestsEnd() { return m_quests->end(); };
        void SetQuestList(std::list<QuestRelation*>* qst_lst) { m_quests = qst_lst; };

        ARCEMU_INLINE uint32 isVendor()         const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR); }
        ARCEMU_INLINE uint32 isTrainer()        const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER); }
        ARCEMU_INLINE uint32 isClass()          const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER_CLASS); }
        ARCEMU_INLINE uint32 isProf()           const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER_PROF); }
        ARCEMU_INLINE uint32 isQuestGiver()     const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER); }
        ARCEMU_INLINE uint32 isGossip()         const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); }
        ARCEMU_INLINE uint32 isTaxi()           const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TAXIVENDOR); }
        ARCEMU_INLINE uint32 isCharterGiver()   const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_ARENACHARTER); }
        ARCEMU_INLINE uint32 isGuildBank()      const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GUILD_BANK); }
        ARCEMU_INLINE uint32 isBattleMaster()   const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_BATTLEFIELDPERSON); }
        ARCEMU_INLINE uint32 isBanker()         const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_BANKER); }
        ARCEMU_INLINE uint32 isInnkeeper()      const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_INNKEEPER); }
        ARCEMU_INLINE uint32 isSpiritHealer()   const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER); }
        ARCEMU_INLINE uint32 isTabardDesigner() const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TABARDCHANGER); }
        ARCEMU_INLINE uint32 isAuctioner()      const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER); }
        ARCEMU_INLINE uint32 isStableMaster()   const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_STABLEMASTER); }
        ARCEMU_INLINE uint32 isArmorer()        const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_ARMORER); }

        ARCEMU_INLINE uint32 GetHealthFromSpell() { return m_healthfromspell; }
        void SetHealthFromSpell(uint32 value) { m_healthfromspell = value;}

        int32 m_speedFromHaste;
        int32 FlatResistanceMod[SCHOOL_COUNT];
        int32 BaseResistanceModPct[SCHOOL_COUNT];
        int32 ResistanceModPct[SCHOOL_COUNT];

        int32 FlatStatMod[5];
        int32 StatModPct[5];
        int32 TotalStatModPct[5];

        int32 ModDamageDone[SCHOOL_COUNT];
        float ModDamageDonePct[SCHOOL_COUNT];
        void CalcResistance(uint32 type);
        void CalcStat(uint32 type);

        bool m_canRegenerateHP;
        void RegenerateHealth();
        void RegenerateMana();
        int BaseAttackType;

        bool CanSee(Unit* obj)      /// Invisibility & Stealth Detection - Partha
        {
            if (!obj)
                return false;

            if (obj->m_invisible)    /// Invisibility - Detection of Players and Units
            {
                if (obj->getDeathState() == CORPSE)  /// can't see dead players' spirits
                    return false;

                if (m_invisDetect[obj->m_invisFlag] < 1)    /// can't see invisible without proper detection
                    return false;
            }

            if (obj->IsStealth())       /// Stealth Detection ( I Hate Rogues :P )
            {
                if (isInFront(obj))     /// stealthed player is in front of creature
                {
                    // Detection Range = 5yds + (Detection Skill - Stealth Skill)/5
                        detectRange = 5.0f + getLevel() + (0.2f * (float)(GetStealthDetectBonus()) - obj->GetStealthLevel());

                    if (detectRange < 1.0f) detectRange = 1.0f;     /// Minimum Detection Range = 1yd
                }
                else /// stealthed player is behind creature
                {
                    if (GetStealthDetectBonus() > 1000) return true;    /// immune to stealth
                    else detectRange = 0.0f;
                }

                detectRange += GetBoundingRadius();         /// adjust range for size of creature
                detectRange += obj->GetBoundingRadius();    /// adjust range for size of stealthed player

                if (GetDistance2dSq(obj) > detectRange * detectRange)
                    return false;
            }

            return true;
        }

        /// Make this unit face another unit
        bool setInFront(Unit* target);

        /// Looting
        void generateLoot();
        bool HasLootForPlayer(Player* plr);

        bool Skinned;
        uint32 GetRequiredLootSkill();

        // Misc
        ARCEMU_INLINE void setEmoteState(uint8 emote) { m_emoteState = emote; };
        ARCEMU_INLINE uint32 GetSQL_id() { return spawnid; };

        virtual void setDeathState(DeathState s);

        uint32 GetOldEmote() { return m_oldEmote; }

        void SendChatMessage(uint8 type, uint32 lang, const char* msg, uint32 delay = 0);
        void SendScriptTextChatMessage(uint32 textid);
        void SendTimedScriptTextChatMessage(uint32 textid, uint32 delay = 0);
        void SendChatMessageToPlayer(uint8 type, uint32 lang, const char* msg, Player* plr);

        // Serialization
        void SaveToDB();
        void LoadAIAgents(CreatureTemplate* t);
        void DeleteFromDB();

        void OnJustDied();
        void OnRemoveCorpse();
        void OnRespawn(MapMgr* m);

        void BuildPetSpellList(WorldPacket & data);

    protected:
        virtual void SafeDelete();      /// use DeleteMe() instead of SafeDelete() to avoid crashes like InWorld Creatures deleted.
    public:
        // In Range
        void AddInRangeObject(Object* pObj);
        void OnRemoveInRangeObject(Object* pObj);
        void ClearInRangeSet();

        // Demon
        void EnslaveExpire();

        // Pet
        void UpdatePet();
        uint32 GetEnslaveCount() { return m_enslaveCount; }
        void SetEnslaveCount(uint32 count) { m_enslaveCount = count; }
        uint32 GetEnslaveSpell() { return m_enslaveSpell; }
        void SetEnslaveSpell(uint32 spellId) { m_enslaveSpell = spellId; }
        bool RemoveEnslave();

        Object* GetPlayerOwner();

        Group* GetGroup();

        int32 GetDamageDoneMod(uint32 school)
        {
            if (school >= SCHOOL_COUNT)
                return 0;

            return ModDamageDone[ school ];
        }

        float GetDamageDonePctMod(uint32 school)
        {
            if (school >= SCHOOL_COUNT)
                return 0;

            return ModDamageDonePct[ school ];
        }

        ARCEMU_INLINE bool IsPickPocketed() { return m_PickPocketed; }
        ARCEMU_INLINE void SetPickPocketed(bool val = true) { m_PickPocketed = val; }

        ARCEMU_INLINE CreatureAIScript* GetScript() { return _myScriptClass; }
        void LoadScript();

        void CallScriptUpdate();

        ARCEMU_INLINE CreatureInfo* GetCreatureInfo() { return creature_info; }
        ARCEMU_INLINE void SetCreatureInfo(CreatureInfo* ci) { creature_info = ci; }
        void SetCreatureProto(CreatureProto* cp) { proto = cp; }

        ARCEMU_INLINE Trainer* GetTrainer() { return mTrainer; }
        void RegenerateFocus();

        CreatureFamilyEntry* myFamily;

        ARCEMU_INLINE bool IsExotic()
        {
            if ((GetCreatureInfo()->Flags1 & CREATURE_FLAG1_EXOTIC) != 0)
                return true;

            return false;
        }


        bool isCritter();
        bool isTrainingDummy()
        {

            if (GetProto()->isTrainingDummy)
                return true;
            else
                return false;
        }

        void FormationLinkUp(uint32 SqlId);
        void ChannelLinkUpGO(uint32 SqlId);
        void ChannelLinkUpCreature(uint32 SqlId);
        bool haslinkupevent;
        WayPoint* CreateWaypointStruct();
        uint32 spawnid;
        uint32 original_emotestate;

        CreatureSpawn* m_spawn;

        void OnPushToWorld();
        virtual void Despawn(uint32 delay, uint32 respawntime);
        void TriggerScriptEvent(int);

        AuctionHouse* auctionHouse;

        void DeleteMe();
        bool CanAddToWorld();

        // scriptdev2
        uint32 GetNpcTextId();

        WayPointMap* m_custom_waypoint_map;
        void LoadWaypointGroup(uint32 pWaypointGroup);
        void LoadCustomWaypoint(float pX, float pY, float pZ, float pO, uint32 pWaitTime, uint32 pFlags, bool pForwardEmoteOneshot, uint32 pForwardEmoteId, bool pBackwardEmoteOneshot, uint32 pBackwardEmoteId, uint32 pForwardSkinId, uint32 pBackwardSkinId);
        void SwitchToCustomWaypoints();
        Player* m_escorter;
        void DestroyCustomWaypointMap();
        bool IsInLimboState() { return m_limbostate; }
        void SetLimboState(bool set) { m_limbostate = set; };
        uint32 GetLineByFamily(CreatureFamilyEntry* family) {return family->skilline ? family->skilline : 0;};
        void RemoveLimboState(Unit* healer);
        void SetGuardWaypoints();
        bool m_corpseEvent;
        MapCell* m_respawnCell;
        bool m_noRespawn;
        uint32 m_respawnTimeOverride;

        float GetBaseParry();
        bool isattackable(CreatureSpawn* spawn);

        void DealDamage(Unit* pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras = false);
        void TakeDamage(Unit* pAttacker, uint32 damage, uint32 spellid, bool no_remove_auras = false);
        void Die(Unit* pAttacker, uint32 damage, uint32 spellid);

        void HandleMonsterSayEvent(MONSTER_SAY_EVENTS Event);

        uint32 GetType() { return m_Creature_type; }
        void SetType(uint32 t) { m_Creature_type = t; }

    protected:
        CreatureAIScript* _myScriptClass;
        bool m_limbostate;
        Trainer* mTrainer;

        void _LoadGoods();
        void _LoadGoods(std::list<CreatureItem*>* lst);
        void _LoadMovement();

        /// Vendor data
        std::vector<CreatureItem>* m_SellItems;

        /// Taxi data
        uint32 mTaxiNode;

        /// Quest data
        std::list<QuestRelation*>* m_quests;

        uint32 m_enslaveCount;
        uint32 m_enslaveSpell;

        bool m_PickPocketed;
        uint32 _fields[UNIT_END];
        uint32 m_healthfromspell;

        CreatureInfo* creature_info;
        CreatureProto* proto;
		CreatureDifficulty* difficulty;
    private:
        uint32 m_Creature_type;
};

#endif // _WOWSERVER_CREATURE_H
