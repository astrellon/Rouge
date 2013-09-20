-- Test Sword Ground Asset
do
	am.gfx_engine.asset(am.asset.new("main")
		:add_texture("data/textures/items/sword.png"))
		
	am.gfx_engine.asset(am.asset.new("ground")
		:add_texture("data/textures/items/swordGround.png"))
end
