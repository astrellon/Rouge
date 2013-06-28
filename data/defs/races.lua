do
	local Engine, Game, Character, Stats = import("Engine", "Game", "Character", "Stats")
	local Sprite = import("Sprite")

	local game = Engine.game()
	
	local human = Character.new()
	human:name("Human")
		:age(21)
		:graphic(Sprite.new("characters/npc/front"))
		:gender("male")
		:race("human")
		:add_passible_type("land")
	
	human:add_body_part("left_arm")
	human:add_body_part("right_arm")
	human:add_body_part("head")
	human:add_body_part("torso")
	human:add_body_part("legs")
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