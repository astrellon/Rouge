-- Village
-- 
return {
	new_game = function(game)
		map = game:map("village1")
		if (map ~= nil) then
			game:current_map(map)
		end
		
		player = am.character.from_def("races:human", "player")
		player:name("Player Name")
			:graphic(am.sprite.new("characters/human:kaswe"))
			:grid_location(8, 18)
			:gender("female")
			:stats():base_stat("speed", 2)
		player:coin_purse():add_coin(80)
		local sword = am.item.from_def("wooden:sword")
		sword:add_body_type("arm", "arm"):add_body_type("hand", "hand")	
		player:equip_item(sword, "left_hand")
		--player:inventory():add_item(sword)
		player:inventory():add_item(am.item.from_def("wooden:shield"))
		
		game:add_game_object_to_map(player)
		game:main(player)
		
		game:start_game()
	end,
	load_game = function(game)
		game:start_game()
		
		player = game:main()
	end
}