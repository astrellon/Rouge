#include "gfx_node_hitbox.h"

#include <ui/mouse_manager.h>

#include "gfx_node.h"

namespace am {
namespace gfx {

	NodeHitbox::NodeHitbox() :
		Renderable(),
		mNodeTarget(nullptr)
	{
		setInteractive(true);
		addListeners(this);
		setName("NODE HITBOX");
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
		setName("NODE HITBOX");
	}
	NodeHitbox::~NodeHitbox()
	{
		if (mNodeTarget)
		{
			mNodeTarget->release();
		}
		removeListeners(this);
	}

	void NodeHitbox::retain()
	{
		base::IManaged::retain();
	}

	void NodeHitbox::onEvent(MouseEvent *e)
	{
		switch (e->getMouseEventType())
		{
		default:
		case ui::Mouse::MOUSE_OUT:
			mNodeTarget->setFlag(TextStyleSelector::NONE);
			break;
		case ui::Mouse::MOUSE_MOVE:
			{
				TextStyleSelector::Flag flag = TextStyleSelector::HOVER;
				if (MouseManager::getManager()->getButtonDown(e->getMouseButton()))
				{
					flag = TextStyleSelector::DOWN;
				}
				mNodeTarget->setFlag(flag);
			}
			break;
		case ui::Mouse::MOUSE_UP:
		case ui::Mouse::MOUSE_OVER:
			mNodeTarget->setFlag(TextStyleSelector::HOVER);
			break;
		case ui::Mouse::MOUSE_DOWN:
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
		target->addEventListener(ui::Mouse::MOUSE_DOWN, this);
		target->addEventListener(ui::Mouse::MOUSE_MOVE, this);
		target->addEventListener(ui::Mouse::MOUSE_UP, this);
		target->addEventListener(ui::Mouse::MOUSE_OUT, this);
		target->addEventListener(ui::Mouse::MOUSE_OVER, this);
	}
	void NodeHitbox::removeListeners(Renderable *target)
	{
		target->removeEventListener(ui::Mouse::MOUSE_DOWN, this);
		target->removeEventListener(ui::Mouse::MOUSE_MOVE, this);
		target->removeEventListener(ui::Mouse::MOUSE_UP, this);
		target->removeEventListener(ui::Mouse::MOUSE_OUT, this);
		target->removeEventListener(ui::Mouse::MOUSE_OVER, this);
	}

}
}
