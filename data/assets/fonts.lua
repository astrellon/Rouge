-- Assets for fonts
do
	local asset = am.asset.new("basic")
		:add_texture("data/textures/fontBasic.png")
	am.gfx_engine.asset(asset)
	
	asset = am.asset.new("arial")
		:add_texture("data/textures/fontArial.png")
		:texture_window_pixel({right_x = 256})
	am.gfx_engine.asset(asset)
end