-- Test Lua Map
Map, Engine, Game, Character = import("Map", "Engine", "Game", "Character")

game = Engine.get_game()

map = Map.new("testMap", 6, 7)
map:set_full_name("The Region of Test")

Engine.clear_using_tile_set()
Engine.using_tile_set("nature")
Engine.get_game():add_map(map)

map:set_tiles({
	"brick:0", "brick:2", "grass:0", "grass:2", "grass:3", "dirt:3",
	"brick:1", "brick:0", "grass:1", "dirt:0", "dirt:2", "grass:1",
	"brick:0", "brick:2", "grass:2", "dirt:3", "dirt:0", "grass:3",
	"brick:2", "brick:1", "grass:3", "dirt:2", "dirt:1", "swamp",
	"brick:1", "brick:0", "grass:1", "grass:0", "swamp", "water",
	"brick:2", "brick:2", "grass:2", "grass:3", "water", "water",
	"brick:2", "brick:2", "grass:0", "swamp", "water", "water",
	"brick:2", "brick:2", "grass:3", "swamp", "water", "water"
})

npc1 = Character.new("npc1")
npc1:set_grid_location(2, 0)
map:add_game_object(npc1)

npc2 = Character.new("npc2")
npc2:set_grid_location(4, 1)
map:add_game_object(npc2)

--function onEnterMap(e)
--	npc1:set_grid_location(2, 0)
--end
--game:add_event_listener("enter_map:testMap", onEnterMap)
