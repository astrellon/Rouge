-- Test Scenario 1
-- 
return {
	newGame = function(game)
		am_log("NEW GAME!!!!!")
		map = game:map("testMap_2")
		if (map ~= nil) then
			game:current_map(map)
		end
		
		player = Character.from_def("races:human", "melli")
		player:name("Melanie")
			:graphic(Sprite.new("characters/mainChar/front"))
			:grid_location(2, 4)
			:gender("female")
			:stats():base_stat("speed", 2)
		local sword = Item.from_def("wooden:sword")
		sword:add_body_type("arm", "arm"):add_body_type("hand", "hand")	
		player:inventory():add_item(sword)
		player:inventory():add_item(Item.from_def("wooden:shield"))
		
		game:add_game_object_to_map(player)
		game:main(player)
		
		game:start_game()
	end,
	loadGame = function(game)
		game:start_game()
		
		player = game:main()
		
		local sword = Item.find("sword1")
		am_log("Sword1? ", sword)
		am_log(sword:get_body_types())
	end
}