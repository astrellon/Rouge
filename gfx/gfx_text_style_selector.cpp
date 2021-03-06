#include "gfx_text_style_selector.h"

#include <util/utils.h>
#include <util/tokeniser.h>

#include <log/logger.h>

namespace am {
namespace gfx {

	TextStyleSelector::TextStyleSelector() :
		mNodeType("text"),
		mFlag(NONE)
	{
	}
	TextStyleSelector::TextStyleSelector( const char *nodeType) :
		mFlag(NONE)
	{
		setNodeType(nodeType);
	}
	TextStyleSelector::~TextStyleSelector()
	{
	}

	bool TextStyleSelector::setNodeType(const char *type)
	{
		std::string typeStr;
		if (type == nullptr || type[0] == '\0')
		{
			typeStr = "text";
		}
		else
		{
			typeStr = util::Utils::toLowerCase(type);
		}
		if (typeStr.compare(mNodeType) != 0)
		{
			mNodeType = typeStr;
			return true;
		}
		return false;
	}
	const char *TextStyleSelector::getNodeType() const
	{
		return mNodeType.c_str();
	}

	bool TextStyleSelector::addClass(const char *klass)
	{
		if (klass != nullptr && klass[0] != '\0')
		{
			std::string klassStr = klass;
			ClassMap::iterator iter = mClasses.find(klassStr);
			if (iter == mClasses.end())
			{
				mClasses[klassStr] = true;
				return true;
			}
		}
		return false;
	}
	bool TextStyleSelector::removeClass(const char *klass)
	{
		if (klass == nullptr || klass[0] == '\0')
		{
			return false;
		}
		ClassMap::iterator iter = mClasses.find(std::string(klass));
		if (iter != mClasses.end())
		{
			mClasses.erase(iter);
			return true;
		}
		return false;
	}
	bool TextStyleSelector::hasClass(const char *klass) const
	{
		if (klass == nullptr || klass[0] == '\0')
		{
			return false;
		}
		ClassMap::const_iterator iter = mClasses.find(std::string(klass));
		if (iter != mClasses.end())
		{
			return true;
		}
		return false;
	}
	TextStyleSelector::ClassMap &TextStyleSelector::getClasses()
	{
		return mClasses;
	}

	TextStyleSelector::AttributeMap &TextStyleSelector::getAttributes()
	{
		return mAttributes;
	}

	bool TextStyleSelector::setAttribute(const char *name, const char *value)
	{
		if (name == nullptr || name[0] == '\0')
		{
			return false;
		}
		if (value == nullptr)
		{
			return removeAttribute(name);
		}
		else
		{
			std::string lower = util::Utils::toLowerCase(name);
			if (lower.compare("class") == 0)
			{
				std::string valueStr = value;
				if (value[0] == '\'' || value[0] == '\"')
				{
					valueStr = valueStr.substr(1, valueStr.size() - 2);
				}
				util::Tokeniser tokeniser(valueStr.c_str());
				mClasses.clear();
				const char *token = tokeniser.nextToken();
				bool changed = false;
				while(token != nullptr)
				{
					changed |= addClass(token);
					token = tokeniser.nextToken();
				}
				return changed;
			}
			else
			{
				std::string valueStr = std::string(value);
				AttributeMap::iterator iter = mAttributes.find(lower);
				if (iter != mAttributes.end())
				{
					if (valueStr.compare(iter->second) == 0)
					{
						return false;
					}
					iter->second = valueStr;
					return true;
				}
				mAttributes[lower] = std::string(value);
				return true;
			}
		}
	}
	bool TextStyleSelector::removeAttribute(const char *name)
	{
		if (name == nullptr || name[0] == '\0')
		{
			return false;
		}
		AttributeMap::iterator iter = mAttributes.find(std::string(name));
		if (iter != mAttributes.end())
		{
			mAttributes.erase(iter);
			return true;
		}
		return false;
	}
	const char *TextStyleSelector::getAttribute(const char *name) const
	{
		if (name == nullptr || name[0] == '\0')
		{
			return nullptr;
		}
		AttributeMap::const_iterator iter = mAttributes.find(std::string(name));
		if (iter != mAttributes.end())
		{
			return iter->second.c_str();
		}
		return nullptr;
	}

	bool TextStyleSelector::setFlag(Flag flag)
	{
		if (mFlag != flag)
		{
			mFlag = flag;
			return true;
		}
		return false;
	}
	TextStyleSelector::Flag TextStyleSelector::getFlag() const
	{
		return mFlag;
	}

	int TextStyleSelector::match(const TextStyleSelector &node) const
	{
		int nodeMatch = 0;
		// Match all
		if (node.mNodeType[0] == '\0' || node.mNodeType[0] == '*')
		{
			nodeMatch = 1;
		}
		// Match specific node.
		else if (node.mNodeType.compare(mNodeType) == 0)
		{
			nodeMatch = 2;
		}
		// Node type doesn't match.
		else
		{
			return -1;
		}

		// Classes won't match.
		size_t nodeClassesSize = node.mClasses.size();
		if (nodeClassesSize > mClasses.size())
		{
			return -2;
		}

		int flagMatch = 0;
		if (node.getFlag() != NONE)
		{
			if (mFlag == node.getFlag())
			{
				if (mFlag != NONE)
				{
					flagMatch = 100;
				}
			}
			else
			{
				return -3;
			}
		}

		int classMatch = 0;

		if (nodeClassesSize > 0)
		{
			ClassMap::const_iterator nodeFind;
		
			for (nodeFind = node.mClasses.begin(); nodeFind != node.mClasses.end(); ++nodeFind)
			{
				ClassMap::const_iterator localFind = mClasses.find(nodeFind->first);
				if (localFind == mClasses.end())
				{
					return -2;
				}
				classMatch += 3;
			}
		}

		return nodeMatch + classMatch + flagMatch;
	}

	bool TextStyleSelector::operator == (const TextStyleSelector &rhs) const
	{
		if (mFlag != rhs.mFlag)
		{
			return false;
		}
		if (mNodeType.compare(rhs.mNodeType) != 0)
		{
			return false;
		}

		if (mAttributes.size() != rhs.mAttributes.size())
		{
			return false;
		}
		if (mClasses.size() != rhs.mClasses.size())
		{
			return false;
		}

		ClassMap::const_iterator classIter;
		for (classIter = mClasses.begin(); classIter != mClasses.end(); ++classIter)
		{
			ClassMap::const_iterator rhsClassIter = rhs.mClasses.find(classIter->first);
			if (rhsClassIter == rhs.mClasses.end())
			{
				return false;
			}
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
	bool TextStyleSelector::operator != (const TextStyleSelector &rhs) const
	{
		return !(*this == rhs);
	}

}
}
