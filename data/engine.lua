-- Main game engine script

local game = nil

function am.on_engine_load()
	local human = am.race.new("human")
	am.engine.add_race(human)
	
	local load, msg = loadfile("data/tileTypes.lua")
	if (load == nil) then
		am.debug.log("Unable to load tile types " .. msg)
	else
		load()
	end
end

function am.new_game(scenario_name)
	game = am.game.new()
	am.engine.game(game)
	
	game:generic_dead_graphic("characters/dead")
	
	local scenario, msg = loadfile("data/scenario/" .. scenario_name .. "/main.lua")
	if (scenario == nil) then
		am.debug.log("Unable to find main.lua for " .. scenario_name .. "\n" .. msg)
		return 0
	end
	scenario().new_game(game)
	return 1
end

function am.load_game(save_name)
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
