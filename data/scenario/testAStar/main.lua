-- Test AStar Scenario
-- 
return {
	newGame = function(game)
		am_log("NEW GAME!!!!!")
		map = game:map("testAStar")
		if (map ~= nil) then
			game:current_map(map)
		end
		
		player = Character.from_def("races:human", "melli")
		player:name("Melanine")
			:graphic(Sprite.new("characters/mainChar/front"))
			:gender("female")
		
		game:add_game_object_to_map(player)
		game:main(player)
		
		game:start_game()
	end,
	loadGame = function(game)
		game:start_game()
		
		player = game:main()
	end
}