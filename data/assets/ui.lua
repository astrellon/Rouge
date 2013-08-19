-- General UI assets
do
	-- Plain pointer cursor
	am.gfx_engine.asset(am.asset.new("cursor")
		:add_texture("data/textures/cursor.png"))
	
	-- Large button used for main menu
	am.gfx_engine.asset(am.asset.new("big_button")
		:add_texture("data/textures/bigButton.png")
		:num_frames(2, 2)
		:scale_nine({
			left_x = 36, right_x = 38, 
			top_y = 26, bottom_y = 38
		}, true))
	
	-- Checkbox asset
	am.gfx_engine.asset(am.asset.new("checkbox")
		:add_texture("data/textures/checkbox.png")
		:num_frames(2, 4))
		
	-- Scrollbar assets
	-- Up glyph
	am.gfx_engine.asset(am.asset.new("scrollbar_up")
		:add_texture("data/textures/scrollBar.png")
		:texture_window_pixel({
			left_x = 0, right_x = 16,
			top_y = 0, bottom_y = 16
		}))
	-- Down glyph
	am.gfx_engine.asset(am.asset.new("scrollbar_down")
		:add_texture("data/textures/scrollBar.png")
		:texture_window_pixel({
			left_x = 0, right_x = 16,
			top_y = 16,	bottom_y = 32
		}))
	-- Back graphic		
	am.gfx_engine.asset(am.asset.new("scrollbar_back")
		:add_texture("data/textures/scrollBarBack.png")
		:scale_nine({top_y = 4, bottom_y = 12}))
	-- Bar graphic
	am.gfx_engine.asset(am.asset.new("scrollbar_bar")
		:add_texture("data/textures/scrollBar.png")
		:texture_window_pixel({left_x = 16, right_x = 32}))
		
	-- UI Panel
	am.gfx_engine.asset(am.asset.new("panel")
		:add_texture("data/textures/uiPanel.png")
		:scale_nine({
			left_x = 8, right_x = 91,
			top_y = 20, bottom_y = 70
		}, true))
		
	-- Paused
	am.gfx_engine.asset(am.asset.new("paused")
		:add_texture("data/textures/paused.png"))
		
	-- Tooltip
	am.gfx_engine.asset(am.asset.new("tooltip")
		:add_texture("data/textures/tooltip.png")
		:scale_nine({
			left_x = 2, right_x = 14,
			top_y = 2, bottom_y = 14
		}))
end