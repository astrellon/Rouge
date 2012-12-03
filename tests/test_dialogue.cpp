#include "test_dialogue.h"

#include <game/character.h>
#include <game/dialogue.h>
#include <game/engine.h>
#include <game/game.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestDialogue::testSimple() 
	{
		Game *game = new Game();
		Engine::getEngine()->setCurrentGame(game);

		Dialogue testDiag("diag1", "Hello there, my name is", "Greetings", "greeting", Dialogue::UNLOCK_NONE);
		equalsStr("diag1", testDiag.getId());
		equalsStr("Hello there, my name is", testDiag.getText());
		equalsStr("Greetings", testDiag.getTitle());
		equalsStr("greeting", testDiag.getSubject());
		assert(Dialogue::UNLOCK_NONE == testDiag.getUnlockFlag());

		game->removeAllDialogue();
		game->addDialogue(&testDiag);

		Dialogue testDiag2("diag2", "My name is Melli", "Name", "name", Dialogue::UNLOCK_LOCKED);
		game->addDialogue(&testDiag2);

		Handle<Character> testNPC(new Character());
		testNPC->setDialogueComp(new DialogueComponent());
		testNPC->getDialogueComp()->setDialogueAvailable("diag1");
		testNPC->getDialogueComp()->setDialogueAvailable("diag2");

		Handle<Character> testPlayer(new Character());
		testPlayer->setDialogueComp(new DialogueComponent());

		vector<Dialogue *> dialogues;
		game->getAvailableDialogues(dialogues, testPlayer, testNPC);

		equals(1u, dialogues.size());
		assert(dialogues[0] == &testDiag);

		dialogues.clear();
		equals(0u, dialogues.size());

		testPlayer->getDialogueComp()->setSubjectLock("name");

		game->getAvailableDialogues(dialogues, testPlayer, testNPC);
		equals(2u, dialogues.size());

		return true;
	}


}
}
