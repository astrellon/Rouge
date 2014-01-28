#include "test_mouse_manager.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <ui/mouse_manager.h>

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_layer.h>
#include <gfx/gfx_engine.h>

namespace am {
namespace tests {

	void TestMouseManager::TestListener::onEvent(ui::MouseEvent *e)
	{
		lastEvent = e;
	}

	bool TestMouseManager::testSimple()
	{
		
		base::Handle<gfx::Layer> rootLayer(new gfx::Layer());
		base::Handle<gfx::Renderable> target1(new gfx::Renderable());
		rootLayer->addChild(target1);
		rootLayer->setInteractive(true);
		target1->setInteractive(true);
		target1->setSize(50, 50);
		target1->setPosition(50, 50);

		gfx::GfxEngine::getEngine()->getUILayer()->addChild(rootLayer);

		TestListener targetListener;
		target1->addEventListener(ui::Mouse::MOUSE_DOWN, &targetListener);

		TestListener layerListener;
		rootLayer->addEventListener(ui::Mouse::MOUSE_DOWN, &layerListener);

		ui::MouseManager *manager = ui::MouseManager::getManager();

		manager->onMouseDown(ui::Mouse::LEFT_BUTTON, 0, 0);
		assert(nullptr == targetListener.lastEvent.get());
		assert(nullptr == layerListener.lastEvent.get());

		manager->onMouseDown(ui::Mouse::LEFT_BUTTON, 51, 52);
		assert(nullptr != targetListener.lastEvent.get());
		am_equals(1, targetListener.lastEvent->getLocalMouseX());
		am_equals(2, targetListener.lastEvent->getLocalMouseY());

		assert(nullptr != layerListener.lastEvent.get());
		assert(layerListener.lastEvent->getOriginalTarget() == target1);
		am_equals(1, layerListener.lastEvent->getLocalMouseX());
		am_equals(2, layerListener.lastEvent->getLocalMouseY());

		gfx::GfxEngine::getEngine()->getUILayer()->removeChild(rootLayer);

		return true;
	}

}
}
