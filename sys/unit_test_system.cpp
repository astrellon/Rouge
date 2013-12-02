#include "unit_test_system.h"

#include <sys/os_system.h>

#include <gfx/gfx_engine.h>
#include <gfx/gfx_text_list.h>
#include <gfx/gfx_asset.h>

#include <tests/test_base.h>
#include <tests/test_mouse_manager.h>
#include <tests/test_map.h>
#include <tests/test_character.h>
#include <tests/test_lua.h>
#include <tests/test_lua_quest.h>
#include <tests/test_lua_event_listener.h>
#include <tests/test_lua_character.h>
#include <tests/test_lua_stats.h>
#include <tests/test_lua_stat_modifiers.h>
#include <tests/test_lua_item.h>
#include <tests/test_lua_inventory.h>
#include <tests/test_lua_tile_type.h>
#include <tests/test_lua_tile.h>
#include <tests/test_lua_tile_set.h>
#include <tests/test_lua_engine.h>
#include <tests/test_levelable.h>
#include <tests/test_dialogue.h>
#include <tests/test_lua_dialogue.h>
#include <tests/test_event_interface.h>
#include <tests/test_lua_map.h>
#include <tests/test_item.h>
#include <tests/test_path_tokeniser.h>
#include <tests/test_util_data.h>
#include <tests/test_lua_data_table.h>
#include <tests/test_utils.h>
#include <tests/test_body_parts.h>
#include <tests/test_lua_asset.h>
#include <tests/test_store.h>

namespace am {
namespace sys {

	UnitTestSystem *UnitTestSystem::sUnitTestSystem = nullptr;

	UnitTestSystem *UnitTestSystem::createUnitTestSystem(OsSystem *linked, Engine *engine)
	{
		sGameSystem = sUnitTestSystem = new UnitTestSystem(linked, engine);
		return sUnitTestSystem;
	}
	UnitTestSystem *UnitTestSystem::getUnitTestSystem()
	{
		return sUnitTestSystem;
	}

	UnitTestSystem::UnitTestSystem(OsSystem *linked, Engine *engine) :
		GameSystem(linked, engine)
	{

	}
	UnitTestSystem::~UnitTestSystem()
	{
	}

	void UnitTestSystem::init()
	{
		GameSystem::init();
		
		GfxEngine *gfxEngine = GfxEngine::getEngine();
		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());

		mDebugConsole->setMaxEntries(100);

		mDebugConsole->setVisible(true);

		int suiteCount = 0;

		runSuite(tests::TestBase);
		runSuite(tests::TestMouseManager);
		runSuite(tests::TestMap);
		runSuite(tests::TestCharacter);
		runSuite(tests::TestLua);
		runSuite(tests::TestLuaQuest);
		runSuite(tests::TestLuaEventListener);
		runSuite(tests::TestLuaCharacter);
		runSuite(tests::TestLuaStats);
		runSuite(tests::TestLuaStatModifiers);
		runSuite(tests::TestLuaItem);
		runSuite(tests::TestLuaInventory);
		runSuite(tests::TestLuaTileType);
		runSuite(tests::TestLuaTile);
		runSuite(tests::TestLuaTileSet);
		runSuite(tests::TestLuaEngine);
		runSuite(tests::TestLevelable);
		runSuite(tests::TestDialogue);
		runSuite(tests::TestLuaDialogue);
		runSuite(tests::TestEventInterface);
		runSuite(tests::TestLuaMap);
		runSuite(tests::TestItem);
		runSuite(tests::TestPathTokeniser);
		runSuite(tests::TestUtilData);
		runSuite(tests::TestLuaDataTable);
		runSuite(tests::TestUtils);
		runSuite(tests::TestBodyParts);
		runSuite(tests::TestLuaAsset);
		runSuite(tests::TestStore);

		const std::vector<std::string> &failed = tests::TestSuite::getFailedTests();
		if (failed.size() > 0)
		{
			std::stringstream ss;
			ss << "-------------------\nFailed tests: ";
			std::vector<std::string>::const_iterator iter;
			bool first = true;
			for (iter = failed.begin(); iter != failed.end(); ++iter)
			{
				if (!first)
				{
					ss << ", ";
				}
				first = false;
				ss << iter->c_str();
			}
			ss << "\n-------------------";
			am_log("FAILED", ss);
		}
		else
		{
			std::stringstream ss;
			ss << "-------------------\nAll ";
			ss << suiteCount << " tests passed!";
			ss << "\n-------------------";
			am_log("PASSED", ss);
		}
	}
	
	void UnitTestSystem::reshape(int width, int height)
	{
		GameSystem::reshape(width, height);

		float fwidth = static_cast<float>(width);
		float fheight = static_cast<float>(height);
	}

	void UnitTestSystem::onKeyUp(ui::Keyboard::Key key)
	{
		GameSystem::onKeyUp(key);
	}

}
}
