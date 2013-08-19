do
	local game = am.engine.game()
	
	local human = am.character.new()
	human:name("Human")
		:age(21)
		:graphic(am.sprite.new("characters/human:male"))
		:gender("male")
		:race("human")
		:add_passible_type("land")
	
	human:add_body_part(am.body_part.new("left_hand", "hand"):weapon_part(true))
	human:add_body_part(am.body_part.new("right_hand", "hand"):weapon_part(true))
	human:add_body_part(am.body_part.new("head", "head"))
	human:add_body_part(am.body_part.new("torso", "torso"))
	human:add_body_part(am.body_part.new("legs", "legs"))
	human:stats()
		:base_stat("health", 12)
		:base_stat("maxHealth", 12)
		:base_stat("strength", 4)
		:base_stat("dexterity", 3)
		:base_stat("constitution", 4)
		:base_stat("arcane", 1)
		:base_stat("divine", 2)
		:base_stat("maxDamage", 4)
		:base_stat("minDamage", 3)
		:base_stat("armour", 2)
		:base_stat("speed", 1)
	
	game:char_def("human", human)
end
