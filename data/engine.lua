-- Main game engine script

local game = nil

function on_engine_load()
	local human = am.race.new("human")
	am.engine.add_race(human)
end

function new_game(scenario_name)
	game = am.game.new()
	am.engine.game(game)
	
	scenario, msg = loadfile("data/scenario/" .. scenario_name .. "/main.lua")
	if (scenario == nil) then
		am_log("Unable to find main.lua for " .. scenario_name .. "\n" .. msg)
		return 0
	end
	scenario().new_game(game)
	return 1
end

function load_game(save_name)
	game = am.game.new()
	am.engine.game(game)
	
	game:load_game(save_name);
	
	scenario_name = game:scenario_name()
	
	scenario = loadfile("data/scenario/" .. scenario_name .. "/main.lua")
	if (scenario == nil) then
		am_log("Unable to find main.lua for " .. scenario_name)
		return 0
	end
	
	scenario().load_game(game)
	
	return 1
end