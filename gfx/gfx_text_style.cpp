#include "gfx_text_style.h"

#include <log/logger.h>

#include <util/json_value.h>
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

	bool TextStyle::loadDef(JsonValue value)
	{
		if (value.has("colour", JV_STR))
		{
			mColour.parseFromString(value["colour"].getCStr());
		}
		return true;
	}

	TextStyle TextStyle::getCalcStyle(const TextStyleSelector &node)
	{
		typedef pair<int, TextStyle &> StylePair;
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
			for (int i = 0; i < found.size(); i++)
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
		return NULL;
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

	void TextStyle::loadStyles(const char *filename)
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
	}
	
}
}
