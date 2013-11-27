#include "ui_debug_inspector.h"

#include <sstream>

#include <log/logger.h>
#include <util/utils.h>

namespace am {
namespace ui {

	am::base::Handle<DebugInspector> DebugInspector::sInspector;

	DebugInspector::DebugInspector() :
		Panel(),
		mTextDirty(true)
	{
		mInfo = new TextField2();
		//mInfo->setColour(0, 0, 0);
		mInfo->setPosition(10, 22);
		mInfo->setInteractive(true);
		addChild(mInfo);

		setTitle("Debug Inspector");

		mInfo->addEventListener(am::ui::Mouse::MOUSE_UP, this);
	}
	DebugInspector::~DebugInspector()
	{
		mInfo->removeEventListener(am::ui::Mouse::MOUSE_UP, this);
	}

	void DebugInspector::setWidth(float width)
	{
		Panel::setWidth(width);
		mInfo->setWidth(mWidth - 18);
	}
	void DebugInspector::setHeight(float height)
	{
		Panel::setHeight(height);
		mInfo->setHeight(mHeight - 28);
	}

	void DebugInspector::setValue(std::string key, std::string value)
	{
		if (value.empty())
		{
			auto find = mValues.find(key);
			if (find != mValues.end())
			{
				mValues.erase(find);
				mTextDirty = true;
			}
		}
		else
		{
			mValues[key] = value;
			mTextDirty = true;
		}
	}
	std::string DebugInspector::getValue(std::string key) const
	{
		auto find = mValues.find(key);
		if (find != mValues.end())
		{
			return find->second;
		}
		return std::string("");
	}

	TextField2 *DebugInspector::getTextField()
	{
		return mInfo;
	}

	void DebugInspector::render(float dt)
	{
		if (mTextDirty)
		{
			updateText();
		}
		UIComponent::render(dt);
	}

	void DebugInspector::setInspector(DebugInspector *inspector)
	{
		sInspector = inspector;
	}
	DebugInspector *DebugInspector::getInspector()
	{
		return sInspector;
	}

	void DebugInspector::onEvent(MouseEvent *e)
	{
		Panel::onEvent(e);
	}

	void DebugInspector::updateText()
	{
		std::stringstream ss;
		ss << "<inspector>";
		
		for (auto iter = mValues.begin(); iter != mValues.end(); ++iter)
		{
			ss << "<title>" << iter->first << "</title>: " << iter->second << "\n";
		}
		
		ss << "</inspector>";
		mInfo->setText(ss.str());
		mTextDirty = false;
	}

}
}
