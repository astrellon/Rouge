#include "ui_dialogue_choices.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>

#include <util/utils.h>
using namespace am::util;

namespace am {
namespace ui {

	DialogueChoices::DialogueChoices() :
		UIComponent(),
		mText(new TextField2())
	{
		addChild(mText);
		mText->addEventListener(MOUSE_UP, this);
	}
	DialogueChoices::~DialogueChoices()
	{
		mText->removeEventListener(MOUSE_UP, this);
	}

	TextField2 *DialogueChoices::getTextField() const
	{
		return mText;
	}

	void DialogueChoices::onEvent(MouseEvent *e)
	{
		if (!e)
		{
			return;
		}
		NodeHitbox *nodeTarget = dynamic_cast<NodeHitbox *>(e->getTarget());
		if (nodeTarget)
		{
			Node *node = nodeTarget->getNodeTarget();
			if (strcmp(node->getNodeType(), "?") == 0 || strcmp(node->getNodeType(), "choice") == 0)
			{
				const char *gotoDiag = node->getAttribute("@");
				if (gotoDiag == NULL)
				{
					gotoDiag = node->getAttribute("goto");
				}
				if (gotoDiag == NULL)
				{
					return;
				}

				Dialogue *newDiag = Dialogue::getDialogue(gotoDiag);
				if (newDiag != NULL)
				{
					//setDialogue(newDiag);
				}
			}
			/*int id = -1;
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
					Dialogue *newDialogue = Dialogue::getDialogue(diagName);
					if (newDialogue)
					{
						setDialogue(newDialogue);
					}
				}
			}*/
		}
	}

	void DialogueChoices::setDialogueChoices(const vector<Dialogue *> &choices)
	{
		mChoices = choices;
		updateText();
	}
	const vector<Dialogue *> &DialogueChoices::getDialogueChoices() const
	{
		return mChoices;
	}

	float DialogueChoices::getMeasuredWidth()
	{
		return mText->getMeasuredWidth();
	}
	float DialogueChoices::getMeasuredHeight()
	{
		return mText->getMeasuredHeight();
	}

	void DialogueChoices::setWidth(float width)
	{
		mText->setWidth(clampWidth(width));
	}
	void DialogueChoices::setHeight(float height)
	{
		mText->setHeight(clampHeight(height));
	}

	float DialogueChoices::getWidth()
	{
		return mText->getWidth();
	}
	float DialogueChoices::getHeight()
	{
		return mText->getHeight();
	}
	/*
	void DialogueBox::setDialogueChoices(const Dialogue::DialogueChoices *choices)
	{
		mDialogueChoices = choices;
	}
	const Dialogue::DialogueChoices *DialogueBox::getDialogueChoices() const
	{
		return mDialogueChoices;
	}
	*/
	void DialogueChoices::updateText()
	{
		if (mChoices.size() > 0)
		{
			stringstream ss;
			//ss << "<dialogue>" << mDialogue->getText() << "</dialogue>\n\n";
			//const Dialogue::DialogueChoices &choices = mDialogue->getChoices();
			vector<Dialogue *>::const_iterator iter;
			int i = 0;
			for (iter = mChoices.begin(); iter != mChoices.end(); ++iter)
			{
				//const DialogueChoice &choice = *iter;
				//const DialogueChoice::Attributes &attrs = choice.getAttributes();
				//DialogueChoice::Attributes::const_iterator iter = attrs.find("class");
				
				/*ss << "<dialogue_action ";
				if (iter != attrs.end())
				{
					ss << "class='" << iter->second.c_str() << "' ";
				}
				ss << "id='" << i << "'>" << (i + 1) << ": " << choice.getText() << "</dialogue_action>\n";*/
				Dialogue *diag = *iter;
				ss << "<dialogue_action class='" << diag->getId() << ' ' << diag->getSubject() << "' id='" << i << "'>";
				ss << diag->getText() << "</dialogue_action>";
				i++;
			}
			mText->setText(ss.str());
			//stringstream ss;
			//ss << "<dialogue>" << mDialogue->getText() << "</dialogue>";
			//mText->setText(ss.str());
		}
		else
		{
			mText->setText("");
		}
	}

}
}
