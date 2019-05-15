#pragma once

#include "GameObject.h"

namespace KeyMouseBoard {
	static bool isPressed(int key) {
		const int mKey = key - (Keyboard::KeyCount + 1);
		if (mKey >= 0) {
			return Mouse::isButtonPressed(Mouse::Button(mKey));
		}
		return Keyboard::isKeyPressed(Keyboard::Key(key));
	}

	static int key(const Event *e) {
		if (e->type == Event::KeyPressed || e->type == Event::KeyReleased) {
			return e->key.code;
		}
		if (e->type == Event::MouseButtonPressed) {
			return e->mouseButton.button + Keyboard::KeyCount + 1;
		}
		std::cerr << "Error parsing unexpected event " << std::to_string(e->type) << std::endl;
		return -1;
	}
}
