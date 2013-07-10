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
	-- End Dialogue

	-- Characters
	-- End Character
	
	-- Items
	-- End Items

	-- Quests
	-- End Quests

	return map
end