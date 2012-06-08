SET @SHADRON := 30451;
SET @VESPERON := 30449;
SET @TENEBRON := 30452;
-- Delete Shadron Texts
DELETE FROM `script_texts` WHERE `npc_entry` = @SHADRON;
DELETE FROM `script_texts` WHERE `entry` IN (-1615008,-1615041,-1615017,-1615042);
-- Insert Shadron Texts
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@SHADRON,0,0,"I fear nothing, least of all you!",14,0,100,0,0,14111,"Shadron - SAY_SHADRON_AGGRO"),
(@SHADRON,1,0,"You are insignificant!",14,0,100,0,0,14112,"Shadron - SAY_SHADRON_SLAY_1"),
(@SHADRON,1,1,"Such mediocre resistance!",14,0,100,0,0,14113,"Shadron - SAY_SHADRON_SLAY_2"),
(@SHADRON,2,0,"We...are superior! How could this...be...",14,0,100,0,0,14118,"Shadron - SAY_SHADRON_DEATH"),
(@SHADRON,3,0,"You are easily bested",14,0,100,0,0,14114,"Shadron - SAY_SHADRON_BREATH"),
(@SHADRON,4,0,"I will take pity on your Sartharion, just this once.",14,0,100,0,0,14117,"SAY_SHADRON_RESPOND"),
(@SHADRON,5,0,"Father tought me well!",14,0,100,0,0,14115,"Shadron - SAY_SHADRON_SPECIAL_1"),
(@SHADRON,5,1,"On your knees!",14,0,100,0,0,14116,"Shadron - SAY_SHADRON_SPECIAL_2"),
(@SHADRON,6,0,"Shadron begins to open a Twilight Portal",42,0,100,0,0,0,"Shadron - SAY_SHADRON_OPEN_PORTAL"),
(@SHADRON,7,0,"A Shadron Disciple appears in the Twilight!",42,0,100,0,0,0,"Shadron - SAY_SHADRON_DISCIPLE");

-- Delete Vesperon Texts
DELETE FROM `script_texts` WHERE `npc_entry` = @VESPERON;
-- Insert Vesperon Texts
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@VESPERON,0,0,"You pose no threat, lesser beings...give me your worst!",14,0,100,0,0,14133,"Vesperon - SAY_VESPERON_AGGRO"),
(@VESPERON,1,0,"The least you could do is put up a fight",14,0,100,0,0,14314,"Vesperon - SAY_VESPERON_SLAY_1"),
(@VESPERON,1,1,"Was that the best you can do?",14,0,100,0,0,14135,"Vesperon - SAY_VESPERON_SLAY_2"),
(@VESPERON,2,0,"I still have some...fight...in...me...",14,0,100,0,0,14140,"Vesperon - SAY_VESPERON_DEATH"),
(@VESPERON,3,0,"I will pick my teeth with your bones!",14,0,100,0,0,14136,"Vesperon - SAY_VESPERON_BREATH"),
(@VESPERON,4,0,"Father was right about you, Sartharion...You are a weakling!",14,0,100,0,0,14139,"Vesperon - SAY_VESPERON_RESPOND"),
(@VESPERON,5,0,"Aren't you tricky...I have a few trick of my own...",14,0,100,0,0,14137,"Vesperon - SAY_VESPERON_SPECIAL_1"),
(@VESPERON,5,1,"Unlike you, I have many talents.",14,0,100,0,0,14138,"Vesperon - SAY_VESPERON_SPECIAL_2"),
(@VESPERON,6,0,"Vesperon begins to open a Twilight Portal",42,0,100,0,0,0,"Vesperon - SAY_VESPERON_OPEN_PORTAL"),
(@VESPERON,7,0,"A Vesperon Disciple appears in the Twilight!",42,0,100,0,0,0,"Vesperon - SAY_VESPERON_DISCIPLE");

-- Delete Tenebron Texts
DELETE FROM `script_texts` WHERE `npc_entry` = @TENEBRON;
-- Insert Tenebron Texts
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@TENEBRON,0,0,"You have no place here.Your place is among the departed.",14,0,100,0,0,14122,"Tenebron - SAY_TENEBRON_AGGRO"),
(@TENEBRON,1,0,"No contest",14,0,100,0,0,14123,"Tenebron - SAY_TENEBRON_SLAY_1"),
(@TENEBRON,1,1,"Typical...Just as I was having fun.",14,0,100,0,0,14124,"Tenebron - SAY_TENEBRON_SLAY_2"),
(@TENEBRON,2,0,"I should not...have held back...",14,0,100,0,0,14129,"Tenebron - SAY_TENEBRON_DEATH"),
(@TENEBRON,3,0,"To darkness I condemn you...",14,0,100,0,0,14125,"Tenebron - SAY_TENEBRON_BREATH"),
(@TENEBRON,4,0,"It is amusing to watch you struggle.Very well witness how it is done.",14,0,100,0,0,14128,"Tenebron - SAY_TENEBRON_RESPOND"),
(@TENEBRON,5,0,"Arrogant little creatures! To challenge the powers you do not yet understand",14,0,100,0,0,14126,"Tenebron - SAY_TENEBRON_SPECIAL_1"),
(@TENEBRON,5,1,"I am no mere dragon! You will find that I am much, much, much, more...",14,0,100,0,0,14127,"Tenebron - SAY_TENEBRON_SPECIAL_2"),
(@TENEBRON,6,0,"Tenebron begins to hatch eggs in the twilight",42,0,100,0,0,0,"Vesperon - SAY_HATCH_EGGS");
