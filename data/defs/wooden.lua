do
	local game = am.engine.game()
	
	-- Wooden Sword
	local item = am.item.new()
	item:item_fullname("Sword", "Wooden")
		:item_type("sword")
		:graphic(am.sprite.new("items/sword:main"))
		:ground_graphic(am.sprite.new("items/sword:ground"))
		:inventory_size(1, 2)
		
	local stats = item:stat_modifiers_self()
	stats:add("minDamage", 2, "+")
	stats:add("maxDamage", 4, "+")
	game:item_def("sword", item)
	
	-- Wooden Shield
	item = am.item.new()
	item:item_fullname("Shield", "Wooden")
		:item_type("shield")
		:graphic(am.sprite.new("items/shield:main"))
		:inventory_size(2, 2)
		
	stats = item:stat_modifiers()
	stats:add("armour", 3, "+")
	game:item_def("shield", item)
end