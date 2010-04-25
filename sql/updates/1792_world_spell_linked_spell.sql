ALTER TABLE `spell_linked_spell` ADD UNIQUE KEY `trigger_effect_type` (`spell_trigger`,`spell_effect`,`type`);
INSERT INTO `spell_linked_spell` VALUES ('9712', '21029', '0', 'Thaumaturgy Channel - Fill Vessel');
