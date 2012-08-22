#include "dialogue_choice.h"

namespace am {
namespace game {

	DialogueChoice::DialogueChoice(const char *text) :
		mText(text)
	{
	}
	DialogueChoice::~DialogueChoice()
	{
	}

	void DialogueChoice::setText(const char *text)
	{
		mText = text;
	}
	const char *DialogueChoice::getText() const
	{
		return mText.c_str();
	}

	void DialogueChoice::setAttribute(const char *attribute, const char *value)
	{
		if (value != NULL)
		{
			mAttributes[attribute] = value;
		}
		else
		{
			mAttributes[attribute] = "true";
		}
	}
	const char *DialogueChoice::getAttribute( const char *attribute ) const
	{
		Attributes::const_iterator iter = mAttributes.find(attribute);
		if (iter == mAttributes.end())
		{
			return NULL;
		}
		return iter->second.c_str();
	}
	bool DialogueChoice::hasAttribute( const char *attribute ) const
	{
		return mAttributes.find(attribute) != mAttributes.end();
	}
	bool DialogueChoice::removeAttribute( const char *attribute )
	{
		Attributes::iterator iter = mAttributes.find(attribute);
		if (iter == mAttributes.end())
		{
			return false;
		}
		mAttributes.erase(iter);
		return true;
	}

	const DialogueChoice::Attributes & DialogueChoice::getAttributes() const
	{
		return mAttributes;
	}

	void DialogueChoice::clearAttributes()
	{
		mAttributes.clear();
	}

}
}
