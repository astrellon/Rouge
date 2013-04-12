-- Main game engine script
Engine, Game, Race, Character, Item, Inventory = import("Engine", "Game", "Race", "Character", "Item", "Inventory")
Sprite = import("Sprite")

local game = nil

function onLoad()
	local human = Race.new("human")
	Engine.add_race(human)
end
onLoad()

function newGame(scenario_name)
	game = Game.new()
	Engine.game(game)
	
	scenario = loadfile("data/scenario/" .. scenario_name .. "/main.lua")
	if (scenario == nil) then
		am_log("Unable to find main.lua for " .. scenario_name)
		return 0
	end
	scenario()(game)
	return 1
end