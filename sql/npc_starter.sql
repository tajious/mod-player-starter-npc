-- --------------------------------------------------------------------------------------
--	ENCHANTER - 607509
-- --------------------------------------------------------------------------------------
SET
@Entry 		:= 607509,
@Model 		:= 27822, -- Human female
@Name 		:= "Matilda Gearhead",
@Title 		:= "Technician",
@Icon 		:= "Speak",
@GossipMenu := 0,
@MinLevel 	:= 80,
@MaxLevel 	:= 80,
@Faction 	:= 35,
@NPCFlag 	:= 1,
@Scale		:= 1.0,
@Type 		:= 7,
@TypeFlags 	:= 0,
@FlagsExtra := 2,
@AIName		:= "SmartAI",
@Script 	:= "npc_starter";

-- NPC
DELETE FROM creature_template WHERE entry = @Entry;
INSERT INTO creature_template (entry, modelid1, name, subname, IconName, gossip_menu_id, minlevel, maxlevel, faction, npcflag, speed_walk, speed_run, scale, unit_class, unit_flags, type, type_flags, InhabitType, RegenHealth, flags_extra, AiName, ScriptName) VALUES
(@Entry, @Model, @Name, @Title, @Icon, @GossipMenu, @MinLevel, @MaxLevel, @Faction, @NPCFlag, 1, 1.14286, @Scale, 1, 2, @Type, @TypeFlags, 3, 1, @FlagsExtra, @AIName, @Script);

-- NPC TEXT
DELETE FROM `npc_text` WHERE `ID`=@Entry;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES (@Entry, 'Hello $N. Gearhead to the rescue!');
