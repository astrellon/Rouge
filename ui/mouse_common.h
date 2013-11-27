#pragma once

namespace am {
namespace ui {

	enum MouseEventType {
		MOUSE_OVER, MOUSE_OUT, MOUSE_MOVE, MOUSE_DOWN, MOUSE_UP
	};

	const char **MouseEventTypeNames;

	enum MouseButton {
		NO_BUTTON, LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON
	};

}
}
