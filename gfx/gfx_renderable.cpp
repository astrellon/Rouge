#include "gfx_renderable.h"

#include <math/vector.h>

#include <base/handle.h>

#include <log/logger.h>

#include <gl.h>
#include "gfx_engine.h"
#include "gfx_layer.h"
#include "gfx_tooltip.h"

namespace am {
namespace gfx {

	std::ostream& operator<<(std::ostream &strm, const Renderable &rhs)
	{
		strm << "Renderable(" << rhs.getName() << ")";
		return strm;
	}
	
	Renderable::Renderable() :
		base::IManaged(),
		EventInterface(),
		mWidth(0),
		mHeight(0),
		mParent(nullptr),
		mInteractive(false),
		mVisible(true),
		mTooltip(nullptr)
	{
#ifdef _DEBUG
		mRenderColour = false;
#endif
	}
	Renderable::Renderable(const Renderable &copy) :
		base::IManaged(),
		EventInterface(),
		mWidth(copy.mWidth),
		mHeight(copy.mHeight),
		mInteractive(copy.mInteractive),
		mParent(nullptr),
		mVisible(copy.mVisible),
		mTransform(copy.mTransform),
		mTooltip(nullptr)
	{
		if (copy.mGfxComponent)
		{
			mGfxComponent = new GfxComponent(*copy.mGfxComponent.get());
		}
		if (copy.mTooltip)
		{
			mTooltip = static_cast<Tooltip *>(copy.mTooltip->clone());
		}
#ifdef _DEBUG
		mRenderColour = copy.mRenderColour;
		mDebugName = copy.mDebugName;
#endif
	}
	Renderable::~Renderable()
	{
#ifdef _DEBUG
		//am_log("DELETING", mDebugName);
#endif
		//removeTooltipListeners(mTooltip);
		if (mTooltip)
		{
			mTooltip->release();
		}
		if (mParent)
		{
			base::Handle<Layer> parent(mParent);
			mParent = nullptr;
			parent->removeChild(this);
		}
	}

	Renderable *Renderable::clone() const
	{
		return new Renderable(*this);
	}

	void Renderable::deinit()
	{
		{
			base::Handle<ui::Event> e(new ui::Event("deinit", this));
			fireEvent<ui::Event>(e);
		}
		if (mParent)
		{
			mParent->removeChild(this);
		}
	}

	void Renderable::preRender(float dt)
	{
		glPushMatrix();
		mTransform.apply();

		if (mGfxComponent)
		{
			GfxComponent::EffectList effects = mGfxComponent->getEffects();
			for (size_t i = 0; i < effects.size(); i++)
			{
				Effect *effect = effects[i];
				effect->update(dt);
				effect->applyToTarget(this);
				if (effect->getPercent() >= 1.0f)
				{
					effects.erase(effects.begin() + i);
					i--;
					continue;
				}
			}
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (mGfxComponent)
		{
			GfxEngine::getEngine()->pushColourStack(mGfxComponent->getColour());
		}
		else
		{
			GfxEngine::getEngine()->pushColourStack(Colour::WHITE);
		}
		GfxEngine::getEngine()->applyColourStack();
	}
	void Renderable::render(float dt)
	{
#ifdef _DEBUG
		if (getRenderColour())
		{
			Colour::WHITE.applyColour();
			glBindTexture(GL_TEXTURE_2D, 0);
			glBegin(GL_QUADS);
				glVertex2f(mTransform.getX(), mTransform.getY());
				glVertex2f(mTransform.getX(), mTransform.getY() + mHeight);
				glVertex2f(mTransform.getX() + mWidth, mTransform.getY() + mHeight);
				glVertex2f(mTransform.getX() + mWidth, mTransform.getY());
			glEnd();
		}
#endif
	}
	void Renderable::postRender(float dt)
	{
		glPopMatrix();
		//if (mGfxComponent)
		//{
			GfxEngine::getEngine()->popColourStack();
		//}

		glDisable(GL_BLEND);
	}

	void Renderable::setSize(float width, float height)
	{
		setWidth(width);
		setHeight(height);
	}
	void Renderable::setWidth(float width)
	{
		mWidth = width;
	}
	float Renderable::getWidth()
	{
		return mWidth;
	}

	void Renderable::setHeight(float height)
	{
		mHeight = height;
	}
	float Renderable::getHeight()
	{
		return mHeight;
	}

	Layer *Renderable::getParent() const
	{
		return mParent;
	}
	void Renderable::setParent(Layer *layer)
	{
		if (mParent)
		{
			mParent->release();
		}
		mParent = layer;
		if (mParent)
		{
			mParent->retain();
		}
	}

	void Renderable::setInteractive(bool set)
	{
		mInteractive = set;
	}
	bool Renderable::isInteractive() const
	{
		return mInteractive;
	}

	void Renderable::setVisible(bool visible)
	{
		mVisible = visible;
	}
	bool Renderable::isVisible() const
	{
		return mVisible;
	}

	GfxComponent *Renderable::getGfxComponent() const
	{
		return mGfxComponent;
	}
	void Renderable::setGfxComponent(GfxComponent *comp)
	{
		mGfxComponent = comp;
	}

	math::TransformLite &Renderable::getTransform()
	{
		return mTransform;
	}

	void Renderable::setPosition(float x, float y)
	{
		mTransform.setXY(x, y);
	}
	void Renderable::setPosition(int x, int y)
	{
		mTransform.setXY(static_cast<float>(x), static_cast<float>(y));
	}
	float Renderable::getPositionX() const
	{
		return mTransform.getX();
	}
	float Renderable::getPositionY() const
	{
		return mTransform.getY();
	}

	void Renderable::setName(const char *name)
	{
#ifdef _DEBUG
		mDebugName = name;
#endif
	}
	std::string Renderable::getName() const
	{
#ifdef _DEBUG
		return mDebugName;
#else
		return "Renderable";
#endif
	}

	void Renderable::setRenderColour(bool render)
	{
#ifdef _DEBUG
		mRenderColour = render;
#endif
	}
	bool Renderable::getRenderColour() const
	{
#ifdef _DEBUG
		return mRenderColour;
#else
		return false;
#endif
	}

	void Renderable::getRenderPath(RenderablePath &path) const
	{
		path.push_back(this);
		Renderable *parent = getParent();
		while (parent != nullptr)
		{
			path.push_back(parent);
			parent = parent->getParent();
		}
	}
	void Renderable::debugRenderPath(const RenderablePath &path, std::vector<std::string> &output)
	{
		for (auto iter = path.begin(); iter != path.end(); ++iter)
		{
			output.push_back((*iter)->getName());
		}
	}

	void Renderable::getScreenToLocal(const float &inX, const float &inY, float &outX, float &outY) const
	{
		float offsetX = -getPositionX();
		float offsetY = -getPositionY();
		const Layer *parent = getParent();
		while (parent != nullptr)
		{
			offsetX -= parent->getPositionX();
			offsetY -= parent->getPositionY();
			parent = parent->getParent();
		}

		outX = inX + offsetX;
		outY = inY + offsetY;
	}
	void Renderable::getLocalToScreen(const float &inX, const float &inY, float &outX, float &outY) const
	{
		float offsetX = getPositionX();
		float offsetY = getPositionY();
		const Layer *parent = getParent();
		while (parent != nullptr)
		{
			offsetX += parent->getPositionX();
			offsetY += parent->getPositionY();
			parent = parent->getParent();
		}

		outX = inX + offsetX;
		outY = inY + offsetY;
	}

	void Renderable::setTooltip(Tooltip *tooltip)
	{
		if (mTooltip)
		{
			mTooltip->release();
		}
		mTooltip = tooltip;
		if (mTooltip)
		{
			mTooltip->retain();
		}
	}
	Tooltip *Renderable::getTooltip()
	{
		return mTooltip;
	}
	bool Renderable::hasTooltip() const
	{
		return mTooltip != nullptr;
	}

}
}
