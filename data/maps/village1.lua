-- Village 1
do
	local game = am.engine.game()
	
	-- Map
	local map_file, msg = loadfile("data/maps/village1_map.lua")
	if (map_file == nil) then
		am.debug.log("Unable to find map data: " .. msg)
		return nil
	end
	
	local map = map_file()
	game:add_map(map)
	
	local map_name = map:name()
	-- End Map
	
	-- Map Regions, these should be defined in the _map file itself at some point.
	local region = am.map_region.new(3, 2, 1)
	region:location(14, 13)
	map:add_map_region(region)
	
	region:on("region_entered", function(event)
	    am.debug.log("entered: " .. event["game_object"]:name())
		game:move_object_to_map_grid(game:main(), "village2", 2, 2)
	end)
	region:on("region_exited", function(event)
	    am.debug.log("exited: " .. event["game_object"]:name())
	end)
	-- End Map Regions

	-- Dialogue
	local diag = am.dialogue.new("elder_intro",
		"Why hello there young miss, welcome to our <? @='elder_village'>village</?>. My name is Frank and I'm <? @='elder_old'>old</?>.",
		"Welcome")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("elder_old",
		"Old? Yes I'm old! Really old!",
		"Old!", "elder_old", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("elder_village", "This village is huge! And old! You should talk to the people who live here, besides me!", "Village", "village", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("startQuest", 
		"Fantastic! I would like you to talk to <? @='aboutFred'>Fred</?> over there for me?", "Talk to Fred", "startQuest", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("aboutFred",
		"Oh Fred, he's a guy, who does stuff what not.", "About Fred", "aboutFred", "locked")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("imFred", "Hi, my name is Fred")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("fred_quest", "Oh, you've talked to me, how about that.")
	am.dialogue.add_dialogue(diag)
	-- End Dialogue

	-- Characters
	local elder, new_char = am.character.new(map_name .. "_elder", "npcs:male1")
	if (new_char) then
		elder:graphic(am.sprite.new("characters/human:male_elder"))
			:grid_location(6, 16)
			:name("Village Elder")
		map:add_game_object(elder)
		
		local comp = am.dialogue_component.new(elder)
			:available("elder_intro", true)
			:available("elder_old", true)
			:available("elder_village", true)
			:start_dialogue("elder_intro")
		elder:dialogue_component(comp)
	end
	-- AI Controller function
	elder:ai_func(function(char, dt)
		if (char:has_destination() == false) then
			local width, height = map:map_size()
			local x = math.modf(math.random(0, width - 1))
			local y = math.modf(math.random(0, height - 1))
			char:grid_destination(x, y)
		end
	end)
	
	local john, new_char = am.character.new(map_name .. "_john", "npcs:male1")
	if (new_char) then
		john:graphic(am.sprite.new("characters/human:male"))
			:grid_location(5, 6)
			:name("John McGee")
		map:add_game_object(john)
	end
	
	local fred, new_char = am.character.new(map_name .. "_fred", "npcs:male1")
	if (new_char) then
		fred:graphic(am.sprite.new("characters/human:male2"))
			:grid_location(13, 6)
			:name("Fred McGee")
		map:add_game_object(fred)
	end
	-- End Character
	
	-- Items
	local item, new_item = am.item.new(map_name .. "_scroll_intro", "wooden:scroll")
	if (new_item) then
		item:grid_location(7, 16)
		map:add_game_object(item)
	end
	item:description("Hello babbit :3\nI hope you found my message well.")
	-- End Items
	
	-- Doors
	local john_door, new_door = am.door.new(john:name() .. "_door")
	if (new_door) then
		john_door:graphic(am.sprite.new("wooden_door:main"))
			:grid_location(5, 8)
			:name("Johns Wooden Door")
		map:add_game_object(john_door)
	end
	
	local fred_door, new_door = am.door.new(fred:name() .. "_door")
	if (new_door) then
		fred_door:graphic(am.sprite.new("wooden_door:main"))
			:grid_location(14, 8)
			:name("Freds Wooden Door")
		map:add_game_object(fred_door)
	end
	-- End Doors

	-- Quests

	-- End Quests

	return map
end