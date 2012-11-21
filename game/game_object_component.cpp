#include "game_object_component.h"

namespace am {
namespace game {

	GameObjectComponent::GameObjectComponent(const char *id)
	{
		if (id != NULL && id[0] != '\0')
		{
			mId = id;
		}
		else
		{
			// Create random id.
		}
	}
	GameObjectComponent::~GameObjectComponent()
	{

	}

	const char *GameObjectComponent::getId() const
	{
		return mId.c_str();
	}
	void GameObjectComponent::setId(const char *id)
	{
		mId = id;
	}

	const GameObjectComponent::ComponentArray &GameObjectComponent::getComponents() const
	{
		return mComponents;
	}
	Component *GameObjectComponent::getComponent(Component::ComponentKey key) const
	{
		for (size_t i = 0; i < mComponents.size(); ++i)
		{
			if (mComponents[i]->getCompKey() == key)
			{
				return mComponents[i];
			}
		}
		return NULL;
	}
	
}
}
