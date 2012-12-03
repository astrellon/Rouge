#include "ui_dialogue_choices.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>
#include <game/character.h>
#include <game/game.h>
#include <game/engine.h>
using namespace am::game;

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
		if (!e || mTalker == NULL || mTalker->getDialogueComp() == NULL)
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

				Dialogue *newDiag = Engine::getGame()->getDialogue(gotoDiag);
				mTalker->getDialogueComp()->talkTo(mTalkedTo, newDiag);
				//mTalker->talkTo(mTalkedTo, newDiag);
			}
			else if (strcmp(node->getNodeType(), "x") == 0 || strcmp(node->getNodeType(), "close") == 0)
			{
				//mTalker->talkTo(mTalkedTo, NULL);
				mTalker->getDialogueComp()->talkTo(mTalkedTo, NULL);
			}
		}
	}

	void DialogueChoices::onEvent(DialogueEvent *e)
	{
		if (e->getDialogue() == NULL)
		{
			mChoices.clear();
			updateText();
		}
		else if (e->getTalkedTo() != NULL)
		{
			vector<Dialogue *> newChoices;
			
			Engine::getGame()->getAvailableDialogues(newChoices, mTalker, e->getTalkedTo());
			mTalkedTo = e->getTalkedTo();
			setDialogueChoices(newChoices);
		}
	}

	void DialogueChoices::setTalker(GameObject *talker)
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
	GameObject *DialogueChoices::getTalker() const
	{
		return mTalker;
	}

	void DialogueChoices::setTalkedTo(GameObject *talkedTo)
	{
		mTalkedTo = talkedTo;
	}
	GameObject *DialogueChoices::getTalkedTo() const
	{
		return mTalkedTo;
	}

	void DialogueChoices::setDialogueChoices(const vector<Dialogue *> &choices)
	{
		bool same = choices.size() == mChoices.size();
		if (same)
		{
			for (size_t i = 0; i < choices.size(); i++)
			{
				if (choices[i] != mChoices[i])
				{
					same = false;
					break;
				}
			}
		}
		if (!same)
		{
			mChoices = choices;
			updateText();
		}
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
