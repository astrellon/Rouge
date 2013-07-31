#include "test_path_tokeniser.h"

#include <tests/asserts.h>

#include <util/path_tokeniser.h>
using namespace am::util;

namespace am {
namespace tests {

	bool TestPathTokeniser::testSimple() {
		PathTokeniser tokeniser("C:\\path\\\\to/folder//");
		am_equalsStr("C:", tokeniser.nextToken());
		am_equalsStr("path", tokeniser.nextToken());
		am_equalsStr("to", tokeniser.nextToken());
		am_equalsStr("folder", tokeniser.nextToken());
		const char *token = tokeniser.nextToken();
		assert(nullptr == token);

		PathTokeniser tokeniser2("/home/user//games//");
		am_equalsStr("/", tokeniser2.nextToken());
		am_equalsStr("home", tokeniser2.nextToken());
		am_equalsStr("user", tokeniser2.nextToken());
		am_equalsStr("games", tokeniser2.nextToken());
		token = tokeniser2.nextToken();
		assert(nullptr == token);

		PathTokeniser tokeniser3("\\\\server\\folder//games//");
		am_equalsStr("\\\\", tokeniser3.nextToken());
		am_equalsStr("server", tokeniser3.nextToken());
		am_equalsStr("folder", tokeniser3.nextToken());
		am_equalsStr("games", tokeniser3.nextToken());
		token = tokeniser3.nextToken();
		assert(nullptr == token);

		PathTokeniser tokeniser4("saves/game1/main.lua");
		am_equalsStr("saves", tokeniser4.nextToken());
		am_equalsStr("game1", tokeniser4.nextToken());
		am_equalsStr("main.lua", tokeniser4.nextToken());
		token = tokeniser4.nextToken();
		assert(nullptr == token);

		PathTokeniser tokeniser5("folder");
		am_equalsStr("folder", tokeniser5.nextToken());
		token = tokeniser5.nextToken();
		assert(nullptr == token);
		return true;
	}

}
}
