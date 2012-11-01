-- Dialogues
dialogues = {}
local diag = {}
diag.id = "diag1"
diag.text = "Hello there ${main name} <? #='HART' @='diag2'>Next</?>"
table.insert(dialogues, diag)

diag = {}
diag.id = "diag2"
diag.text = "My name is <? #='name' @='diag3'>Melli</?> How are you today?"
diag.subject = "How Are"
diag.unlock = "subject"
table.insert(dialogues, diag)

diag = {}
diag.id = "diag3"
diag.text = "Melli is my name"
diag.subject = "name"
diag.unlock = "subject"
table.insert(dialogues, diag)