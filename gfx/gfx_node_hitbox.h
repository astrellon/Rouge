#pragma once

#include <base/handle.h>
using namespace am::base;

#include <ui/ievent_listener.h>
using namespace am::ui;

#include <gfx/gfx_renderable.h>

namespace am {
namespace gfx {

	class Node;

	class NodeHitbox : public IEventListener, public Renderable {
	public:
		NodeHitbox();
		NodeHitbox(Node *node);

		virtual void retain();
		virtual ~NodeHitbox();

		virtual void onEvent(MouseEvent *e);

		virtual void setNodeTarget(Node *target);
		virtual Node *getNodeTarget() const;

	protected:

		Node *mNodeTarget;
		
		void addListeners(Renderable *target);
		void removeListeners(Renderable *target);
	};

}
}
