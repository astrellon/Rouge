#include "ui_dialogue_box.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>
#include <game/game_object.h>
#include <game/string_pool.h>
#include <game/game.h>
#include <game/engine.h>

#include <gfx/gfx_texture.h>

#include <util/utils.h>

#include <gl.h>

namespace am {
namespace ui {

	DialogueBox::DialogueBox() :
		UIComponent(),
		mText(new gfx::TextField2()),
		mDialogue(nullptr)
	{
		addChild(mText);
		mText->addEventListener(ui::Mouse::MOUSE_UP, this);
	}
	DialogueBox::~DialogueBox()
	{
		mText->removeEventListener(ui::Mouse::MOUSE_UP, this);
	}

	gfx::TextField2 *DialogueBox::getTextField() const
	{
		return mText;
	}

	void DialogueBox::onEvent(MouseEvent *e)
	{
		if (!e || mTalker == nullptr || mTalker->getDialogueComp() == nullptr)
		{
			return;
		}
		gfx::NodeHitbox *nodeTarget = dynamic_cast<gfx::NodeHitbox *>(e->getOriginalTarget());
		if (nodeTarget)
		{
			gfx::Node *node = nodeTarget->getNodeTarget();
			if (strcmp(node->getNodeType(), "?") == 0 || strcmp(node->getNodeType(), "choice") == 0)
			{
				const char *gotoDiag = node->getAttribute("@");
				if (gotoDiag == nullptr)
				{
					gotoDiag = node->getAttribute("goto");
				}
				if (gotoDiag == nullptr)
				{
					return;
				}

				game::Dialogue *newDiag = game::Engine::getGame()->getDialogue(gotoDiag);
				mTalker->getDialogueComp()->setSubjectLock(newDiag->getSubject());
				mTalker->getDialogueComp()->talkTo(mTalkedTo, newDiag);
			}
			else if (strcmp(node->getNodeType(), "x") == 0 || strcmp(node->getNodeType(), "close") == 0)
			{
				mTalker->getDialogueComp()->talkTo(mTalkedTo, nullptr);
			}
		}
	}
	void DialogueBox::onEvent(DialogueEvent *e)
	{
		mTalkedTo = e->getTalkedTo();
		setDialogue(e->getDialogue()); 
	}

	void DialogueBox::setTalker(game::GameObject *talker)
	{
		if (mTalker.get() != nullptr)
		{
			mTalker->removeEventListener("dialogue", this);
		}
		mTalker = talker;
		if (mTalker.get() != nullptr)
		{
			mTalker->addEventListener("dialogue", this);
		}
	}
	game::GameObject *DialogueBox::getTalker() const
	{
		return mTalker;
	}

	void DialogueBox::setTalkedTo(game::GameObject *talkedTo)
	{
		mTalkedTo = talkedTo;
	}
	game::GameObject *DialogueBox::getTalkedTo() const
	{
		return mTalkedTo;
	}

	void DialogueBox::setDialogue(game::Dialogue *dialogue)
	{
		if (mDialogue != dialogue)
		{
			mDialogue = dialogue;
			updateText();
		}
	}
	game::Dialogue *DialogueBox::getDialogue() const
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

		gfx::Texture::bindTexture(0);

		glColor4f(0.3f, 0.7f, 0.1f, 0.35f);
		glRectf(0.0f, 0.0f, getWidth(), getHeight());
	}

	void DialogueBox::updateText()
	{
		if (mDialogue != nullptr)
		{
			std::stringstream ss;
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
