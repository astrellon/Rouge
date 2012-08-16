#pragma once

#include <gfx/gfx_layer.h>
using namespace am::gfx;

#include <base/handle.h>
using namespace am::base;

#include <vector>
using namespace std;

namespace am {
namespace game {

	class Map;
	class TileType;

	class GameObject : public Layer {
	public:

		typedef vector<TileType *> PassibleTypeList;

		GameObject();
		~GameObject();

		virtual void setName(const char *name);
		virtual void setName(const string &name);
		virtual string getName() const;

		virtual void setLocation(float x, float y, bool setDraw = true);
		virtual float getLocationX() const;
		virtual float getLocationY() const;

		virtual bool setGridLocation(int x, int y, bool setDraw = true);
		virtual int getGridLocationX() const;
		virtual int getGridLocationY() const;

		virtual void move(float x, float y);
		virtual void moveGrid(int x, int y);

		virtual void setFixedToGrid(bool fixed);
		virtual bool isFixedToGrid() const;

		virtual void setOnlyOnPassable(bool only);
		virtual bool isOnlyOnPassable() const;

		virtual void update(float dt);

		virtual void setCameraOffset(float x, float y);
		virtual float getCameraOffsetX() const;
		virtual float getCameraOffsetY() const;

		virtual void setMap(Map *map);
		virtual Map *getMap();

		virtual void addPassibleType(TileType *tileType);
		virtual void removePassibleType(TileType *tileType);
		virtual void removeAllPassibleTypes();
		virtual bool hasPassibleType(TileType *tileType) const;
		virtual PassibleTypeList &getPassibleTypes();
		virtual const PassibleTypeList &getPassibleTypes() const;

		virtual const char *getGameObjectTypeName() const;

		virtual int getGameId() const;

		static int nextGameId();
		static GameObject *getByGameId(int id);

	protected:

		bool mFixedToGrid;
		bool mOnlyOnPassable;

		int mGameId;

		float mLocationX;
		float mLocationY;

		float mCameraOffsetX;
		float mCameraOffsetY;

		PassibleTypeList mPassibleTypes;

		string mName;

		Map *mMap;

		virtual void setGameId(int id);

		static void addGameObject(GameObject *obj);
		static void removeGameObject(GameObject *obj);

		static int sGameIdCount;
		typedef map<int, GameObject *> GameObjectIdMap;
		static GameObjectIdMap sGameObjects;

	};

	typedef vector<Handle<GameObject> > ObjectList;

}
}
