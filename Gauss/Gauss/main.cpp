#include "Game.h"

using namespace sf;

int main() {

	RenderWindow window(VideoMode(1920, 1080), "Gauss", Style::Default);

	Clock clock;
	float dt = 0.f;

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

		dt = clock.restart().asSeconds();

		game.Update(dt);
		game.Draw();
	}

	return 0;
}