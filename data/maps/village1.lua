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
		elder:grid_location(6, 16)
			:name("John")
		map:add_game_object(elder)
		
		local comp = am.dialogue_component.new(elder)
			:available("elder_intro", true)
			:available("elder_old", true)
			:available("elder_village", true)
			:start_dialogue("elder_intro")
		elder:dialogue_component(comp)
	end
	-- End Character
	
	-- Items
	local item, new_item = am.item.new(map_name .. "_scroll_intro", "wooden:scroll")
	if (new_item) then
		item:grid_location(7, 16)
		map:add_game_object(item)
	end
	item:on("item_pickedup", function(context, event)
		am.debug.log("PICKED UP!")
	end)
	item:on("item_dropped", function(context, event)
		am.debug.log("DROPPED!")
	end)
	-- End Items

	-- Quests

	-- End Quests

	return map
end