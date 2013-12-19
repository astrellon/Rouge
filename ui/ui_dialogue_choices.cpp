#include "ui_dialogue_choices.h"

#include <sstream>

#include <log/logger.h>

#include <game/dialogue.h>
#include <game/character.h>
#include <game/game.h>
#include <game/engine.h>

#include <gfx/gfx_texture.h>

#include <util/utils.h>

#include <gl.h>

#include <ui/ui_store_screen.h>

namespace am {
namespace ui {

	DialogueChoices::DialogueChoices() :
		UIComponent(),
		mText(new gfx::TextField2())
	{
		addChild(mText);
		mText->setDisplayHeight(90.0f);
		mText->addEventListener(ui::Mouse::MOUSE_UP, this);
	}
	DialogueChoices::~DialogueChoices()
	{
		mText->removeEventListener(ui::Mouse::MOUSE_UP, this);
	}

	gfx::TextField2 *DialogueChoices::getTextField() const
	{
		return mText;
	}

	void DialogueChoices::onEvent(MouseEvent *e)
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
				mTalker->getDialogueComp()->talkTo(mTalkedTo, newDiag);
				//mTalker->talkTo(mTalkedTo, newDiag);
			}
			else if (strcmp(node->getNodeType(), "x") == 0 || strcmp(node->getNodeType(), "close") == 0)
			{
				//mTalker->talkTo(mTalkedTo, nullptr);
				mTalker->getDialogueComp()->talkTo(mTalkedTo, nullptr);
			}
			else if (strcmp(node->getNodeType(), "s") == 0 || strcmp(node->getNodeType(), "store") == 0)
			{
				game::Engine *engine = Engine::getEngine();
				GameHud *hud = engine->getGameHud();
				StoreScreen *storeScreen = hud->getStoreScreen();
				game::Character *character = dynamic_cast<game::Character *>(mTalkedTo.get());
				if (character)
				{
					game::Store *store = character->getStore(0);
					if (store)
					{
						storeScreen->setStore(store);
						storeScreen->show();
					}
				}
				//store->setStore()
			}
		}
	}

	void DialogueChoices::onEvent(DialogueEvent *e)
	{
		if (e->getDialogue() == nullptr)
		{
			mChoices.clear();
			updateText();
		}
		else if (e->getTalkedTo() != nullptr)
		{
			std::vector<Dialogue *> newChoices;
			
			game::Engine::getGame()->getAvailableDialogues(newChoices, mTalker, e->getTalkedTo());
			mTalkedTo = e->getTalkedTo();
			setDialogueChoices(newChoices);
		}
	}

	void DialogueChoices::setTalker(game::GameObject *talker)
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
	game::GameObject *DialogueChoices::getTalker() const
	{
		return mTalker;
	}

	void DialogueChoices::setTalkedTo(game::GameObject *talkedTo)
	{
		mTalkedTo = talkedTo;
	}
	game::GameObject *DialogueChoices::getTalkedTo() const
	{
		return mTalkedTo;
	}

	void DialogueChoices::setDialogueChoices(const std::vector<game::Dialogue *> &choices)
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
	const std::vector<game::Dialogue *> &DialogueChoices::getDialogueChoices() const
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

		gfx::Texture::bindTexture(0);

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
			std::stringstream ss;
			std::vector<game::Dialogue *>::const_iterator iter;
			int i = 1;
			for (iter = mChoices.begin(); iter != mChoices.end(); ++iter)
			{
				game::Dialogue *diag = *iter;
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
