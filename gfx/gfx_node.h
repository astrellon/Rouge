#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <vector>
#include <string>

#include <gfx/gfx_text_style_selector.h>
#include <gfx/gfx_text_style.h>
#include <gfx/gfx_renderable.h>
#include <gfx/gfx_node_hitbox.h>

namespace am {
namespace gfx {

	class Node : public base::IManaged, public TextStyleSelector {
	public:
		typedef std::vector< base::Handle<Node> > NodeList;
		typedef std::vector< base::Handle<NodeHitbox> > NodeHitboxList;
		
		Node();
		Node(const char *nodeType, const char *text = nullptr);
		~Node();

		virtual void deinit();

		virtual void appendText(const char *text);

		virtual void addChild(Node *child);
		virtual void removeChild(Node *child);
		virtual void clear();
		virtual bool hasChild(Node *child) const;
		virtual int getNumChildren() const;
		virtual Node *getChildAt(int index);
		virtual NodeList &getChildren();

		virtual Node *getParent() const;
		virtual Node *firstChild();
		virtual Node *nextSibling();

		virtual TextStyle getTextStyle();

		virtual void setText(const char *text);
		virtual const char *getText() const;

		virtual const NodeHitboxList &getHitboxes() const;
		virtual NodeHitboxList &getHitboxes();

		// TextStyleSelector methods
		virtual bool setNodeType(const char *nodeType);

		virtual bool addClass(const char *klass);
		virtual bool removeClass(const char *klass);

		virtual bool setAttribute(const char *name, const char *value);
		virtual bool removeAttribute(const char *name);

		virtual bool setFlag(Flag flag);

	protected:

		NodeList mChildren;
		base::Handle<Node> mParent;
		std::string mText;
		TextStyle mStyle;
		bool mStyleDirty;

		NodeHitboxList mNodeHitboxes;

		virtual void setParent(Node *node);
		virtual Node *getChildAfter(Node *node);

		int getChild(Node *child) const;
	};

}
}
