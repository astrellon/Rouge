#include "game_system.h"

#include "igfx_engine.h"
#include "engine.h"

#include "isprite.h"
#include "itext_field.h"
#include "ilayer.h"

#include "transform.h"

#include "transform.h"
#include "vector.h"

#include <sstream>

using namespace std;
using namespace am::math;

namespace am {
namespace sys {

	GameSystem::GameSystem(ISystem *linked, Engine *engine, IGfxEngine *gfxEngine) :
		mLinkedSystem(linked),
		mEngine(engine),
		mGfxEngine(gfxEngine)
	{

	}
	GameSystem::~GameSystem()
	{
		deinit();
	}

	void GameSystem::setSize(int width, int height)
	{
		mLinkedSystem->setSize(width, height);
	}
	void GameSystem::setPosition(int x, int y)
	{
		mLinkedSystem->setPosition(x, y);
	}

	int GameSystem::getWidth() const
	{
		return mLinkedSystem->getWidth();
	}
	int GameSystem::getHeight() const
	{
		return mLinkedSystem->getHeight();
	}

	int GameSystem::getX() const
	{
		return mLinkedSystem->getX();
	}
	int GameSystem::getY() const
	{
		return mLinkedSystem->getY();
	}

	void GameSystem::setTitle(const char *title)
	{
		mLinkedSystem->setTitle(title);
	}
	const char *GameSystem::getTitle() const
	{
		return mLinkedSystem->getTitle();
	}

	void GameSystem::init()
	{
		mEngine->init();
		mGfxEngine->init();

		mInfo = mGfxEngine->createTextField();
		mInfo->getTransform().setPosition(Vector4f(0, 100, 0));
		mGfxEngine->getRootLayer()->addChild(mInfo);
	}
	void GameSystem::reshape(int width, int height)
	{
		mGfxEngine->reshape(width, height);
	}
	void GameSystem::update(float dt)
	{
		mEngine->update(dt);
	}
	void GameSystem::display(float dt)
	{
		mGfxEngine->display(dt);
	}
	void GameSystem::deinit()
	{
		mGfxEngine->deinit();
		mEngine->deinit();
	}

	void GameSystem::onMouseDown(int mouseButton, int x, int y)
	{
		mGfxEngine->getCursor()->getTransform().setPosition(am::math::Vector4f(x, y, 0));
	}
	void GameSystem::onMouseMove(int mouseButton, int x, int y)
	{
		mGfxEngine->getCursor()->getTransform().setPosition(am::math::Vector4f(x, y, 0));
		mEngine->mouseFunc(mouseButton, x, y);
	}
	void GameSystem::onMouseUp(int mouseButton, int x, int y)
	{
		mGfxEngine->getCursor()->getTransform().setPosition(am::math::Vector4f(x, y, 0));
	}
	void GameSystem::onKeyDown(const bool *keys, int key)
	{
	}
	void GameSystem::onKeyUp(const bool *keys, int key)
	{
	}
	
	bool GameSystem::isProgramRunning() const
	{
		return mLinkedSystem->isProgramRunning();
	}
	void GameSystem::setProgramRunning(bool running)
	{
		mLinkedSystem->setProgramRunning(running);
	}

	bool GameSystem::isRunning() const
	{
		return mLinkedSystem->isRunning();
	}
	int GameSystem::startLoop()
	{
		return mLinkedSystem->startLoop();
	}
	void GameSystem::stopLoop()
	{
		mLinkedSystem->stopLoop();
	}

	ISystem *GameSystem::getLinkedSystem()
	{
		return mLinkedSystem;
	}
	IGfxEngine *GameSystem::getGfxEngine()
	{
		return mGfxEngine;
	}
	Engine *GameSystem::getEngine()
	{
		return mEngine;
	}
}
}
