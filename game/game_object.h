#pragma once

#include <gfx/gfx_layer.h>
using namespace am::gfx;

#include <base/handle.h>

#include <vector>
#include <ostream>

#include <game/dialogue.h>
#include <game/tile_type.h>
#include <game/dialogue_component.h>
#include <game/iattribute_data.h>
#include <game/map_region.h>

#include <util/idefinition.h>
using namespace am::util;

#include <sfx/sfx_isource.h>
using namespace am::sfx;

namespace am {
namespace util {
namespace data {
	class IData;
	class Table;
}
}
using namespace am::util;

namespace game {

	class Map;
	class LoadingState;

	class GameObject : public Layer, public IAttributeData, public IDefinition, public IEventListener {
	public:

		enum GameObjectType {
			UNKNOWN_TYPE, GAME_OBJECT, CHARACTER, ITEM, DOOR
		};

		enum DamageType {
			UNKNOWN, ARROW, BLUNT, PIERCING, FALLING, FIRE, COLD, MAGICAL, ACID, MAX_DAMAGE_TYPE
		};

		typedef std::vector< base::Handle<TileType> > PassibleTypeList;
		typedef std::map<std::string, bool> SubjectMap;

		GameObject();
		GameObject(const GameObject &rhs);
		~GameObject();

		virtual void setName(const char *name);
		virtual void setName(const string &name);
		virtual string getName() const;

		virtual void setDescription(const char *description);
		virtual void setDescription(const string &description);
		virtual string getDescription() const;

		virtual void setLocation(float x, float y, bool setDraw = true);
		virtual float getLocationX() const;
		virtual float getLocationY() const;

		virtual bool setGridLocation(int x, int y, bool setDraw = true);
		virtual int getGridLocationX() const;
		virtual int getGridLocationY() const;

		virtual bool setGridLocationF(float x, float y, bool setDraw = true);
		virtual float getGridLocationXF() const;
		virtual float getGridLocationYF() const;

		virtual void setDialogueComp(DialogueComponent *comp, bool setAttached = true);
		virtual DialogueComponent *getDialogueComp() const;

		virtual bool move(float x, float y);
		virtual bool moveGrid(int x, int y);

		virtual void setFixedToGrid(bool fixed);
		virtual bool isFixedToGrid() const;

		virtual void setOnlyOnPassable(bool only);
		virtual bool isOnlyOnPassable() const;

		// The update function relates to changes in real time, the dt
		// value will be in milliseconds since the last update.
		virtual void update(float dt);
		// The onGameTick function relates to one in-game tick.
		// If game tick returns true then it is okay for the game to
		// move onto the next game object.
		// Otherwise it will wait until it is told that it is the next
		// game objects game tick.
		virtual bool onGameTick(float dt);

		virtual void setCameraOffset(float x, float y);
		virtual float getCameraOffsetX() const;
		virtual float getCameraOffsetY() const;

		virtual void setMap(Map *map);
		virtual Map *getMap() const;

		virtual void setOriginalMap(Map *map);
		virtual Map *getOriginalMap() const;

		virtual void receiveDamage(float damage);

		virtual void addPassibleType(TileType *tileType);
		virtual void removePassibleType(TileType *tileType);
		virtual void removeAllPassibleTypes();
		virtual bool hasPassibleType(TileType *tileType) const;
		virtual PassibleTypeList &getPassibleTypes();
		virtual const PassibleTypeList &getPassibleTypes() const;

		virtual const char *getGameObjectTypeName() const;
		virtual GameObjectType getGameObjectType() const;

		virtual bool setGameId(const char *id);
		virtual const char *getGameId() const;
		virtual void registerSelf(const char *id);

		virtual ISource *getSource(bool create = true);
		virtual void setSource(ISource *source);

		virtual float distanceTo(GameObject *obj) const;
		virtual float distanceToGrid(GameObject *obj) const;

		// Returns true if the interaction prevents movement.
		// If byMovement is true then the interaction is from the game object moving to
		// interact with the game object. Otherwise it's from the UI (ie clicking).
		virtual bool interactWith(GameObject *interacter, bool byMovement);

		// Layer methods
		virtual bool interactWithLayer() const;

		virtual void addToMapRegion(MapRegion *region);
		virtual void removeFromMapRegion(MapRegion *region);
		virtual bool isInMapRegion(MapRegion *region) const;
		virtual const MapRegion::MapRegionList &getMapRegions() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		static const char *getDamageTypeName(DamageType type);
		static const char *getDamageTypeName(int type);
		static DamageType getDamageType(const char *typeName);
		static DamageType getDamageType(int type);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		std::string mGameId;

		bool mFixedToGrid;
		bool mOnlyOnPassable;

		float mLocationX;
		float mLocationY;

		float mCameraOffsetX;
		float mCameraOffsetY;

		PassibleTypeList mPassibleTypes;

		std::string mName;
		std::string mDescription;

		Map *mMap;
		Map *mOriginalMap;

		base::Handle<DialogueComponent> mDialogueComp;
		base::Handle<ISource> mSoundSource;

		MapRegion::MapRegionList mMapRegions;

		void applyToSource();

		virtual void postRender(float dt);

		static const char *sDamageTypeNames[];
	};

	typedef std::vector<base::Handle<GameObject> > ObjectList;

}
}
