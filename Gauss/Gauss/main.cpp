#include "Game.h"

using namespace sf;

int main() {

	RenderWindow window(VideoMode(1920, 1080), "Gauss", Style::Default);
	window.setFramerateLimit(60); // 0.016

	Clock clock;
	float dt;

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

		//std::cout << "dt: " << dt << std::endl;
	}

	return 0;
}