-- Village 2
do
	local game = am.engine.game()
	
	-- Map
	local map_file, msg = loadfile("data/maps/village2_map.lua")
	if (map_file == nil) then
		am.debug.log("Unable to find map data: " .. msg)
		return nil
	end
	
	local map = map_file()
	game:add_map(map)
	
	local map_name = map:name()
	-- End Map

	-- Dialogue
	local diag = am.dialogue.new("smithy_intro",
		"Greetings, my name is 'name' welcome to the second <? @='smithy_village'>village</?>",
		"Welcome")
	am.dialogue.add_dialogue(diag)

	diag = am.dialogue.new("smithy_village",
		"Old? Yes I'm old! Really old!",
		"Village", "smithy_village", "locked")
	am.dialogue.add_dialogue(diag)
	-- End Dialogue
	
	-- Map Regions, these should be defined in the _map file itself at some point.
	local region = am.map_region.new(3, 1, 1)
	region:location(5, 1)
	map:add_map_region(region)
	
	region:on("region_entered", function(event)
	    am.debug.log("entered: " .. event.game_object:name())
		game:move_object_to_map_grid(event.game_object, "village1", 2, 2)
	end)
	-- End Map Regions

	-- Characters
	local smithy, new_char = am.character.new(map_name .. "_smithy", "npcs:male1")
	if (new_char) then
		smithy:graphic(am.sprite.new("characters/human:male2"))
			:grid_location(13, 6)
			:name("Smithy")
		map:add_game_object(smithy)
		
		local comp = am.dialogue_component.new(smithy)
			:available("smithy_intro", true)
			:available("smithy_village", true)
			:start_dialogue("smithy_intro")
		smithy:dialogue_component(comp)
	end
	-- End Character
	
	-- Items
	local item, new_item = am.item.new(map_name .. "_scroll_intro", "wooden:scroll")
	if (new_item) then
		item:grid_location(7, 5)
		map:add_game_object(item)
	end
	item:description("Hello babbit :3\nI hope you found this second message well.")
	-- End Items
	
	-- Doors

	-- End Doors

	-- Quests

	-- End Quests
	return map
end