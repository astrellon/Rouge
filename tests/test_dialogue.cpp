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
		game::Game *game = new game::Game();
		game::Engine::getEngine()->setCurrentGame(game);

		game::Dialogue *testDiag = new game::Dialogue("diag1", "Hello there, my name is", "Greetings", "greeting", game::Dialogue::UNLOCK_NONE);
		am_equalsStr("diag1", testDiag->getId());
		am_equalsStr("Hello there, my name is", testDiag->getText());
		am_equalsStr("Greetings", testDiag->getTitle());
		am_equalsStr("greeting", testDiag->getSubject());
		assert(game::Dialogue::UNLOCK_NONE == testDiag->getUnlockFlag());

		game->removeAllDialogue();
		game->addDialogue(testDiag);

		game::Dialogue *testDiag2 = new game::Dialogue("diag2", "My name is Melli", "Name", "name", game::Dialogue::UNLOCK_LOCKED);
		game->addDialogue(testDiag2);

		base::Handle<game::Character> testNPC(new game::Character());
		testNPC->setDialogueComp(new DialogueComponent());
		testNPC->getDialogueComp()->setDialogueAvailable("diag1");
		testNPC->getDialogueComp()->setDialogueAvailable("diag2");

		base::Handle<game::Character> testPlayer(new game::Character());
		testPlayer->setDialogueComp(new DialogueComponent());

		std::vector<game::Dialogue *> dialogues;
		game->getAvailableDialogues(dialogues, testPlayer, testNPC);

		am_equals(1u, dialogues.size());
		assert(dialogues[0] == testDiag);

		dialogues.clear();
		am_equals(0u, dialogues.size());

		testPlayer->getDialogueComp()->setSubjectLock("name");

		game->getAvailableDialogues(dialogues, testPlayer, testNPC);
		am_equals(2u, dialogues.size());

		return true;
	}

}
}
