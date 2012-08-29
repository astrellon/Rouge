#include "selector.h"

#include <log/logger.h>

#include <util/utils.h>
#include <util/tokeniser.h>

namespace am {
namespace util {

	Selector::Selector()
	{
	}
	Selector::Selector(const char *id) :
		mId(id)
	{
	}
	Selector::~Selector()
	{
	}

	bool Selector::setId(const char *id)
	{
		string idStr;
		if (id == NULL || id[0] == '\0')
		{
			idStr = "";
		}
		else
		{
			idStr = Utils::toLowerCase(id);
		}
		if (idStr.compare(mId) != 0)
		{
			mId = idStr;
			return true;
		}
		return false;
	}
	const char *Selector::getId() const
	{
		return mId.c_str();
	}

	Selector::AttributeMap &Selector::getAttributes()
	{
		return mAttributes;
	}

	bool Selector::setAttribute(const char *name, JsonValue value, SelectorValue::SelectorValueType type)
	{
		if (name == NULL || name[0] == '\0')
		{
			return false;
		}
		if (value == NULL)
		{
			return removeAttribute(name);
		}
		else
		{
			string lower = Utils::toLowerCase(name);
			//string valueStr = string(value);
			AttributeMap::iterator iter = mAttributes.find(lower);
			if (iter != mAttributes.end())
			{
				//if (valueStr.compare(iter->second) == 0)
				if (value == iter->second.getValue())
				{
					return false;
				}
				iter->second.setValue(value);
				return true;
			}
			mAttributes[lower] = SelectorValue(value, type);
			return true;
		}
	}
	bool Selector::removeAttribute(const char *name)
	{
		if (name == NULL || name[0] == '\0')
		{
			return false;
		}
		AttributeMap::iterator iter = mAttributes.find(string(name));
		if (iter != mAttributes.end())
		{
			mAttributes.erase(iter);
			return true;
		}
		return false;
	}
	JsonValue Selector::getAttribute(const char *name) const
	{
		if (name == NULL || name[0] == '\0')
		{
			return NULL;
		}
		AttributeMap::const_iterator iter = mAttributes.find(string(name));
		if (iter != mAttributes.end())
		{
			return iter->second.getValue();
		}
		return NULL;
	}

	
	int Selector::match(Selector &selector)
	{
		int selectorMatch = 0;
		// Match all
		if (selector.mId.size() == 0 || selector.mId[0] == '*')
		{
			selectorMatch = 1;
		}
		// Match specific node.
		else if (selector.mId.compare(mId) == 0)
		{
			selectorMatch = 2;
		}
		// Node type doesn't match.
		else
		{
			return -1;
		}

		// Attributes won't match.
		size_t selectorAttrSize = selector.mAttributes.size();
		if (selectorAttrSize > mAttributes.size())
		{
			return -2;
		}

		int attrMatch = 0;
		if (selectorAttrSize > 0)
		{
			AttributeMap::iterator selectorFind;

			for (selectorFind = selector.mAttributes.begin(); selectorFind != selector.mAttributes.end(); ++selectorFind)
			{
				AttributeMap::iterator localFind = mAttributes.find(selectorFind->first);
				if (localFind == mAttributes.end())
				{
					return -3;
				}
				//if (localFind->second.compare(selectorFind->second) != 0)
				if (!selectorFind->second.match(localFind->second.getValue()))
				{
					return -4;
				}
				attrMatch += 3;
			}
		}
		return selectorMatch + attrMatch;
	}

	bool Selector::operator == (const Selector &rhs) const
	{
		if (mId.compare(rhs.mId) != 0)
		{
			return false;
		}

		if (mAttributes.size() != rhs.mAttributes.size())
		{
			return false;
		}

		AttributeMap::const_iterator attrIter;
		for (attrIter = mAttributes.begin(); attrIter != mAttributes.end(); ++attrIter)
		{
			AttributeMap::const_iterator rhsAttrIter = rhs.mAttributes.find(attrIter->first);
			if (rhsAttrIter == rhs.mAttributes.end())
			{
				return false;
			}
		}

		return true;
	}
	bool Selector::operator != (const Selector &rhs) const
	{
		return !(*this == rhs);
	}

}
}
