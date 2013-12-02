#pragma once

#include <base/handle.h>

#include <ui/ievent_listener.h>

#include <gfx/gfx_renderable.h>

namespace am {
namespace gfx {

	class Node;

	class NodeHitbox : public ui::IEventListener, public Renderable 
	{
	public:
		NodeHitbox();
		NodeHitbox(Node *node);

		virtual void retain();
		virtual ~NodeHitbox();

		virtual void onEvent(ui::MouseEvent *e);

		virtual void setNodeTarget(Node *target);
		virtual Node *getNodeTarget() const;

	protected:

		Node *mNodeTarget;
		
		void addListeners(Renderable *target);
		void removeListeners(Renderable *target);
	};

}
}
