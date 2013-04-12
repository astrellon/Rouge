#include "test_path_tokeniser.h"

#include <tests/asserts.h>

#include <util/path_tokeniser.h>
using namespace am::util;

namespace am {
namespace tests {

	bool TestPathTokeniser::testSimple() {
		PathTokeniser tokeniser("C:\\path\\\\to/folder//");
		equalsStr("C:", tokeniser.nextToken());
		equalsStr("path", tokeniser.nextToken());
		equalsStr("to", tokeniser.nextToken());
		equalsStr("folder", tokeniser.nextToken());
		const char *token = tokeniser.nextToken();
		assert(NULL == token);

		PathTokeniser tokeniser2("/home/user//games//");
		equalsStr("/", tokeniser2.nextToken());
		equalsStr("home", tokeniser2.nextToken());
		equalsStr("user", tokeniser2.nextToken());
		equalsStr("games", tokeniser2.nextToken());
		token = tokeniser2.nextToken();
		assert(NULL == token);

		PathTokeniser tokeniser3("\\\\server\\folder//games//");
		equalsStr("\\\\", tokeniser3.nextToken());
		equalsStr("server", tokeniser3.nextToken());
		equalsStr("folder", tokeniser3.nextToken());
		equalsStr("games", tokeniser3.nextToken());
		token = tokeniser3.nextToken();
		assert(NULL == token);

		PathTokeniser tokeniser4("saves/game1/main.lua");
		equalsStr("saves", tokeniser4.nextToken());
		equalsStr("game1", tokeniser4.nextToken());
		equalsStr("main.lua", tokeniser4.nextToken());
		token = tokeniser4.nextToken();
		assert(NULL == token);

		PathTokeniser tokeniser5("folder");
		equalsStr("folder", tokeniser5.nextToken());
		token = tokeniser5.nextToken();
		assert(NULL == token);
		return true;
	}

}
}
