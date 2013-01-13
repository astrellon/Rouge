-- Test Lua Map
do
	-- Game Imports
	Map, Engine, Game, Character, Dialogue = import("Map", "Engine", "Game", "Character", "Dialogue")
	DialogueComponent, Quest = import("DialogueComponent", "Quest")
	-- Gfx Imports
	Sprite = import("Sprite")

	game = Engine.get_game()

	-- Map
	map = Map.new("testMap", 6, 7)
	map:set_full_name("The Region of Test")
	game:add_map(map)

	Engine.clear_using_tile_set()
	Engine.using_tile_set("nature")
	Engine.get_game():add_map(map)

	map:set_tiles({
		"brick:0", "brick:2", "grass:0", "grass:2", "grass:3", "dirt:3",
		"grass:1", "brick:0", "grass:1", "dirt:0", "dirt:2", "grass:1",
		"dirt:0", "brick:2", "grass:2", "dirt:3", "dirt:0", "grass:3",
		"brick:2", "brick:1", "grass:3", "dirt:2", "dirt:1", "swamp",
		"brick:1", "brick:0", "grass:1", "grass:0", "swamp", "water",
		"brick:2", "brick:2", "grass:2", "grass:3", "water", "water",
		"brick:2", "brick:2", "grass:0", "swamp", "water", "water",
		"brick:2", "brick:2", "grass:3", "swamp", "water", "water"
	})
	-- End Map

	-- Dialogue
	diag = Dialogue.new("diag1",
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
		"Oh Fred, he's a guy, who does stuff what not not.", "About Fred", "aboutFred", "locked")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("imFred", "Hi, my name is Fred")
	Dialogue.add_dialogue(diag)

	diag = Dialogue.new("fredQuest", "Oh, you've talked to me, how about that.")
	Dialogue.add_dialogue(diag)
	-- End Dialogue

	-- Characters
	npc1 = Character.new("npc1")
	npc1:set_grid_location(2, 0)
	npc1:set_graphic(Sprite.new("characters/npc/front"))
	npc1:set_name("John")
	map:add_game_object(npc1)

	comp = DialogueComponent.new(npc1)
	npc1:set_dialogue_component(comp)
	comp:set_dialogue_available("diag1")
	comp:set_dialogue_available("diag2")
	comp:set_dialogue_available("diag3")
	comp:set_dialogue_available("startQuest")
	comp:set_dialogue_available("aboutFred")
	comp:set_start_dialogue("diag1")

	npc2 = Character.new("npc2")
	npc2:set_grid_location(4, 1)
	npc2:set_name("Fred")
	npc2:set_graphic(Sprite.new("characters/npc/front"))
	map:add_game_object(npc2)

	comp = DialogueComponent.new(npc2)
	npc2:set_dialogue_component(comp)
	comp:set_dialogue_available("imFred")
	comp:set_start_dialogue("imFred")
	-- End Character

	-- Quests
	fredQuest = Quest.new("fredQuest")
	fredQuest:add_event_listener("startQuest", function(event)
		npc2:get_dialogue_component():set_dialogue_available("fredQuest")
	end)
	fredQuest:add_event_listener("finishQuest", function(event)
		am_log("Finished Fred Quest")
	end)

	npc1:add_event_listener("dialogue:startQuest", function(event)
		fredQuest:start_quest()
	end)
	npc2:add_event_listener("dialogue:fredQuest", function(event)
		fredQuest:finish_quest()
	end)
	-- End Quests

	return map
end