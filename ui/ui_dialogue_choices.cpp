#include "ui_dialogue_choices.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>
#include <game/character.h>

#include <gfx/gfx_texture.h>
using namespace am::gfx;

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
		mText->setLineScroll(1);
		mText->setDisplayHeight(90.0f);
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

		Texture::bindTexture(0);

		glColor4f(0.3f, 0.1f, 0.7f, 0.35f);
		glRectf(0.0f, 0.0f, getWidth(), getHeight());

		/*glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilMask(0x1);
		glStencilFunc(GL_ALWAYS, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glRectf(2.0f, 2.0f, getWidth() - 2.0f, getHeight() - 2.0f);

		glStencilFunc(GL_NOTEQUAL, 0x0, 0x1);*/
	}

	void DialogueChoices::updateText()
	{
		if (mChoices.size() > 0)
		{
			stringstream ss;
			vector<Dialogue *>::const_iterator iter;
			int i = 1;
			for (iter = mChoices.begin(); iter != mChoices.end(); ++iter)
			{
				Dialogue *diag = *iter;
				ss << i++ << ": <? class='" << diag->getId() << ' ' << diag->getSubject();
				ss << "' @='" << diag->getId() << "'>" << diag->getTitle() << "</?>\n";
			}
			mText->setText(ss.str());
		}
		else
		{
			mText->setText("");
		}
	}

	void DialogueChoices::postRender(float dt)
	{
		/*glDisable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);*/

		UIComponent::postRender(dt);
	}

}
}
