#include "Game.h"

using namespace sf;

int main() {

	RenderWindow window(VideoMode(800, 600), "Galactic Defender", Style::Default);
	window.setFramerateLimit(60);

	//Game Loop
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();
		}
	}

	return 0;
}