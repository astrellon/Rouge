do
	local Engine, Game, Item, StatModifiers = import("Engine", "Game", "Item", "StatModifiers")
	local Sprite = import("Sprite")

	local game = Engine.game()
	
	-- Wooden Sword
	local item = Item.new()
	item:item_fullname("Sword", "Wooden")
		:item_type("sword")
		:graphic(Sprite.new("items/sword"))
		:ground_graphic(Sprite.new("items/swordGround"))
		
	local stats = item:stat_modifiers()
	stats:add("minDamage", 2, "+")
	stats:add("maxDamage", 4, "+")
	game:item_def("sword", item)
	
	-- Wooden Shield
	item = Item.new()
	item:item_fullname("Shield", "Wooden")
		:item_type("shield")
		:graphic(Sprite.new("items/shield"))
		
	stats = item:stat_modifiers()
	stats:add("armour", 3, "+")
	game:item_def("shield", item)
end