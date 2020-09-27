/*
 *
### LICENSE
------------------------------------------------------------------------------------------------------------------
This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).
*/

#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"


enum NpcStarter {
    NPC_STARTER_ID = 607509
};


uint32 CfgNpcStarterAnnounce;
uint32 CfgNpcStarterMessageTimer;
uint32 CfgNpcStarterNumPhrases;

class NpcStarterConfig : public WorldScript
{
public:
    NpcStarterConfig() : WorldScript("NpcStarterConfig_conf") {}

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/npc_starter.conf";

#ifdef WIN32
            cfg_file = "npc_starter.conf";
#endif
            std::string cfg_def_file = cfg_file + ".dist";
            sConfigMgr->LoadMore(cfg_def_file.c_str());
            sConfigMgr->LoadMore(cfg_file.c_str());
            CfgNpcStarterAnnounce = sConfigMgr->GetBoolDefault("Starter.Announce", 1);
            CfgNpcStarterMessageTimer = sConfigMgr->GetIntDefault("Starter.MessageTimer", 60000);
            CfgNpcStarterNumPhrases = sConfigMgr->GetIntDefault("Starter.NumPhrases", 3);

            if (CfgNpcStarterNumPhrases > 0) {
                if (CfgNpcStarterMessageTimer < 60000 || CfgNpcStarterMessageTimer > 300000) {
                    CfgNpcStarterMessageTimer = 60000;
                }
            }

        }
    }
};


class NpcStarterAnnounce : public PlayerScript
{
public:
    NpcStarterAnnounce() : PlayerScript("NpcStarterAnnounce") {}

    void OnLogin(Player *player) override
    {
        if (CfgNpcStarterAnnounce) {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00NpcStarter |rmodule.");
        }
    }
};


class npc_starter : public CreatureScript
{
public:

    npc_starter() : CreatureScript("npc_starter")
    {}

    static string PickPhrase()
    {
        uint32 phraseNum = urand(1, CfgNpcStarterNumPhrases);
        std::string phrase = "St.P" + std::to_string(phraseNum);
        std::string randMessage = sConfigMgr->GetStringDefault(phrase, "Silly old gods...");

        return randMessage.c_str();
    }


    bool OnGossipHello(Player* player, Creature* creature) override
    {
        switch (player->getClass())
        {
            case CLASS_WARRIOR:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_rogue_eviscerate:30|t|r Use Arms Spec", GOSSIP_SENDER_MAIN, 1);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_warrior_innerrage:30|t|r Use Fury Spec", GOSSIP_SENDER_MAIN, 2);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_warrior_defensivestance:30|t|r Use Protection Spec", GOSSIP_SENDER_MAIN, 3);
                break;
            case CLASS_PALADIN:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_holybolt:30|t|r Use Holy Spec", GOSSIP_SENDER_MAIN, 4);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_devotionaura:30|t|r Use Protection Spec", GOSSIP_SENDER_MAIN, 5);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_auraoflight:30|t|r Use Retribution Spec", GOSSIP_SENDER_MAIN, 6);
                break;
            case CLASS_HUNTER:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_hunter_beasttaming:30|t|r Use Beastmastery Spec", GOSSIP_SENDER_MAIN, 7)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_marksmanship:30|t|r Use Markmanship Spec", GOSSIP_SENDER_MAIN, 8);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_Hunter_swiftstrike:30|t|r Use Survival Spec", GOSSIP_SENDER_MAIN, 9);
                break;
            case CLASS_ROGUE:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_rogue_eviscerate:30|t|r Use Assasination Spec", GOSSIP_SENDER_MAIN, 10);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_backstab:30|t|r Use Combat Spec", GOSSIP_SENDER_MAIN, 11);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_stealth:30|t|r Use Subtlety Spec", GOSSIP_SENDER_MAIN, 12);
                break;
            case CLASS_DRUID:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_starfall:30|t|r Use Ballance Spec", GOSSIP_SENDER_MAIN, 13);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_racial_bearform:30|t|r Use Feral Spec", GOSSIP_SENDER_MAIN, 14);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_healingtouch:30|t|r Use Restoration Spec", GOSSIP_SENDER_MAIN, 15);
                break;
            case CLASS_MAGE:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_magicalsentry:30|t|r Use Arcane Spec", GOSSIP_SENDER_MAIN, 16);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_fire_flamebolt:30|t|r Use Fire Spec", GOSSIP_SENDER_MAIN, 17);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_frost_frostbolt02:30|t|r Use Frost Spec", GOSSIP_SENDER_MAIN, 18);
                break;
            case CLASS_WARLOCK:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_deathcoil:30|t|r Use Affliction Spec", GOSSIP_SENDER_MAIN, 19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_metamorphosis:30|t|r Use Demonology Spec", GOSSIP_SENDER_MAIN, 20);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_rainoffire:30|t|r Use Destruction Spec", GOSSIP_SENDER_MAIN, 21);
                break;
            case CLASS_SHAMAN:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_lightning:30|t|r Use Elemental Spec", GOSSIP_SENDER_MAIN, 22);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_lightningshield:30|t|r Use Enhancement Spec", GOSSIP_SENDER_MAIN, 23);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_magicimmunity:30|t|r Use Restoration Spec", GOSSIP_SENDER_MAIN, 24);
                break;
            case CLASS_PRIEST:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_wordfortitude:30|t|r Use Discipline Spec", GOSSIP_SENDER_MAIN, 25);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_holybolt:30|t|r Use Holy Spec", GOSSIP_SENDER_MAIN, 26);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_shadowwordpain:30|t|r Use Shadow Spec", GOSSIP_SENDER_MAIN, 27);
                break;
            case CLASS_DEATH_KNIGHT:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_deathknight_bloodpresence:30|t|r Use Blood Spec", GOSSIP_SENDER_MAIN, 28);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_deathknight_frostpresence:30|t|r Use Frost Spec", GOSSIP_SENDER_MAIN, 29);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_deathknight_unholypresence:30|t|r Use Unholy Spec", GOSSIP_SENDER_MAIN, 30);
                break;
        }

        player->PlayerTalkClass->SendGossipMenu(NPC_STARTER_ID, creature->GetGUID());

        return true;
    }

private:
    void _warriorArmsPvE(Player* player, Creature* creature)
    {

    }

    void _warriorArmsPvP(Player* player, Creature* creature)
    {

    }

    void _warriorFuryPvE(Player* player, Creature* creature)
    {

    }

    void _warriorFuryPvP(Player* player, Creature* creature)
    {

    }

    void _warriorProtectionPvE(Player* player, Creature* creature)
    {

    }

    void _warriorProtectionPvP(Player* player, Creature* creature)
    {

    }

    void _paladinHolyPvE(Player* player, Creature* creature)
    {

    }

    void _paladinHolyPvP(Player* player, Creature* creature)
    {

    }

    void _paladinProtectionPvE(Player* player, Creature* creature)
    {

    }

    void _paladinProtectionPvP(Player* player, Creature* creature)
    {

    }

    void _paladinRetributionPvE(Player* player, Creature* creature)
    {

    }

    void _paladinRetributionPvP(Player* player, Creature* creature)
    {

    }

    void _hunterBeastMasteryPvE(Player* player, Creature* creature)
    {

    }

    void _hunterBeastMasterPvP(Player* player, Creature* creature)
    {

    }

    void _hunterMarksManShipPvE(Player* player, Creature* creature)
    {

    }

    void _hunterMarksManShipPvP(Player* player, Creature* creature)
    {

    }

    void _hunterSurvivalPvE(Player* player, Creature* creature)
    {

    }

    void _hunterSurvivalPvP(Player* player, Creature* creature)
    {

    }

    void _rogueAssassinationPvE(Player* player, Creature* creature)
    {

    }

    void _rogueAssassinationPvP(Player* player, Creature* creature)
    {

    }

    void _rogueCombatPvE(Player* player, Creature* creature)
    {

    }

    void _rogueCombatPvP(Player* player, Creature* creature)
    {

    }

    void _rogueSubtletyPvE(Player* player, Creature* creature)
    {

    }

    void _rogueSubtletyPvP(Player* player, Creature* creature)
    {

    }

    void _druidBalancePvE(Player* player, Creature* creature)
    {

    }

    void _druidBalancePvP(Player* player, Creature* creature)
    {

    }

    void _druidFeralCatPvE(Player* player, Creature* creature)
    {

    }

    void _druidFeralBearPvE(Player* player, Creature* creature)
    {

    }

    void _druidFeralCatPvP(Player* player, Creature* creature)
    {

    }

    void _druidRestorationPvE(Player* player, Creature* creature)
    {

    }

    void druidRestorationPvP(Player* player, Creature* creature)
    {

    }

    void _mageArcanePvE(Player* player, Creature* creature)
    {

    }

    void _mageArcanePvP(Player* player, Creature* creature)
    {

    }

    void _mageFirePvE(Player* player, Creature* creature)
    {

    }

    void _mageFirePvP(Player* player, Creature* creature)
    {

    }

    void _mageFrostPvP(Player* player, Creature* creature)
    {

    }

    void _warlockAfflicationPvE(Player* player, Creature* creature)
    {

    }

    void _warlockAfflicationPvP(Player* player, Creature* creature)
    {

    }

    void _warlockDemonologyPvE(Player* player, Creature* creature)
    {

    }

    void _warlockDemonologyPvP(Player* player, Creature* creature)
    {

    }

    void _warlockDestructionPvE(Player* player, Creature* creature)
    {

    }

    void _warlockDestructionPvP(Player* player, Creature* creature)
    {

    }

    void _shamanElementalPvE(Player* player, Creature* creature)
    {

    }

    void _shamanElementalPvP(Player* player, Creature* creature)
    {

    }

    void _shamanEnhancementPvE(Player* player, Creature* creature)
    {

    }

    void _shamanEnhancementPvP(Player* player, Creature* creature)
    {

    }

    void _shamanRestorationPvE(Player* player, Creature* creature)
    {

    }

    void _shamanRestoration_PvP(Player* player, Creature* creature)
    {

    }

    void _priestDisciplinePvE(Player* player, Creature* creature)
    {

    }

    void _priestDisciplinePvP(Player* player, Creature* creature)
    {

    }

    void _priestHolyPvE(Player* player, Creature* creature)
    {

    }

    void _priestHolyPvP(Player* player, Creature* creature)
    {

    }

    void _priestShadowPvE(Player* player, Creature* creature)
    {

    }

    void _priestShadowPvP(Player* player, Creature* creature)
    {

    }

    void _deathKnightBloodPvE(Player* player, Creature* creature)
    {

    }

    void _deathKnightBloodPvP(Player* player, Creature* creature)
    {

    }

    void _deathKnightFrostPvE(Player* player, Creature* creature)
    {

    }

    void _deathKnightFrostPvP(Player* player, Creature* creature)
    {

    }

    void _deathKnightUnholyPvE(Player* player, Creature* creature)
    {

    }

    void _deathKnightUnholyPvP(Player* player, Creature* creature)
    {

    }

};


void AddNpcStarterScripts()
{
    NpcStarterConfig();
    NpcStarterAnnounce();
    npc_starter();
}
