#include "test_mouse_manager.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <ui/mouse_manager.h>
using namespace am::ui;

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_layer.h>
using namespace am::gfx;

namespace am {
namespace tests {

	void TestMouseManager::TestListener::onEvent(MouseEvent *e)
	{
		lastEvent = e;
	}

	bool TestMouseManager::testSimple() {
		
		Handle<Layer> rootLayer(new Layer());
		Handle<Renderable> target1(new Renderable());
		rootLayer->addChild(target1);
		rootLayer->setInteractive(true);
		target1->setInteractive(true);
		target1->setSize(50, 50);
		target1->setPosition(50, 50);

		TestListener targetListener;
		target1->addEventListener(MOUSE_DOWN, &targetListener);

		TestListener layerListener;
		rootLayer->addEventListener(MOUSE_DOWN, &layerListener);

		MouseManager *manager = MouseManager::getManager();
		manager->setRootLayer(rootLayer);

		manager->onMouseDown(LEFT_BUTTON, 0, 0);
		assert(nullptr == targetListener.lastEvent.get());
		assert(nullptr == layerListener.lastEvent.get());

		manager->onMouseDown(LEFT_BUTTON, 51, 52);
		assert(nullptr != targetListener.lastEvent.get());
		am_equals(1, targetListener.lastEvent->getLocalMouseX());
		am_equals(2, targetListener.lastEvent->getLocalMouseY());

		assert(nullptr != layerListener.lastEvent.get());
		assert(layerListener.lastEvent->getOriginalTarget() == target1);
		am_equals(1, layerListener.lastEvent->getLocalMouseX());
		am_equals(2, layerListener.lastEvent->getLocalMouseY());

		return true;
	}

}
}
