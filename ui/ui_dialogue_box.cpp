#include "ui_dialogue_box.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>
#include <game/game_object.h>
#include <game/string_pool.h>

#include <gfx/gfx_texture.h>
using namespace am::gfx;

#include <util/utils.h>
using namespace am::util;

#include <gl.h>

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

				Dialogue *newDiag = Dialogue::getDialogue(gotoDiag);
				mTalker->getDialogueComp()->setSubjectLock(newDiag->getSubject());
				mTalker->getDialogueComp()->talkTo(mTalkedTo, newDiag);
				//mTalker->talkTo(mTalkedTo, newDiag);
				//Handle<DialogueEvent> e(new DialogueEvent(mTalker, mTalkedTo, newDiag));
				//mTalker->fireEvent<DialogueEvent>(e);
			}
			else if (strcmp(node->getNodeType(), "x") == 0 || strcmp(node->getNodeType(), "close") == 0)
			{
				//mTalker->talkTo(mTalkedTo, NULL);
				mTalker->getDialogueComp()->talkTo(mTalkedTo, NULL);
				//Handle<DialogueEvent> e(new DialogueEvent(mTalker, mTalkedTo, NULL));
				//mTalker->fireEvent<DialogueEvent>(e);
			}
		}
	}
	void DialogueBox::onEvent(DialogueEvent *e)
	{
		mTalkedTo = e->getTalkedTo();
		setDialogue(e->getDialogue()); 
	}

	void DialogueBox::setTalker(GameObject *talker)
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
	GameObject *DialogueBox::getTalker() const
	{
		return mTalker;
	}

	void DialogueBox::setTalkedTo(GameObject *talkedTo)
	{
		mTalkedTo = talkedTo;
	}
	GameObject *DialogueBox::getTalkedTo() const
	{
		return mTalkedTo;
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
	
	void DialogueBox::preRender(float dt)
	{
		UIComponent::preRender(dt);

		Texture::bindTexture(0);

		glColor4f(0.3f, 0.7f, 0.1f, 0.35f);
		glRectf(0.0f, 0.0f, getWidth(), getHeight());
	}

	void DialogueBox::updateText()
	{
		if (mDialogue != NULL)
		{
			stringstream ss;
			ss << "<dialogue>" << StringPool::filterText(mDialogue->getText()) << "</dialogue>";
			mText->setText(ss.str());
		}
		else
		{
			mText->setText("");
		}
	}

}
}
