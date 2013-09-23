-- Test Wooden Door Asset
do
	am.gfx_engine.asset(am.asset.new("main")
		:add_texture("data/textures/wooden_door.png")
		:frame_rate(0)
		:num_frames(2, 1))
end
