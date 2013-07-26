-- Test Lua Map
do
	local game = am.engine.game()
	
	-- Map
	local map = am.map.new("testMap", 18, 8)
	map:full_name("The Region of Test")

	am.engine.clear_using_tile_set()
	am.engine.using_tile_set("nature")
	am.engine.game():add_map(map)
	am.engine.game():add_map(map)

	map:tiles({
		"brick:0", "brick:2", "grass:0", "grass:2", "grass:3", "dirt:3","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0",
		"grass:1", "brick:0", "grass:1", "dirt:0", "dirt:2", "grass:1", "grass:1","water","grass:1","grass:1","grass:1","grass:1","grass:1","grass:1","grass:1","grass:1","grass:1","grass:1",
		"dirt:0", "brick:2",  "grass:2", "dirt:3", "dirt:0", "grass:3", "grass:2","water","grass:2","water","water","water","grass:2","grass:2","grass:2","grass:2","grass:2","grass:2",
		"brick:2", "brick:1", "grass:3", "dirt:2", "dirt:1", "swamp",   "grass:3","water","grass:3","water","water","water","grass:3","grass:3","grass:3","grass:3","grass:3","grass:3",
		"brick:1", "brick:0", "grass:1", "grass:0", "swamp", "water",   "grass:1","water","grass:1","water","water","water","grass:1","grass:1","grass:1","grass:1","grass:1","grass:1",
		"brick:2", "brick:2", "grass:2", "grass:3", "water", "water",   "grass:2","water","grass:2","water","water","water","grass:2","grass:2","grass:2","grass:2","grass:2","grass:2",
		"brick:2", "brick:2", "grass:0", "swamp", "water", "water",     "grass:0","water","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0","grass:0",
		"brick:2", "brick:2", "grass:3", "swamp", "water", "water",     "grass:3","grass:3","grass:3","grass:3","grass:3","grass:3","grass:3","grass:3","grass:3","grass:3","grass:3","grass:3"
	})
	
	game:add_map(map)
	-- End Map

	-- Dialogue
	local diag = am.dialogue.new("diag1",
		"Hello there ${char main} <? @='diag2'>Next</?>")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("diag2",
		"My name is <? @='diag3'>Melli</?> How are you today?\nWould you like to do <? @='startQuest'>something</?> for me?",
		"How Are", "how_are", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("diag3", "Melli is my name, <x>good bye</x>", "Name", "name", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("startQuest", 
		"Fantastic! I would like you to talk to <? @='aboutFred'>Fred</?> over there for me?", "Talk to Fred", "startQuest", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("aboutFred",
		"Oh Fred, he's a guy, who does stuff what not.", "About Fred", "aboutFred", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("imFred", "Hi, my name is Fred")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("fred_quest", "Oh, you've talked to me, how about that.")
	am.dialogue.add_dialogue(diag)
	-- End Dialogue

	-- Characters
	local npc1, new_char = am.character.new("map2_john", "npcs:male1")
	if (new_char) then
		npc1:grid_location(2, 0)
			:graphic(am.sprite.new("characters/npc/front"))
			:name("John")
		map:add_game_object(npc1)
		
		local comp = am.dialogue_component.new(npc1)
			:available("diag1", true)
			:available("diag2", true)
			:available("diag3", true)
			:available("startQuest", true)
			:available("aboutFred", true)
			:start_dialogue("diag1")
		npc1:dialogue_component(comp)
	end
	
	local npc2, new_char = am.character.new("map2_fred", "npcs:male2")
	if (new_char) then
		npc2:grid_location(4, 1)
			:name("Fred")
			:graphic(am.sprite.new("characters/npc/front"))
		map:add_game_object(npc2)
		
		comp = am.dialogue_component.new(npc2)
			:available("imFred", true)
			:start_dialogue("imFred")
		npc2:dialogue_component(comp)
	end
	-- End Character
	
	-- Items
	
	-- End Items

	-- Quests
	local fred_quest, new_quest = am.quest.new("fred_quest")
	if (new_quest) then
		fred_quest:title("Fred Quest")
		game:add_quest(fred_quest)
	end
	fred_quest:on("startQuest", function(event)
		npc2:dialogue_component():available("fred_quest", true)
	end)
	fred_quest:on("finishQuest", function(event)
		am_log("Finished Fred Quest")
		game:main():add_experience(1000)
	end)

	npc1:on("dialogue:startQuest", function(event)
		fred_quest:start_quest()
	end)
	npc2:on("dialogue:fred_quest", function(event)
		fred_quest:finish_quest()
	end)
	-- End Quests

	return map
end