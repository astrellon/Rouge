#include "ui_dialogue_choices.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>
#include <game/character.h>

#include <util/utils.h>
using namespace am::util;

#include <gl.h>

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
				Handle<DialogueEvent> e(new DialogueEvent(mTalker, mTalkedTo, newDiag));
				mTalker->fireEvent<DialogueEvent>(e);
			}
		}
	}

	void DialogueChoices::onEvent(DialogueEvent *e)
	{
		if (e->getTalkedTo() != NULL)
		{
			mChoices.clear();
			Dialogue::getAvailableDialogues(mChoices, mTalker, e->getTalkedTo());
			mTalkedTo = e->getTalkedTo();
			updateText();
		}
	}

	void DialogueChoices::setTalker(Character *talker)
	{
		if (mTalker.get() != NULL)
		{
			mTalker->removeEventListener("dialogue", this);
		}
		mTalker = talker;
		if (mTalker.get() != NULL)
		{
			mTalker->addEventListener("dialogue", this);
		}
	}
	Character *DialogueChoices::getTalker() const
	{
		return mTalker;
	}

	void DialogueChoices::setTalkedTo(Character *talkedTo)
	{
		mTalkedTo = talkedTo;
	}
	Character *DialogueChoices::getTalkedTo() const
	{
		return mTalkedTo;
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

	void DialogueChoices::preRender(float dt)
	{
		UIComponent::preRender(dt);

		glBegin(GL_QUADS);
			glColor4f(0.3f, 0.1f, 0.7f, 0.35f);
			glVertex2f(0, 0);
			glVertex2f(getWidth(), 0);
			glVertex2f(getWidth(), getHeight());
			glVertex2f(0, getHeight());
		glEnd();
	}

	void DialogueChoices::updateText()
	{
		if (mChoices.size() > 0)
		{
			stringstream ss;
			vector<Dialogue *>::const_iterator iter;
			for (iter = mChoices.begin(); iter != mChoices.end(); ++iter)
			{
				Dialogue *diag = *iter;
				ss << "<? class='" << diag->getId() << ' ' << diag->getSubject();
				ss << "' @='" << diag->getId() << "'>" << diag->getTitle() << "</?>\n";
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
