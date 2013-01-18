-- Main game engine script
Engine, Game, Race, Character = import("Engine", "Game", "Race", "Character")

local game = nil

function onLoad()
	local human = Race.new("human")
	Engine.add_race(human)
end
onLoad()

function newGame()
	game = Game.new()
	Engine.set_current_game(game)
	
	am_log("NEW", "New game")
	
	map = game:get_map("testMap_2")
	if (map ~= nil) then
		game:set_current_map(map)
	end
	
	player = Character.new()
end