-- Test Sword Ground Asset
do
	am.gfx_engine.asset(am.asset.new("main")
		:add_texture("data/textures/items/sword.png")
		:texture_window_pixel({top_y = 0, bottom_y = 48}))
		
	am.gfx_engine.asset(am.asset.new("ground")
		:add_texture("data/textures/items/swordGround.png"))
end
