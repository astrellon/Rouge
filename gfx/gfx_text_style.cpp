#include "gfx_text_style.h"

#include <log/logger.h>

//#include <util/json_value.h>
#include <util/utils.h>
using namespace am::util;

#include <algorithm>
#include <sstream>

namespace am {
namespace gfx {

	TextStyle::NodeStyleList TextStyle::sNodeStyleList;
	
	TextStyle::TextStyle() :
		mColour(-1, -1, -1, -1)
	{
	}
	TextStyle::TextStyle(const Colour &colour) :
		mColour(colour)
	{
	}
	TextStyle::~TextStyle()
	{
	}

	void TextStyle::setColour(const Colour &colour)
	{
		mColour = colour;
	}
	Colour TextStyle::getColour() const
	{
		return mColour;
	}
	void TextStyle::removeColour()
	{
		mColour.setColour(-1, -1, -1, -1);
	}
	bool TextStyle::hasColour() const
	{
		return mColour.getRed() >= 0.0f;
	}

	/*bool TextStyle::loadDef(JsonValue value)
	{
		if (value.has("colour", JV_STR))
		{
			mColour.parseFromString(value["colour"].getCStr());
		}
		return true;
	}*/
	bool TextStyle::loadDef(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			return false;
		}
		if (lua.isTableString("colour"))
		{
			mColour.parseFromString(lua_tostring(lua, -1));
			lua.pop(1);
		}
		return true;
	}

	TextStyle TextStyle::getCalcStyle(const TextStyleSelector &node)
	{
		// Was pair<int, TextStyle &> but it seems that the sort
		// algorithm actually changes things up a bit if you do that.
		typedef pair<int, TextStyle> StylePair;
		vector< StylePair > found;
		NodeStyleList::iterator iter;
		for (iter = sNodeStyleList.begin(); iter != sNodeStyleList.end(); ++iter)
		{
			int match = node.match(iter->first);
			if (match > 0)
			{
				found.push_back(StylePair(match, iter->second));
			}
		}

		if (found.size() > 0)
		{
			sort(found.begin(), found.end(), 
				[](const StylePair &a, const StylePair &b) -> bool
			{
				return a.first < b.first;
			});

			TextStyle result;
			for (size_t i = 0; i < found.size(); i++)
			{
				stringstream ss;
				if (found[i].second.hasColour())
				{
					result = found[i].second;
				}
			}
			return result;
		}
		return TextStyle();
	}

	TextStyle *TextStyle::getStyle(const TextStyleSelector &node)
	{
		NodeStyleList::iterator iter;
		for (iter = sNodeStyleList.begin(); iter != sNodeStyleList.end(); ++iter)
		{
			if (iter->first == node)
			{
				return &iter->second;
			}
		}
		return nullptr;
	}

	void TextStyle::addStyle(const TextStyleSelector &node, const TextStyle &style)
	{
		NodeStyleList::iterator iter;
		bool found = false;
		for (iter = sNodeStyleList.begin(); iter != sNodeStyleList.end(); ++iter)
		{
			if (iter->first == node)
			{
				iter->second = style;
				found = true;
				break;
			}
		}
		if (!found)
		{
			sNodeStyleList.push_back(NodeStylePair(node, style));
		}
	}

	bool TextStyle::removeStyle(const TextStyleSelector &node)
	{
		NodeStyleList::iterator iter;
		for (iter = sNodeStyleList.begin(); iter != sNodeStyleList.end(); ++iter)
		{
			if (iter->first == node)
			{
				sNodeStyleList.erase(iter);
				return true;
			}
		}
		return false;
	}

	/*void TextStyle::loadStyles(const char *filename)
	{
		JsonValue loaded = JsonValue::import_from_file(filename);
		if (loaded.getType() != JV_ARR)
		{
			stringstream errss;
			errss << "Error loading styles file '" << filename << "'";
			am_log("STYLE", errss);
			return;
		}

		JsonArray *arr = loaded.getArr();
		JsonArray::iterator iter;
		for (iter = arr->begin(); iter != arr->end(); ++iter)
		{
			if (iter->getType() != JV_OBJ)
			{
				stringstream errss;
				errss << "Non object style in styles '" << filename << "'";
				am_log("STYLE", errss);
				continue;
			}

			JsonValue &value = *iter;
			if (!value.has("style", JV_OBJ))
			{
				stringstream errss;
				errss << "Style selector does not have a style '" << filename << "'";
				am_log("STYLE", errss);
				continue;
			}

			TextStyleSelector selector;
			if (value.has("nodeType", JV_STR))
			{
				selector.setNodeType(value["nodeType"].getCStr());
			}
			if (value.has("flag", JV_STR))
			{
				string lowerFlag = Utils::toLowerCase(value["flag"].getCStr());
				if (lowerFlag.size() == 0)
				{
					// Do nothing.
				}
				else if (lowerFlag.compare("hover") == 0)
				{
					selector.setFlag(TextStyleSelector::HOVER);
				}
				else if (lowerFlag.compare("down") == 0)
				{
					selector.setFlag(TextStyleSelector::DOWN);
				}
				else
				{
					stringstream errss;
					errss << "Unknown flag for selector: " << lowerFlag;
					am_log("STYLE", errss);
				}
			}
			if (value.has("attributes", JV_OBJ))
			{
				JsonObject *attrs = value["attributes"].getObj();
				JsonObject::iterator attrIter;
				for (attrIter = attrs->begin(); attrIter != attrs->end(); ++attrIter)
				{
					const char *name = attrIter->first.c_str();
					JsonType type = attrIter->second.getType();
					if (type == JV_STR)
					{
						selector.setAttribute(name, attrIter->second.getCStr());
					}
					else if (type == JV_INT)
					{
						stringstream ss;
						ss << attrIter->second.getInt();
						selector.setAttribute(name, ss.str().c_str());
					}
					else if (type == JV_FLOAT)
					{
						stringstream ss;
						ss << attrIter->second.getFloat();
						selector.setAttribute(name, ss.str().c_str());
					}
					else if (type == JV_BOOL)
					{
						if (attrIter->second.getBool())
						{
							selector.setAttribute(name, "true");
						}
						else
						{
							selector.setAttribute(name, "false");
						}
					}
					else
					{
						stringstream errss;
						errss << "Unable to use '" << attrIter->first << "' as it is of type '";
						errss << attrIter->second.getTypeName() << "', for an attribute value.";
						am_log("STYLE", errss);
						continue;
					}
				}
			}
			TextStyle style;
			if (!style.loadDef(value["style"]))
			{
				stringstream errss;
				errss << "Failed to load style in file '" << filename << "'";
				am_log("STYLE", errss);
				continue;
			}

			addStyle(selector, style);
		}
	}*/

	void TextStyle::loadStylesLua(const char *filename)
	{
		LuaState lua(false);
		if (!lua.loadFile(filename))
		{
			stringstream errss;
			errss << "Error loading styles file '" << filename << "'";
			lua.logStack("STYLELUA");
			am_log("STYLE", errss);
			lua.close();
			return;
		}

		lua_getglobal(lua, "styles");
		if (!lua_istable(lua, -1))
		{
			stringstream errss;
			errss << "Did not find a global 'styles' table in '" << filename << "'";
			am_log("STYLE", errss);
			lua.close();
			return;
		}

		lua_pushnil(lua);
		while(lua_next(lua, -2) != 0)
		{
			int result = parseStyleTable(lua);
			if (result == -1)
			{
				stringstream errss;
				errss << "Non object at [" << lua_tonumber(lua, -2) << "] in '" << filename << "'";
				am_log("STYLE", errss);
			}
			else if (result == -2)
			{
				stringstream errss;
				errss << "Style selector does not have style at [" << lua_tonumber(lua, -2) << "] in '" << filename << "'";
				am_log("STYLE", errss);
			}
			else if (result == -3)
			{
				stringstream errss;
				errss << "Error load style definition at [" << lua_tonumber(lua, -2) << "] in '" << filename << "'";
				am_log("STYLE", errss);
			}
			stringstream ss;
			lua.printStack(ss);
			lua_pop(lua, 1);
		}
		lua.close();
		return;
	}
	int TextStyle::parseStyleTable(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			return -1;
		}
		if (!lua.isTableTable("style"))
		{
			return -2;
		}
		TextStyle style;
		if (!style.loadDef(lua))
		{
			return -3;
		}
		lua.pop(1);
		TextStyleSelector selector;
		
		if (lua.isTableString("nodeType"))
		{
			selector.setNodeType(lua_tostring(lua, -1));
			lua.pop(1);
		}
		if (lua.isTableString("flag"))
		{
			string lowerFlag = Utils::toLowerCase(lua_tostring(lua, -1));
			lua.pop(1);
			if (lowerFlag.size() == 0)
			{
				// Do nothing.
			}
			else if (lowerFlag.compare("hover") == 0)
			{
				selector.setFlag(TextStyleSelector::HOVER);
			}
			else if (lowerFlag.compare("down") == 0)
			{
				selector.setFlag(TextStyleSelector::DOWN);
			}
			else
			{
				stringstream errss;
				errss << "Unknown flag for selector: " << lowerFlag;
				am_log("STYLE", errss);
			}
		}
		if (lua.isTableTable("attributes"))
		{
			lua_pushnil(lua);
			while (lua_next(lua, -2) != 0)
			{
				const char *key = lua_tostring(lua, -2);
				if (lua_isstring(lua, -1))
				{
					selector.setAttribute(key, lua_tostring(lua, -1));
				}
				else if (lua_isnumber(lua, -1))
				{
					stringstream ss;
					ss << lua_tonumber(lua, -1);
					selector.setAttribute(key, ss.str().c_str());
				}
				else if (lua_isboolean(lua, -1))
				{
					selector.setAttribute(key, lua_toboolean(lua, -1) ? "true" : "false");
				}
				else
				{
					stringstream errss;
					errss << "Unable to use '" << key << "' as it is of type '";
					errss << lua_typename(lua, -1) << "', for an attribute value.";
					am_log("STYLE", errss);
					return 0;
				}
				lua.pop(1);
			}
			lua.pop(1);
		}
		
		addStyle(selector, style);
		return 1;
	}
	
}
}
