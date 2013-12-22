do
	-- Grass
	am.gfx_engine.asset(am.asset.new("grass")
		:add_texture("data/textures/tiles/cartoonGrass.png")
		--:texture_window_pixel({bottom_y = 64})
		:num_frames(4, 2))
		
	-- Grass Transition
	am.gfx_engine.asset(am.asset.new("grass_transition")
		:add_texture("data/textures/tiles/grass_transition.png")
		:num_frames(16, 2))
		
	-- Dirt
	am.gfx_engine.asset(am.asset.new("dirt")
		:add_texture("data/textures/tiles/grass_dirt_sand.png")
		:texture_window_pixel({top_y = 64, right_x = 64})
		:num_frames(2, 2))
	
	-- Sand
	am.gfx_engine.asset(am.asset.new("sand")
		:add_texture("data/textures/tiles/grass_dirt_sand.png")
		:texture_window_pixel({top_y = 64, left_x = 64})
		:num_frames(2, 2))
		
	-- Swamp
	am.gfx_engine.asset(am.asset.new("swamp")
		:add_texture("data/textures/tiles/swamp.png")
		:frame_rate(1)
		:num_frames(2, 2))
	
	-- Water
	am.gfx_engine.asset(am.asset.new("water")
		:add_texture("data/textures/tiles/water.png")
		:frame_rate(1)
		:num_frames(2, 2))
	
	-- Bricks
	am.gfx_engine.asset(am.asset.new("bricks")
		:add_texture("data/textures/tiles/bricks.png")
		:texture_window_pixel({right_x = 31, bottom_y = 31}))
	
	-- Wood
	am.gfx_engine.asset(am.asset.new("wood")
		:add_texture("data/textures/tiles/wood.png")
		:num_frames(4, 1))
	
end