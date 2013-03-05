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
	Engine.game(game)
	
	map = game:map("testMap_2")
	if (map ~= nil) then
		game:current_map(map)
	end
	
	-- player = Character.new()
end