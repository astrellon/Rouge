do
	local Engine, Game, Character, Stats = import("Engine", "Game", "Character", "Stats")
	local Sprite = import("Sprite")

	local game = Engine.game()

	local npc = Character.from_def("races:human")
	npc:name("Male Townsman")
		:age(21)
	game:char_def("male1", npc)

	npc = Character.from_def("races:human")
	npc:name("Male Townsman")
		:age(22)
	game:char_def("male2", npc)

	npc = Character.from_def("races:human")
	npc:name("Female Townsman")
		:age(18)
		:gender("female")
	game:char_def("female1", npc)

	npc = Character.from_def("races:human")
	npc:name("Female Townsman")
		:age(21)
		:gender("female")
	game:char_def("female2", npc)
end