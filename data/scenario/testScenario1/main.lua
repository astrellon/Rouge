-- Test Scenario 1
-- 
return function(game)
	map = game:map("testMap_2")
	if (map ~= nil) then
		game:current_map(map)
	end
	
	player = Character.from_def("races:human", "melli")
	player:name("Melanine")
		:graphic(Sprite.new("characters/mainChar/front"))
		:grid_location(2, 1)
		:gender("female")
	player:inventory():add_item(Item.from_def("wooden:sword"))
	player:inventory():add_item(Item.from_def("wooden:shield"))
	
	game:add_game_object_to_map(player)
	game:main(player)
	
	game:start_game()
end