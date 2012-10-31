#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
using namespace std;

#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ievent_listener.h>
using namespace am::ui;

#include <lua/lua_state.h>
using namespace am::lua;

#include "game_object.h"
#include "item_common.h"
#include "stat_modifiers.h"

namespace am {
namespace game {

	class Item : public GameObject, public IEventListener {
	public:

		enum ItemLocation {
			GROUND, INVENTORY, HAND, MAX_LENGTH
		};

		Item();
		~Item();

		virtual void setGraphic(Sprite *graphic, bool calcInvSize = false);
		virtual Sprite *getGraphic();

		virtual void setGroundGraphic(Sprite *graphic);
		virtual Sprite *getGroundGraphic();

		virtual void setItemType(ItemCommon::ItemType type);
		virtual ItemCommon::ItemType getItemType() const;

		virtual void setItemTypeName(const char *name);
		virtual const char *getItemTypeName() const;

		virtual void setInventorySize(short sizeX, short sizeY);
		virtual short getInventorySizeX() const;
		virtual short getInventorySizeY() const;

		virtual void setItemValue(unsigned int value);
		virtual unsigned int getItemValue() const;

		virtual void setItemLocation(ItemLocation location);
		virtual ItemLocation getItemLocation() const;

		virtual void setQuestItemId(int questItemId);
		virtual int getQuestItemId() const;
		virtual bool isQuestItem() const;

		virtual void setItemName(const char *name);
		virtual const char *getItemName() const;

		virtual void setPrefix(const char *prefix);
		virtual const char *getPrefix() const;

		virtual void setPostfix(const char *postfix);
		virtual const char *getPostfix() const;

		virtual void setItemFullname( const char *name, const char *prefix = "", const char *postfix = "" );
		virtual const char *getFullItemName() const;

		virtual void setItemFrom(const Item &item);

		virtual StatModifiers &getStatModifiers();

		//virtual void loadDef(JsonValue value);
		virtual void loadDef(LuaState &lua);

		virtual float getWidth();
		virtual float getHeight();

		virtual string getName() const;
		virtual const char *getGameObjectTypeName() const;

		virtual void loadFromLua(const char *filename);

		static ItemLocation getItemLocationType(const char *typeName);
		static ItemLocation getItemLocationType(int typeValue);

		static const char *getItemLocationTypeName(ItemLocation location);

	protected:
		
		Handle<Sprite> mGraphic;
		Handle<Sprite> mGroundGraphic;
		Handle<Sprite> mCurrentGraphic;

		string mItemName;
		string mPrefix;
		string mPostfix;
		string mFullname;
		
		ItemCommon::ItemType mItemType;
		ItemLocation mItemLocation;

		short mInventorySizeX;
		short mInventorySizeY;

		int mQuestItemId;

		unsigned int mItemValue;

		StatModifiers mStatModifiers;

		virtual void updateFullname();
		virtual void updateGraphic();

		//virtual void parseStats(const JsonObject &stats, bool magical);
		virtual void parseStats(LuaState &lua, bool magical);

		static const char *sItemLocationNames[];
	};

}
}
