-- Main game engine script
Engine, Game, Race = import("Engine", "Game", "Race")

local game = nil

function onLoad()
	local human = Race.new("human")
	Engine.add_race(human)
end
onLoad()

function newGame()
	game = Game.new()
	Engine.set_current_game(game)
	
	map = game:get_map("testMap_2")
	if (map ~= nil) then
		game:set_current_map(map)
	end
end