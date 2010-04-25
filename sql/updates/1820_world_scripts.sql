
-- Blackfathom Deeps Instance
UPDATE `instance_template` SET `script`='instance_blackfathom_deeps' WHERE  `map`=48;
UPDATE `gameobject_template` SET `ScriptName`='go_fire_akumai' WHERE `entry` in (21118, 21119, 21120, 21121);

-- ACID INFINITE SUMMONS FIXED (AGAIN)
UPDATE `creature_ai_scripts` SET `event_flags`=2 WHERE `id`=481102;
UPDATE `creature_ai_scripts` SET `event_flags`=2 WHERE `id`=1165702;

