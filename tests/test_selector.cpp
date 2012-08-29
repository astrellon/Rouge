#include "test_selector.h"

#include <tests/asserts.h>

#include <util/selector.h>
using namespace am::util;

namespace am {
namespace tests {

	bool TestSelector::testSimple() {
		
		Selector toMatch;
		toMatch.setId("char");
		
		Selector selector1;
		int match = selector1.match(toMatch);
		equals(-1, match);

		selector1.setId("char");
		match = selector1.match(toMatch);
		equals(2, match);

		selector1.setAttribute("gender", "male");
		match = selector1.match(toMatch);
		equals(2, match);

		toMatch.setAttribute("gender", "male");
		match = selector1.match(toMatch);
		equals(5, match);

		toMatch.setAttribute("gender", "female");
		match = selector1.match(toMatch);
		equals(-4, match);

		return true;
	}

	bool TestSelector::testComplex()
	{
		Selector toMatch("char");
		toMatch.setAttribute("gender", "male");
		toMatch.setAttribute("class", "knight");
		toMatch.setAttribute("age", "20");

		Selector toMatch2("char");
		toMatch2.setAttribute("gender", "male");
		toMatch2.setAttribute("class", "knight");
		toMatch2.setAttribute("age", "20");
		toMatch2.setAttribute("height", "1m");

		Selector npc("char");
		npc.setAttribute("gender", "male");
		npc.setAttribute("class", "knight");
		npc.setAttribute("age", "20");

		int match = npc.match(toMatch);
		int match2 = npc.match(toMatch2);
		equals(11, match);
		equals(-2, match2);

		npc.setAttribute("height", "1m");
		match = npc.match(toMatch);
		match2 = npc.match(toMatch2);
		equals(11, match);
		equals(14, match2);

		return true;
	}

}
}
