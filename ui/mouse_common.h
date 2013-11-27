#pragma once

namespace am {
namespace ui {

	class Mouse 
	{
	public:
		enum EventType 
		{
			MOUSE_OVER, MOUSE_OUT, MOUSE_MOVE, MOUSE_DOWN, MOUSE_UP
		};

		static const char *EventTypeNames[];

		enum Button 
		{
			NO_BUTTON, LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON
		};

	};

}
}
