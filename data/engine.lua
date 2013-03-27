-- Main game engine script
Engine, Game, Race, Character, Item, Inventory = import("Engine", "Game", "Race", "Character", "Item", "Inventory")
Sprite = import("Sprite")

local game = nil

function onLoad()
	local human = Race.new("human")
	Engine.add_race(human)
end
onLoad()

function newGame()
	game = Game.new()
	Engine.game(game)
	
	map = game:map("testMap_2")
	if (map ~= nil) then
		game:current_map(map)
	end
	
	player = Character.from_def("races:human", "melli")
	player:name("Melanine")
		:graphic(Sprite.new("characters/mainChar/front"))
		:grid_location(2, 1)
		:gender("female")
	player:inventory():add_item(Item.from_def("wooden:sword"))
	player:inventory():add_item(Item.from_def("wooden:shield"))
	
	game:add_game_object_to_map(player)
	game:main(player)
end