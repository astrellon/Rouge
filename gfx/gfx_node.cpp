#include "gfx_node.h"

#include "gfx_layer.h"

namespace am {
namespace gfx {

	Node::Node() :
		TextStyleSelector(),
		mStyleDirty(true)
	{
		
	}
	Node::Node(const char *nodeType, const char *text) :
		TextStyleSelector(nodeType),
		mStyleDirty(true)
	{
		if (text)
		{
			mText = text;
		}
	}
	Node::~Node()
	{
	}

	void Node::deinit()
	{
		while (mChildren.size())
		{
			mChildren.back()->deinit();
		}
		mChildren.clear();

		mNodeHitboxes.clear();

		if (mParent)
		{
			base::Handle<Node> temp(this);
			mParent->removeChild(this);
		}
	}

	void Node::appendText(const char *text)
	{
		if (mChildren.size() > 0)
		{
			if (strcmp(mChildren.back()->getNodeType(), "text") != 0)
			{
				addChild(new Node("text"));
			}
			mChildren.back()->appendText(text);
		}
		else
		{
			mText += text;
		}
	}

	void Node::addChild(Node *child)
	{
		if (child == nullptr)
		{
			return;
		}
		int index = getChild(child);
		if (index < 0)
		{
			if (mText.length() > 0)
			{
				Node *newNode = new Node("", mText.c_str());
				mText.clear();
				addChild(newNode);
			}
			if (child->mParent != nullptr)
			{
				child->mParent->removeChild(child);
			}
			mChildren.push_back(child);
			child->setParent(this);
		}
	}
	void Node::removeChild(Node *child)
	{
		if (child == nullptr)
		{
			return;
		}
		int index = getChild(child);
		if (index >= 0)
		{
			mChildren.erase(mChildren.begin() + index);
			child->setParent(nullptr);
		}
	}
	void Node::clear()
	{
		mText = "";
		while(mChildren.size())
		{
			removeChild(mChildren[0]);
		}

		while (mNodeHitboxes.size())
		{
			mNodeHitboxes[0]->getParent()->removeChild(mNodeHitboxes[0]);
			mNodeHitboxes.erase(mNodeHitboxes.begin());
		}
		
	}
	bool Node::hasChild(Node *child) const
	{
		return getChild(child) >= 0;
	}
	Node *Node::getChildAt(int index)
	{
		if (index < 0 || index >= static_cast<int>(mChildren.size()))
		{
			return nullptr;
		}
		return mChildren[index];
	}
	int Node::getNumChildren() const
	{
		return static_cast<int>(mChildren.size());
	}
	Node::NodeList &Node::getChildren()
	{
		return mChildren;
	}

	void Node::setText(const char *text)
	{
		mText = text;
	}
	const char *Node::getText() const
	{
		return mText.c_str();
	}

	bool Node::setNodeType(const char *nodeType)
	{
		if (TextStyleSelector::setNodeType(nodeType))
		{
			mStyleDirty = true;
			return true;
		}
		return false;
	}
	bool Node::addClass(const char *klass)
	{
		if (TextStyleSelector::addClass(klass))
		{
			mStyleDirty = true;
			return true;
		}
		return false;
	}
	bool Node::removeClass(const char *klass)
	{
		if (TextStyleSelector::removeClass(klass))
		{
			mStyleDirty = true;
			return true;
		}
		return false;
	}

	bool Node::setAttribute(const char *name, const char *value)
	{
		if (TextStyleSelector::setAttribute(name, value))
		{
			mStyleDirty = true;
			return true;
		}
		return false;
	}
	bool Node::removeAttribute(const char *name)
	{
		if (TextStyleSelector::removeAttribute(name))
		{
			mStyleDirty = true;
			return true;
		}
		return false;
	}
	bool Node::setFlag(Flag flag)
	{
		if (TextStyleSelector::setFlag(flag))
		{
			mStyleDirty = true;
			return true;
		}
		return false;
	}

	const Node::NodeHitboxList &Node::getHitboxes() const
	{
		return mNodeHitboxes;
	}
	Node::NodeHitboxList &Node::getHitboxes()
	{
		return mNodeHitboxes;
	}

	void Node::setParent(Node *node)
	{
		mParent = node;
	}
	Node *Node::getParent() const
	{
		return mParent;
	}
	Node *Node::firstChild()
	{
		if (mChildren.size() > 0)
		{
			Node *first = mChildren[0];
			if (first->getNumChildren() > 0)
			{
				return first->firstChild();
			}
			return first;
		}
		else if (mText.length() > 0)
		{
			return this;
		}
		return nullptr;
	}
	Node *Node::nextSibling()
	{
		if (mParent.get() != nullptr)
		{
			return mParent->getChildAfter(this);
		}
		return nullptr;
	}
	
	TextStyle Node::getTextStyle()
	{
		if (mNodeType.compare("text") == 0 && mParent.get() != nullptr)
		{
			return mParent->getTextStyle();
		}
		if (mStyleDirty)
		{
			mStyle = TextStyle::getCalcStyle(*this);
			mStyleDirty = false;
		}
		return mStyle;
	}

	Node *Node::getChildAfter(Node *node)
	{
		int index = getChild(node);
		if (index >= 0)
		{
			index++;
			if (index >= static_cast<int>(mChildren.size()))
			{
				if (mParent != nullptr)
				{
					return mParent->getChildAfter(this);
				}
				return nullptr;
			}
			Node *child = mChildren[index];
			if (child->mChildren.size() > 0)
			{
				return child->firstChild();
			}
			return child;
		}
		return nullptr;
	}

	int Node::getChild(Node *child) const
	{
		size_t index = 0;
		for (index = 0; index < mChildren.size(); ++index)
		{
			if (mChildren[index].get() == child)
			{
				return static_cast<int>(index);
			}
		}
		return -1;
	}

}
}
