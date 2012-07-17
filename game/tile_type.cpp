#include "tile_type.h"

#include <log/logger.h>

#include <util/utils.h>

#include <sstream>
using namespace std;

namespace am {
namespace game {

	TileType::TileTypeMap TileType::sTileTypes;

	TileType::TileType(const char *name, const char *fullName) :
		mName(name)
	{
		if (fullName)
		{
			mFullName = fullName;
		}
	}
	TileType::~TileType()
	{
	}

	const char *TileType::getName() const
	{
		return mName.c_str();
	}

	void TileType::setFullName(const char *name)
	{
		mFullName = name;
	}
	const char *TileType::getFullName() const
	{
		return mFullName.c_str();
	}

	bool TileType::loadFromDef(JsonValue value)
	{
		if (value.has("fullName", JV_STR))
		{
			mFullName = value["fullName"].getCStr();
		}
		return true;
	}

	void TileType::addTileType(TileType *type)
	{
		if (type == NULL)
		{
			return;
		}
		sTileTypes[type->mName] = type;
	}
	TileType *TileType::getTileType(const char *name)
	{
		return getTileType(string(name));
	}
	TileType *TileType::getTileType(const string &name)
	{
		TileTypeMap::iterator iter = sTileTypes.find(name);
		if (iter == sTileTypes.end())
		{
			return NULL;
		}
		return iter->second;
	}

	bool TileType::loadStandardTileTypes(const char *filename)
	{
		JsonValue loaded = JsonValue::import_from_file(filename);
		if (loaded.getType() == JV_INT)
		{
			stringstream errss;
			errss << "Unable to load standard tile types: " << loaded.getInt();
			am_log("TILE", errss);
			return false;
		}

		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load standard tile types, loaded file was of type: " << loaded.getTypeName();
			am_log("TILE", errss);
			return false;
		}

		JsonObject *obj = loaded.getObj();
		JsonObject::iterator iter;
		for (iter = obj->begin(); iter != obj->end(); ++iter)
		{
			if (iter->second.getType() == JV_CMT)
			{
				continue;
			}
			if (iter->second.getType() != JV_OBJ)
			{
				stringstream errss;
				errss << "Tile type '" << iter->first << "' was of type '" << iter->second.getTypeName() << "' and not an object.";
				am_log("TILE", errss);
				continue;
			}

			string tileName = Utils::toLowerCase(iter->first.c_str());

			TileType *loadedType = new TileType(tileName.c_str());
			if (!loadedType->loadFromDef(iter->second))
			{
				stringstream errss;
				errss << "Failed to load '" << tileName << "' object in definition was invalid.";
				am_log("TILE", errss);
				delete loadedType;
				continue;
			}

			stringstream ss;
			ss << "Added tile type '" << loadedType->getName() << "'";
			am_log("TILE", ss);
			addTileType(loadedType);
		}
		return true;
	}
	
}
}
