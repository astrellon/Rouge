#include "dialogue.h"

namespace am {
namespace game {

	Dialogue::Dialogue(const char *text) :
		mText(text)
	{
	}
	Dialogue::~Dialogue()
	{
	}

	void Dialogue::setText(const char *text)
	{
		mText = text;
	}
	const char *Dialogue::getText() const
	{
		return mText.c_str();
	}

	Dialogue::DialogueChoices &Dialogue::getChoices()
	{
		return mChoices;
	}

}
}
