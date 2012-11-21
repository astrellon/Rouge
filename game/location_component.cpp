#include "location_component.h"

#include <game/tile_type.h>
#include <game/game_object_component.h>
using am::game::TileType;
using am::game::GameObjectComponent;

namespace am {
namespace game {

	LocationComponent::LocationComponent() :
		mFixedToGrid(false),
		mOnlyOnPassable(false)
	{
		
	}
	LocationComponent::~LocationComponent()
	{

	}

	
	const char *LocationComponent::getCompName() const
	{
		return getName();
	}
	Component::ComponentKey LocationComponent::getCompKey() const
	{
		return getCompKey();
	}

	Component::ComponentKey LocationComponent::getKey()
	{
		return 0;
	}
	const char *LocationComponent::getName()
	{
		return "LocationComponent";
	}
	
}
}
