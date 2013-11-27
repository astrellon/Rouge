#include "keyboard_common.h"

namespace am {
namespace ui {

	const char *Keyboard::EventTypeNames[] = 
	{ 
		"key_down", "key_up", "key_press" 
	};

	Keyboard::Key Keyboard::getKey(int key)
	{
		if (key < 0 || key > Keyboard::KEY_MAX_LENGTH)
		{
			return Keyboard::KEY_MAX_LENGTH;
		}
		return static_cast<Keyboard::Key>(key);
	}

	const char *Keyboard::KeyNames[] = 
	{ 
		"None", "Pause", "Insert", "Page Up", "Page Down", "End", "Home", "Delete", 
		"Backspace", "Tab", "Enter", "Up Arrow", "Down Arrow", "Left Arrow", "Right Arrow", 
		"Print Screen", "Caps Lock", "Left Shift", "Right Shift", "Left Ctrl", "Right Ctrl", 
		"Left Alt", "Right Alt", "Scroll Lock",	"Left Special", "Right Special", "Reserved 2", 
		"Escape", "Shift", "Ctrl", "Alt", "Reserved 1",	"Space", "!", "\"", "#", "$", "%", 
		"&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", 
		"6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", 
		"F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", 
		"V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e", 
		"f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", 
		"v", "w", "x", "y", "z", "{", "|", "}", "~", "Num 0", "Num 1", "Num 2", "Num 3", 
		"Num 4", "Num 5", "Num 6", "Num 7", "Num 8", "Num 9", "Num +", "Num -", "Num *", 
		"Num /", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", 
		"MAX_KEY_LENGHT" 
	};
}
}
