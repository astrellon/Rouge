#include "ui_dialogue_box.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>

namespace am {
namespace ui {

	DialogueBox::DialogueBox() :
		UIComponent(),
		mText(new TextField2()),
		mDialogue(NULL)
	{
		addChild(mText);
		mText->addEventListener(MOUSE_UP, this);
	}
	DialogueBox::~DialogueBox()
	{
		mText->removeEventListener(MOUSE_UP, this);
	}

	TextField2 *DialogueBox::getTextField() const
	{
		return mText;
	}

	void DialogueBox::onEvent(MouseEvent *e)
	{
		if (e)
		{
			NodeHitbox *nodeTarget = dynamic_cast<NodeHitbox *>(e->getTarget());
			if (nodeTarget && nodeTarget->getNodeTarget()->getAttribute("id") != NULL)
			{
				stringstream ss;
				ss << "Clicked on " << nodeTarget->getNodeTarget()->getAttribute("id");
				am_log("DIAG", ss);
			}
		}
	}

	void DialogueBox::setDialogue(Dialogue *dialogue)
	{
		if (mDialogue != dialogue)
		{
			mDialogue = dialogue;
			updateText();
		}
	}
	Dialogue *DialogueBox::getDialogue() const
	{
		return mDialogue;
	}

	float DialogueBox::getMeasuredWidth()
	{
		return mText->getMeasuredWidth();
	}
	float DialogueBox::getMeasuredHeight()
	{
		return mText->getMeasuredHeight();
	}

	void DialogueBox::setWidth(float width)
	{
		mText->setWidth(clampWidth(width));
	}
	void DialogueBox::setHeight(float height)
	{
		mText->setHeight(clampHeight(height));
	}

	float DialogueBox::getWidth()
	{
		return mText->getWidth();
	}
	float DialogueBox::getHeight()
	{
		return mText->getHeight();
	}

	void DialogueBox::updateText()
	{
		if (mDialogue != NULL)
		{
			stringstream ss;
			ss << "<dialogue>" << mDialogue->getText() << "</dialogue>\n\n";
			const Dialogue::DialogueChoices &choices = mDialogue->getChoices();
			Dialogue::DialogueChoices::const_iterator iter;
			int i = 0;
			for (iter = choices.begin(); iter != choices.end(); ++iter)
			{
				const DialogueChoice &choice = *iter;
				const DialogueChoice::Attributes &attrs = choice.getAttributes();
				DialogueChoice::Attributes::const_iterator iter;
				ss << "<dialogue_action ";
				for (iter = attrs.begin(); iter != attrs.end(); ++iter)
				{
					ss << iter->first << "='" << iter->second << "' ";
				}
				ss << "id='" << i << "'>" << ++i << ": " << choice.getText() << "</dialogue_action>\n";
			}
			mText->setText(ss.str());
		}
		else
		{
			mText->setText("");
		}
	}

}
}
