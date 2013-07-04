#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
#include <vector>
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
#include "body_part_common.h"

namespace am {
namespace game {

	class Item : public GameObject, public IEventListener {
	public:

		enum ItemLocation {
			GROUND, INVENTORY, HAND, MAX_LENGTH
		};

		Item();
		Item(const Item &copy);
		~Item();

		Item *clone() const;

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
		virtual string getItemName() const;

		virtual void setPrefix(const char *prefix);
		virtual string getPrefix() const;

		virtual void setPostfix(const char *postfix);
		virtual string getPostfix() const;

		virtual void setItemFullname( const char *name, const char *prefix = "", const char *postfix = "" );
		virtual string getFullItemName() const;

		virtual void setItemFrom(const Item &item);

		virtual StatModifiers &getStatModifiers();

		virtual float getWidth();
		virtual float getHeight();

		virtual string getName() const;
		virtual const char *getGameObjectTypeName() const;

		static ItemLocation getItemLocationType(const char *typeName);
		static ItemLocation getItemLocationType(int typeValue);

		static const char *getItemLocationTypeName(ItemLocation location);

		virtual bool addBodyPartType(BodyPartType::PartType type);
		virtual bool removeBodyPartType(BodyPartType::PartType type);
		virtual bool hasBodyPartType(BodyPartType::PartType type);
		virtual const BodyPartType::TypeList &getBodyPartTypeList() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		Handle<Sprite> mGraphic;
		Handle<Sprite> mGroundGraphic;
		Handle<Sprite> mCurrentGraphic;

		short mPrefix;
		short mPostfix;
		
		ItemCommon::ItemType mItemType;
		ItemLocation mItemLocation;

		short mInventorySizeX;
		short mInventorySizeY;

		int mQuestItemId;

		unsigned int mItemValue;

		StatModifiers mStatModifiers;
		BodyPartType::TypeList mEquipableTo;

		virtual void updateGraphic();

		//virtual void parseStats(const JsonObject &stats, bool magical);
		virtual void parseStats(LuaState &lua, bool magical);

		virtual void onLevelUp();
		virtual void onExperienceChange();

		void getPrePostfix(short &prefix, short &postfix) const;

		static const char *sItemLocationNames[];
	};

}
}
