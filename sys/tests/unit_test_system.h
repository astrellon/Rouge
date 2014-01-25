#pragma once

#include <sys/game_system.h>

#include <base/handle.h>

namespace am {

namespace game {
    class Engine;
}

namespace gfx {
    class GfxEngine;
    class Layer;
}

namespace ui {
    class MouseManager;
    class Image;
    class UIComponent;
}

namespace sys {

    class OsSystem;

    class UnitTestSystem : public GameSystem 
    {
    public:

        ~UnitTestSystem();

        virtual void init();
        virtual void reshape(int width, int height);

        virtual void onKeyUp(ui::Keyboard::Key key);

        static UnitTestSystem *createUnitTestSystem(OsSystem *linked, game::Engine *engine);
        static UnitTestSystem *getUnitTestSystem();

    protected:
        
        UnitTestSystem(OsSystem *linked, game::Engine *engine);
        
        static UnitTestSystem *sUnitTestSystem;
    };

}
}
