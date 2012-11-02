#include "test_dialogue.h"

#include <game/character.h>
#include <game/dialogue.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestDialogue::testSimple() {
		Dialogue testDiag("diag1", "Hello there, my name is", "Greetings", "greeting", Dialogue::NONE);
		equalsStr("diag1", testDiag.getId());
		equalsStr("Hello there, my name is", testDiag.getText());
		equalsStr("Greetings", testDiag.getTitle());
		equalsStr("greeting", testDiag.getSubject());
		assert(Dialogue::NONE == testDiag.getUnlockFlag());

		Dialogue::removeAllDialogue();
		Dialogue::addDialogue(&testDiag);

		Dialogue testDiag2("diag2", "My name is Melli", "Name", "name", Dialogue::LOCKED);
		Dialogue::addDialogue(&testDiag2);

		Handle<Character> testNPC(new Character());
		testNPC->setDialogueAvailable("diag1");
		testNPC->setDialogueAvailable("diag2");

		Handle<Character> testPlayer(new Character());

		vector<Dialogue *> dialogues;
		Dialogue::getAvailableDialogues(dialogues, testPlayer, testNPC);

		equals(1u, dialogues.size());
		assert(dialogues[0] == &testDiag);

		dialogues.clear();
		equals(0u, dialogues.size());

		testPlayer->setSubjectLock("name");

		Dialogue::getAvailableDialogues(dialogues, testPlayer, testNPC);
		equals(2u, dialogues.size());

		return true;
	}


}
}
