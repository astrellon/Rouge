-- Test Lua Map
-- Game Imports
Map, Engine, Game, Character, Dialogue = import("Map", "Engine", "Game", "Character", "Dialogue")
-- Gfx Imports
Sprite = import("Sprite")

game = Engine.get_game()

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

diag = Dialogue.new("diag1",
	"Hello there ${char main} <? #='HART' @='diag2'>Next</?>")
Dialogue.add_dialogue(diag)

diag = Dialogue.new("diag2",
	"My name is <? #='name' @='diag3'>Melli</?> How are you today?",
	"How Are", "how_are", "locked")
Dialogue.add_dialogue(diag)

diag = Dialogue.new("diag3", "Melli is my name, <x>good bye</x>", "Name", "name", "locked")
Dialogue.add_dialogue(diag)

npc1 = Character.new("npc1")
npc1:set_grid_location(2, 0)
npc1:set_graphic(Sprite.new("characters/npc/front"))
npc1:set_name("John")
map:add_game_object(npc1)

npc2 = Character.new("npc2")
npc2:set_grid_location(4, 1)
npc2:set_name("Fred")
npc2:set_graphic(Sprite.new("characters/npc/front"))
map:add_game_object(npc2)

game:set_current_map(map)