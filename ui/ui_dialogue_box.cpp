#include "ui_dialogue_box.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>

#include <util/utils.h>
using namespace am::util;

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
		if (!e)
		{
			return;
		}
		NodeHitbox *nodeTarget = dynamic_cast<NodeHitbox *>(e->getTarget());
		if (nodeTarget)
		{
			Node *node = nodeTarget->getNodeTarget();
			int id = -1;
			bool parsed = Utils::fromString<int>(id, node->getAttribute("id"));
			if (!parsed || id < 0 || id >= mDialogue->getChoices().size())
			{
				return;
			}
			const DialogueChoice &choice = mDialogue->getChoices()[id];
			const char *action = choice.getAttribute("action");
			if (action)
			{
				if (strcmp(action, "goto") == 0)
				{
					const char *diagName = choice.getAttribute("value");
					/*Dialogue *newDialogue = Dialogue::getDialogue(diagName);
					if (newDialogue)
					{
						setDialogue(newDialogue);
					}*/
				}
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
				DialogueChoice::Attributes::const_iterator iter = attrs.find("class");
				ss << "<dialogue_action ";
				if (iter != attrs.end())
				{
					ss << "class='" << iter->second.c_str() << "' ";
				}
				ss << "id='" << i << "'>" << (i + 1) << ": " << choice.getText() << "</dialogue_action>\n";
				i++;
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
