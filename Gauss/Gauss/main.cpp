#include "Game.h"
#include "GameMapEditor.h"

using namespace sf;

bool MAPEDITOR = true;

int main() {

	//srand(time(NULL));

	RenderWindow window(VideoMode(1920, 1080), "Gauss", Style::Default);
	//window.setFramerateLimit(20);
	Clock clock;
	float dt = 0.f;

	Game game(&window);
	GameMapEditor mapEditor(&window);

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

		if (MAPEDITOR) {
			mapEditor.Update(dt);
			mapEditor.Draw();
		}
		else {
			game.Update(dt);
			game.Draw();
		}
	}

	return 0;
}