#include "gfx_node_hitbox.h"

#include <ui/mouse_manager.h>

#include "gfx_node.h"

namespace am {
namespace gfx {

	NodeHitbox::NodeHitbox() :
		Renderable(),
		mNodeTarget(NULL)
	{
		setInteractive(true);
		addListeners(this);
		//setRenderColour(true);
	}
	NodeHitbox::NodeHitbox(Node *node) :
		mNodeTarget(node)
	{
		if (mNodeTarget)
		{
			mNodeTarget->retain();
		}
		setInteractive(true);
		addListeners(this);
		//setRenderColour(true);
	}
	NodeHitbox::~NodeHitbox()
	{
		if (mNodeTarget)
		{
			mNodeTarget->release();
		}
		removeListeners(this);
	}

	void NodeHitbox::onEvent(MouseEvent *e)
	{
		switch (e->getMouseEventType())
		{
		default:
		case am::ui::MOUSE_OUT:
			mNodeTarget->setFlag(TextStyleSelector::NONE);
			break;
		case am::ui::MOUSE_MOVE:
			{
				TextStyleSelector::Flag flag = TextStyleSelector::HOVER;
				if (MouseManager::getManager()->getButtonDown(e->getMouseButton()))
				{
					flag = TextStyleSelector::DOWN;
				}
				mNodeTarget->setFlag(flag);
			}
			break;
		case am::ui::MOUSE_UP:
		case am::ui::MOUSE_OVER:
			mNodeTarget->setFlag(TextStyleSelector::HOVER);
			break;
		case am::ui::MOUSE_DOWN:
			mNodeTarget->setFlag(TextStyleSelector::DOWN);
			break;
		}
	}

	void NodeHitbox::setNodeTarget(Node *node)
	{
		if (mNodeTarget)
		{
			mNodeTarget->release();
		}
		mNodeTarget = node;
		if (mNodeTarget)
		{
			mNodeTarget->retain();
		}
	}
	Node *NodeHitbox::getNodeTarget() const
	{
		return mNodeTarget;
	}

	void NodeHitbox::addListeners(Renderable *target)
	{
		target->addEventListener(MOUSE_DOWN, this);
		target->addEventListener(MOUSE_MOVE, this);
		target->addEventListener(MOUSE_UP, this);
		target->addEventListener(MOUSE_OUT, this);
		target->addEventListener(MOUSE_OVER, this);
	}
	void NodeHitbox::removeListeners(Renderable *target)
	{
		target->removeEventListener(MOUSE_DOWN, this);
		target->removeEventListener(MOUSE_MOVE, this);
		target->removeEventListener(MOUSE_UP, this);
		target->removeEventListener(MOUSE_OUT, this);
		target->removeEventListener(MOUSE_OVER, this);
	}

}
}
