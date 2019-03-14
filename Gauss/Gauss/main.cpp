#include "Game.h"

using namespace sf;

int main() {

	RenderWindow window(VideoMode(1920, 1080), "Gauss", Style::Default);
	window.setFramerateLimit(60);

	Game game(&window);

	//Game Loop
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();
		}

		game.Update();
		game.Draw();
	}

	return 0;
}