-- Test Lua Map
do
	-- Game Imports
	local Map, Engine, Game, Character, Dialogue = import("Map", "Engine", "Game", "Character", "Dialogue")
	local DialogueComponent, Quest = import("DialogueComponent", "Quest")
	-- Gfx Imports
	local Sprite = import("Sprite")

	local game = Engine.game()

	-- Map
	local map = Map.new("testMap", 3, 3)
	map:full_name("The Region of AStar Test")
	game:add_map(map)

	Engine.clear_using_tile_set()
	Engine.using_tile_set("nature")
	Engine.game():add_map(map)
	Engine.game():add_map(map)

	map:tiles({
		"grass:0", "grass:0", "grass:0",
		"grass:0", "water", "water",
		"grass:0", "water", "grass:0"
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