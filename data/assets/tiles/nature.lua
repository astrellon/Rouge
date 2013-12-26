do
	-- Grass
	am.gfx_engine.asset(am.asset.new("grass")
		:texture("data/textures/tiles/cartoonGrass.png")
		--:texture_window_pixel({bottom_y = 64})
		:num_frames(4, 2))
		
	-- Grass Transition
	am.gfx_engine.asset(am.asset.new("grass_transition")
		:texture("data/textures/tiles/cartoonGrassTransition.png")
		:num_frames(16, 2))
		
	-- Dirt
	am.gfx_engine.asset(am.asset.new("dirt")
		:texture("data/textures/tiles/cartoonDirt.png")
		--:texture_window_pixel({top_y = 64, right_x = 64})
		:num_frames(2, 2))
	
	-- Sand
	am.gfx_engine.asset(am.asset.new("sand")
		:texture("data/textures/tiles/cartoonSand.png")
		--:texture_window_pixel({top_y = 64, left_x = 64})
		:num_frames(2, 2))

    -- Sand Transition
    am.gfx_engine.asset(am.asset.new("sand_transition")
        :texture("data/textures/tiles/cartoonSandTransition.png")
        :num_frames(16,2))
		
	-- Swamp
	am.gfx_engine.asset(am.asset.new("swamp")
		:texture("data/textures/tiles/swamp.png")
		:frame_rate(1)
		:num_frames(2, 2))
	
	-- Water
	am.gfx_engine.asset(am.asset.new("water")
		:texture("data/textures/tiles/cartoonWater.png")
		:frame_rate(4)
		:num_frames(4, 2))
	
	-- Bricks
	am.gfx_engine.asset(am.asset.new("bricks")
		:texture("data/textures/tiles/bricks.png")
		:texture_window_pixel({right_x = 31, bottom_y = 31}))
	
	-- Wood
	am.gfx_engine.asset(am.asset.new("wood")
		:texture("data/textures/tiles/wood.png")
		:num_frames(4, 1))
	
end
