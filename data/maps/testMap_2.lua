-- Test Lua Map
do
	-- Game Imports
	local Map, Engine, Game, Character, Dialogue = import("Map", "Engine", "Game", "Character", "Dialogue")
	local DialogueComponent, Quest = import("DialogueComponent", "Quest")
	-- Gfx Imports
	local Sprite = import("Sprite")

	local game = Engine.game()

	-- Map
	local map = Map.new("testMap", 18, 8)
	map:full_name("The Region of Test")
	game:add_map(map)

	Engine.clear_using_tile_set()
	Engine.using_tile_set("nature")
	Engine.game():add_map(map)
	Engine.game():add_map(map)

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
	-- End Map

	-- Dialogue
	local diag = Dialogue.new("diag1",
		"Hello there ${char main} <? @='diag2'>Next</?>")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("diag2",
		"My name is <? @='diag3'>Melli</?> How are you today?\nWould you like to do <? @='startQuest'>something</?> for me?",
		"How Are", "how_are", "locked")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("diag3", "Melli is my name, <x>good bye</x>", "Name", "name", "locked")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("startQuest", 
		"Fantastic! I would like you to talk to <? @='aboutFred'>Fred</?> over there for me?", "Talk to Fred", "startQuest", "locked")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("aboutFred",
		"Oh Fred, he's a guy, who does stuff what not.", "About Fred", "aboutFred", "locked")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("imFred", "Hi, my name is Fred")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("fredQuest", "Oh, you've talked to me, how about that.")
	Dialogue.add_dialogue(diag)
	-- End Dialogue

	-- Characters
	local npc1, newChar = Character.new("map2_john", "npcs:male1")
	if (newChar) then
		npc1:grid_location(2, 0)
			:graphic(Sprite.new("characters/npc/front"))
			:name("John")
		map:add_game_object(npc1)
		
		local comp = DialogueComponent.new(npc1)
			:available("diag1", true)
			:available("diag2", true)
			:available("diag3", true)
			:available("startQuest", true)
			:available("aboutFred", true)
			:start_dialogue("diag1")
		npc1:dialogue_component(comp)
	end
	
	local npc2, newChar = Character.new("map2_fred", "npcs:male2")
	if (newChar) then
		npc2:grid_location(4, 1)
			:name("Fred")
			:graphic(Sprite.new("characters/npc/front"))
		map:add_game_object(npc2)
		
		comp = DialogueComponent.new(npc2)
			:available("imFred", true)
			:start_dialogue("imFred")
		npc2:dialogue_component(comp)
	end
	-- End Character
	
	-- Items
	
	-- End Items

	-- Quests
	local fredQuest, newQuest = Quest.new("fredQuest")
	if (newQuest) then
		fredQuest:title("Fred Quest")
		game:add_quest(fredQuest)
	end
	fredQuest:on("startQuest", function(event)
		npc2:dialogue_component():available("fredQuest", true)
	end)
	fredQuest:on("finishQuest", function(event)
		am_log("Finished Fred Quest")
		game:main():add_experience(1000)
	end)

	npc1:on("dialogue:startQuest", function(event)
		fredQuest:start_quest()
	end)
	npc2:on("dialogue:fredQuest", function(event)
		fredQuest:finish_quest()
	end)
	-- End Quests

	return map
end