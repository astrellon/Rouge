#include "loading_state.h"

#include <log/logger.h>

#include "engine.h"
#include "dialogue.h"
#include "game.h"
#include "character.h"

namespace am {
namespace game {

	LoadingState::_GameObjectInfo::_GameObjectInfo() :
		hasDestination(false)
	{
	}
	LoadingState::_GameObjectInfo::_GameObjectInfo(GameObject *gameObj) :
		hasDestination(false),
		gameObj(gameObj)
	{
	}

	LoadingState::LoadingState()
	{

	}
	LoadingState::~LoadingState()
	{

	}

	void LoadingState::addMapToLoad(const char *mapName)
	{
		mMapsToLoad[string(mapName)] = true;
	}
	const LoadingState::MapsToLoad &LoadingState::getMapsToLoad() const
	{
		return mMapsToLoad;
	}

	void LoadingState::setCurrentMap(const char *mapName)
	{
		mCurrentMap = mapName;
		addMapToLoad(mapName);
	}
	const char *LoadingState::getCurrentMap() const
	{
		return mCurrentMap.c_str();
	}

	void LoadingState::setMainCharacter(const char *mainChar)
	{
		mMainCharacter = mainChar;
	}
	const char *LoadingState::getMainCharacter() const
	{
		return mMainCharacter.c_str();
	}

	void LoadingState::addGameObjectToMap(GameObject *gameObj, const char *mapName)
	{
		if (!gameObj || !mapName || mapName[0] == '\0')
		{
			return;
		}
		mGameObjectsToMap[string(mapName)].push_back(gameObj);
		addMapToLoad(mapName);
	}
	const LoadingState::GameObjectsToMap &LoadingState::getGameObjectsToMap() const
	{
		return mGameObjectsToMap;
	}

	void LoadingState::setGameId(const char *gameId, GameObject *gameObj)
	{
		if (!gameId || gameId[0] == '\0')
		{
			return;
		}
		if (!gameObj)
		{
			am_log("LOADERR", "Attempting to set a gameId onto a NULL gameObj");
			return;
		}
		mGameObjectInfoMap[string(gameId)].gameObj = gameObj;
	}
	GameObject *LoadingState::getGameId(const char *gameId) const
	{
		if (!gameId || gameId[0] == '\0')
		{
			return NULL;
		}
		auto iter = mGameObjectInfoMap.find(string(gameId));
		if (iter != mGameObjectInfoMap.end())
		{
			return iter->second.gameObj;
		}
		return NULL;
	}

	void LoadingState::setGameObjHasDestination(const char *gameId, bool hasDestination)
	{
		if (!gameId || gameId[0] == '\0')
		{
			return;
		}
		mGameObjectInfoMap[string(gameId)].hasDestination = hasDestination;
	}
	bool LoadingState::getGameObjHasDestination(const char *gameId) const
	{
		if (!gameId || gameId[0] == '\0')
		{
			return false;
		}
		auto iter = mGameObjectInfoMap.find(string(gameId));
		if (iter != mGameObjectInfoMap.end())
		{
			return iter->second.hasDestination;
		}
		return false;
	}
	const LoadingState::GameObjectInfoMap &LoadingState::getGameInfoMap() const
	{
		return mGameObjectInfoMap;
	}

	void LoadingState::setStartDialogue(const char *startDialogue, DialogueComponent *comp)
	{
		if (!startDialogue || startDialogue[0] == '\0' || !comp)
		{
			return;
		}
		mStartDialogueMap[string(startDialogue)].push_back(comp);
	}
	const LoadingState::StartDialogueMap &LoadingState::getStartDialogueMap() const
	{
		return mStartDialogueMap;
	}

	void LoadingState::postLoad(Game *game)
	{
		for (auto iter = mGameObjectInfoMap.begin(); iter != mGameObjectInfoMap.end(); ++iter)
		{
			GameObject *obj = iter->second.gameObj;
			if (obj)
			{
				obj->registerSelf(iter->first.c_str());
			}
		}

		for (auto iter = mMapsToLoad.begin(); iter != mMapsToLoad.end(); ++iter)
		{
			Handle<Map> map(game->getMapLua(iter->first));
			if (!map)
			{
				stringstream ss;
				ss << "Map failed to load '" << iter->first << '\'';
				am_log("LOADERR", ss);
				continue;
			}

			auto objsFind = mGameObjectsToMap.find(iter->first);
			if (objsFind != mGameObjectsToMap.end())
			{
				for (auto objsIter = objsFind->second.begin(); objsIter != objsFind->second.end(); ++objsIter)
				{
					map->addGameObject(objsIter->get());
				}
			}
		}

		for (auto iter = mStartDialogueMap.begin(); iter != mStartDialogueMap.end(); ++iter)
		{
			Dialogue *startDiag = game->getDialogue(iter->first.c_str());
			if (!startDiag)
			{
				stringstream ss;
				ss << "Unable to find start dialogue '" << iter->first << '\'';
				am_log("LOADERR", ss);
				continue;
			}
			for (auto diagIter = iter->second.begin(); diagIter != iter->second.end(); ++diagIter)
			{
				(*diagIter)->setStartDialogue(startDiag);
			}
		}

		if (!mCurrentMap.empty())
		{
			game->setCurrentMap(mCurrentMap.c_str());
		}
		if (!mMainCharacter.empty())
		{
			game->setMainCharacter(dynamic_cast<Character *>(game->getGameObject(mMainCharacter.c_str())));
		}

		for (auto iter = mGameObjectInfoMap.begin(); iter != mGameObjectInfoMap.end(); ++iter)
		{
			Character *obj = dynamic_cast<Character *>(iter->second.gameObj.get());
			if (obj && iter->second.hasDestination)
			{
				obj->recalcDestination();
			}
		}
	}

}
}
