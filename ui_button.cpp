#include "ui_button.h"

#include "gfx/gfx_sprite.h"

namespace am {
namespace ui {

	Button::Button(Sprite *graphic) :
		mGraphic(graphic)
	{
		graphic->addEventListener("mouse_move", this);
		graphic->addEventListener("mouse_down", this);
		graphic->addEventListener("mouse_up", this);
		graphic->addEventListener("mouse_out", this);
		graphic->addEventListener("mouse_over", this);
	}

}
}
