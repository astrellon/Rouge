-- Main game engine script
Engine, Game, Race, Character = import("Engine", "Game", "Race", "Character")
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
	
	player = Character.new("melli")
	player:name("Melanine")
		:graphic(Sprite.new("characters/mainChar/front"))
		:grid_location(2, 1)
		:gender("female")
		:race("human")
	
	game:main(player)
end