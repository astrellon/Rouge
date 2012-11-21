#pragma once

#include <vector>
using std::vector;

#include <game/component.h>

namespace am {
namespace game {

	class TileType;
	class GameObjectComponent;

	class LocationComponent : public Component {
	public:

		typedef vector<TileType *> PassibleTypeList;

		LocationComponent();
		~LocationComponent();

		void setLocation(GameObjectComponent *obj, float x, float y) const;
		bool setGridLocation(GameObjectComponent *obj, int x, int y) const;

		void setFixedToGrid(bool fixed);
		bool isFixedToGrid() const;

		void setOnlyOnPassable(bool only);
		bool isOnlyOnPassable() const;

		void setCameraOffset(float x, float y);
		float getCameraOffsetX() const;
		float getCameraOffsetY() const;

		void addPassibleType(TileType *tileType);
		void removePassibleType(TileType *tileType);
		void removeAllPassibleTypes();
		bool hasPassibleType(TileType *tileType) const;
		PassibleTypeList &getPassibleTypes();
		const PassibleTypeList &getPassibleTypes() const;

		virtual const char *getCompName() const;
		virtual ComponentKey getCompKey() const;

		static ComponentKey getKey();
		static const char *getName();

	protected:

		bool mFixedToGrid;
		bool mOnlyOnPassable;

		float mCameraOffsetX;
		float mCameraOffsetY;

		PassibleTypeList mPassibleTypes;

	};

}
}
