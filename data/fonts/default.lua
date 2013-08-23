-- Default fonts
do
	am.gfx_engine.font(am.font.new("basic")
		:asset("fonts:basic")
		:fixed_width(false)
		:kerning(1)
		:leading(1)
		:space_width(6)
		:tab_width(24)
		:process())
	
	am.gfx_engine.font(am.font.new("arial")
		:asset("fonts:arial")
		:fixed_width(false)
		:kerning(0)
		:leading(1)
		:space_width(8)
		:tab_width(32)
		:process())
end