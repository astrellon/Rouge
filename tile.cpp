#include "tile.h"

#include "gfx/gfx_engine.h"
using namespace am::gfx;

#include "tile_set.h"

namespace am {
namespace base {

	Tile::Tile(const char *name) :
		mName(name),
		mFullName("No full name"),
		mGraphic(NULL),
		mTileSet(NULL)
	{
		//printf("Creating tile '%s'\n", name);
	}
	Tile::Tile(const char *name, const char *fullName) :
		mName(name),
		mFullName(fullName),
		mGraphic(NULL),
		mTileSet(NULL)
	{
		//printf("Creating tile '%s', '%s'\n", name, fullName);
	}
	Tile::~Tile()
	{
		//printf("Deleting tile '%s'\n", mFullName.c_str());
	}

	void Tile::setName(const char *name)
	{
		if (!name)
		{
			return;
		}
		if (mTileSet)
		{
			mTileSet->removeTile(this);
		}
		mName = name;
		if (mTileSet)
		{
			mTileSet->addTile(this);
		}
	}
	string Tile::getName() const
	{
		return mName;
	}

	string Tile::getFullName() const
	{
		return mFullName;
	}
	void Tile::setFullName(const char *name)
	{
		mFullName = name;
	}

	string Tile::getDescription() const
	{
		return mDescription;
	}
	void Tile::setDescription(const char *description)
	{
		mDescription = description;
	}

	Asset *Tile::getGraphicAsset()
	{
		return mGraphic.get();
	}
	void Tile::setGraphicAsset(Asset *asset)
	{
		mGraphic = asset;
	}

	void Tile::setTileSet(TileSet *tileSet)
	{
		if (mTileSet)
		{
			mTileSet->release();
		}
		mTileSet = tileSet;
		if (mTileSet)
		{
			mTileSet->retain();
		}
	}
	TileSet *Tile::getTileSet() const
	{
		return mTileSet;
	}

	void Tile::loadDef(JsonValue value) 
	{
		if (value.has("assetName", JV_STR))
		{
			mGraphic = GfxEngine::getEngine()->getAsset(value["assetName"].getCStr());
		}
		if (value.has("fullName", JV_STR))
		{
			mFullName = value["fullName"].getCStr();
		}
		if (value.has("description", JV_STR))
		{
			mDescription = value["description"].getCStr();
		}
	}

}
}
