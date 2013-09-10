-- Human Character Sprite
do
	-- General characters
	am.gfx_engine.asset(am.asset.new("male")
		:add_texture("data/textures/characters/front.png")
		:texture_window_pixel({right_x = 32}))
	am.gfx_engine.asset(am.asset.new("male2")
		:add_texture("data/textures/characters/front.png")
		:texture_window_pixel({left_x = 32, right_x = 64}))
	am.gfx_engine.asset(am.asset.new("male3")
		:add_texture("data/textures/characters/front.png")
		:texture_window_pixel({left_x = 65, right_x = 96}))
	am.gfx_engine.asset(am.asset.new("male_elder")
		:add_texture("data/textures/characters/front.png")
		:texture_window_pixel({left_x = 97}))
		
	-- Special characters
	am.gfx_engine.asset(am.asset.new("kaswe")
		:add_texture("data/textures/characters/kaswe.png"))
end
