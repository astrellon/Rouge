#pragma once

#include <string>
using namespace std;

#include <gfx/gfx_asset.h>
using namespace am::gfx;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

#include <util/json_value.h>
using namespace am::util;

namespace am {
namespace game {

	class TileSet;

	class Tile : public IManaged {
	public:
		Tile(const char *name);
		Tile(const char *name, const char *fullName);
		~Tile();

		string getName() const;
		void setName(const char *name);

		string getFullName() const;
		void setFullName(const char *name);

		string getDescription() const;
		void setDescription(const char *description);

		Asset *getGraphicAsset();
		void setGraphicAsset(Asset *asset);

		void setTileSet(TileSet *tileSet);
		TileSet *getTileSet() const;

		void loadDef(JsonValue value);
	
	protected:
	
		string mName;
		string mFullName;
		TileSet *mTileSet;
		Handle<Asset> mGraphic;
		string mDescription;

	};

}
}
