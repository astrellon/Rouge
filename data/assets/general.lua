-- General assets
do
	local asset = am.asset.new("cursor")
	asset:add_texture("data/textures/cursor.png")
	am.gfx_engine.asset(asset)
end