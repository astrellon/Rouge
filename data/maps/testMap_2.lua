-- Test Lua Map
do
	-- Game Imports
	Map, Engine, Game, Character, Dialogue = import("Map", "Engine", "Game", "Character", "Dialogue")
	DialogueComponent, Quest = import("DialogueComponent", "Quest")
	-- Gfx Imports
	Sprite = import("Sprite")

	game = Engine.game()

	-- Map
	map = Map.new("testMap", 6, 7)
	map:full_name("The Region of Test")
	game:add_map(map)

	Engine.clear_using_tile_set()
	Engine.using_tile_set("nature")
	Engine.game():add_map(map)

	map:tiles({
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
		:grid_location(2, 0)
		:graphic(Sprite.new("characters/npc/front"))
		:name("John")
	map:add_game_object(npc1)

	comp = DialogueComponent.new(npc1)
		:available("diag1", true)
		:available("diag2", true)
		:available("diag3", true)
		:available("startQuest", true)
		:available("aboutFred", true)
		:start_dialogue("diag1")
	npc1:dialogue_component(comp)

	npc2 = Character.new("npc2")
		:grid_location(4, 1)
		:name("Fred")
		:graphic(Sprite.new("characters/npc/front"))
	map:add_game_object(npc2)

	comp = DialogueComponent.new(npc2)
		:available("imFred", true)
		:start_dialogue("imFred")
	npc2:dialogue_component(comp)
	-- End Character

	-- Quests
	fredQuest = Quest.new("fredQuest")
	fredQuest:on("startQuest", function(event)
		npc2:dialogue_component():available("fredQuest", true)
	end)
	fredQuest:on("finishQuest", function(event)
		am_log("Finished Fred Quest")
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