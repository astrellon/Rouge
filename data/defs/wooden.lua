do
	local game = am.engine.game()
	
	-- Wooden Sword
	local item = am.item.new()
		:item_fullname("Sword", "Wooden")
		:item_type("sword")
		:graphic(am.sprite.new("items/sword:main"), true)
		:ground_graphic(am.sprite.new("items/sword:ground"))
		:inventory_size(1, 2)
		:description("This sword be wooden!")
		:add_body_type("hand")
		:item_value(20)
		
	local stats = item:stat_modifiers()
	stats:add("minDamage", 2, "+")
	stats:add("maxDamage", 4, "+")
	game:item_def("sword", item)
	
	-- Wooden Shield
	item = am.item.new()
		:item_fullname("Shield", "Wooden")
		:item_type("shield")
		:graphic(am.sprite.new("items/shield:main"), true)
		:inventory_size(2, 2)
		:description("This shield be wooden!")
		:add_body_type("hand")
		:item_value(15)
		
	stats = item:stat_modifiers()
	stats:add("armour", 3, "+")
	game:item_def("shield", item)
	
	-- Paper message scroll
	item = am.item.new()
		:item_fullname("Scroll", "Paper")
		:item_type("message_scroll")
		:graphic(am.sprite.new("items/scroll:main"), true)
		:inventory_size(2, 1)
		:description("This scroll should have some writing on it, but it doesn't :|")
	game:item_def("scroll", item)
end