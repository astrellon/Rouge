#pragma once

#include <base/handle.h>
#include <base/return_codes.h>
using namespace am::base;

#include <string>
#include <vector>
using namespace std;

#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ievent_listener.h>
using namespace am::ui;

#include "game_object.h"

namespace am {
namespace game {

	class Character;

	class Door : public GameObject {
	public:

		enum LockType 
		{
			NONE, UNLOCKED, LOCKED, LOCKED_ITEM, LOCKED_SPECIAL, MAX_LOCK_TYPE
		};

		Door();
		Door(const Door &copy);
		~Door();

		void setGraphic(Sprite *graphic, bool calcCameraOffset = true);
		Sprite *getGraphic() const;

		virtual void setDoorType(TileType *type);
		virtual TileType *getDoorType() const;
		
		virtual void update(float dt);
		virtual bool onGameTick(float dt);

		virtual float getWidth();
		virtual float getHeight();

		virtual void receiveDamage(float damage);

		virtual bool canOpenBy(Character *byCharacter);
		virtual void setOpened(bool opened);
		virtual bool isOpened() const;

		virtual void setLock(LockType lock);
		virtual LockType getLock() const;

		virtual void setMap(Map *map);
		
		virtual const char *getGameObjectTypeName() const;
		virtual GameObjectType getGameObjectType() const;

		virtual MapRegion *getDoorRegion() const;

		virtual void onEvent(MapRegionEvent *e);

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		virtual void updateGraphic();

		static const char *getLockTypeName(LockType type);
		static const char *getLockTypeName(int type);
		static LockType getLockType(const char *typeName);
		static LockType getLockType(int type);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;

		bool mOpened;
		LockType mLock;

		Handle<TileType> mDoorType;

		Handle<MapRegion> mDoorRegion;

		virtual void updateTileType();
		virtual void removeFromMap(Map *map);
		virtual void addToMap(Map *map);

		static const char *sLockTypeNames[];
	};

}
}
