#pragma once
#include "math_3d.h"
#include "direction.h"
#include <cctype>

enum mouse {
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE
};

class button_state {
public:
	bool pressed_now;
	bool just_released;
	bool just_pressed;
	void _upd (bool _pressed_now);
};
#define kb_	sf::Keyboard
struct input {
	struct _mouse_info {
		friend struct input;
		v2i pos;
		button_state mbutton[3];
		_mouse_info () {
			FOR (i, 3) {
				mbutton[i].pressed_now = false;
				mbutton[i].just_pressed = false;
				mbutton[i].just_released = false;
			}
		}
	} mouse;
	struct _kb_info {
		friend struct input;
		button_state ctrl;
		button_state alt;
		button_state shift;
		button_state escape;
		button_state space;
		button_state delete_;
		button_state dirs[4];
	private:
		button_state _abc[26];
		button_state _digits[10];
	public:
		button_state &operator [] (char letter) { return isdigit (letter) ? _digits [letter - '0'] : (_abc [letter - (isupper (letter) ? 'A' : 'a')]);}
		_kb_info () {
			FOR (i, 26) {
				_abc[i].just_pressed = false;
				_abc[i].just_released = false;
				_abc[i].pressed_now = false;
			}
		}
	} kb;
	void upd ();
};

extern input in;