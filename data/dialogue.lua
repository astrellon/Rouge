-- Dialogues
Dialogue = import("Dialogue")

diag = Dialogue.new("diag1",
	"Hello there ${char main} <? #='HART' @='diag2'>Next</?>")
Dialogue.add_dialogue(diag)

diag = Dialogue.new("diag2",
	"My name is <? #='name' @='diag3'>Melli</?> How are you today?",
	"How Are", "how_are", "locked")
Dialogue.add_dialogue(diag)

diag = Dialogue.new("diag3", "Melli is my name, <x>good bye</x>", "Name", "name", "locked")
Dialogue.add_dialogue(diag)
