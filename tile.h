#pragma once

#include <string>
using namespace std;

#include "gfx/gfx_asset.h"
using namespace am::gfx;

#include "json_value.h"
#include "handle.h"
using namespace am::util;

namespace am {
namespace base {

	class Tile {
	public:
		Tile(const char *name);
		Tile(const char *name, const char *fullName);
		~Tile();

		string getName();
		const string &getName() const;

		string getFullName();
		const string &getFullName() const;
		void setFullName(const char *name);

		Asset *getGraphicAsset();
		void setGraphicAsset(Asset *asset);

		void loadDef(JsonObject &value);
	
	protected:
	
		string mName;
		string mFullName;
		Handle<Asset> mGraphic;

		int parseDef(const char *name, JsonValue &value);
	};

}
}
