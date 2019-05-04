#include "Game.h"
#include "GameMapEditor.h"
#include "MainMenu.h"

using namespace sf;

bool MAPEDITOR = false;
bool JOYSTICK_TEST = false;

void DrawDebug(RenderTarget &renderTarget);
void GetJoystickInput();
Text debugText;

int main() {

	//srand(time(NULL));

	RenderWindow window(VideoMode(1920, 1080), "Gauss", Style::Default);
	//window.setFramerateLimit(20);
	Clock clock;
	float dt = 0.f;

	Game game(&window);
	GameMapEditor mapEditor(&window);

	Font f;
	if(!f.loadFromFile("Fonts/Dosis-Light.ttf")){
		std::cout << "Could not load font!" << std::endl;
	}
	debugText.setFont(f);
	debugText.setCharacterSize(24);
	debugText.setFillColor(Color::White);
	debugText.setPosition(0.f, 0.f);


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
		if (JOYSTICK_TEST) {
			GetJoystickInput();
			DrawDebug(window);
		}
		window.display();
	}

	return 0;
}

void GetJoystickInput() {
	// Joystick test
	Joystick::Identification id = Joystick::getIdentification(0);
	std::cout << "\nVendor ID: " << id.vendorId << "\nProduct ID: " << id.productId << std::endl;
	sf::String controller;
	for (size_t i = 0; i < Joystick::Count; i++)
	{
		Joystick::Identification id = Joystick::getIdentification(i);
		controller += "\nJoystick " + std::to_string(i) + ": " + id.name;
	}

	//query joystick for settings if it's plugged in...
	if (sf::Joystick::isConnected(0)) {
		// check how many buttons joystick number 0 has
		unsigned int buttonCount = sf::Joystick::getButtonCount(0);

		const int conId = 2;

		// check if joystick number 0 has a Z axis
		float x = sf::Joystick::getAxisPosition(conId, sf::Joystick::X);
		float y = sf::Joystick::getAxisPosition(conId, sf::Joystick::Y);
		float z = sf::Joystick::getAxisPosition(conId, sf::Joystick::Z);
		float r = sf::Joystick::getAxisPosition(conId, sf::Joystick::R);
		float u = sf::Joystick::getAxisPosition(conId, sf::Joystick::U);
		float v = sf::Joystick::getAxisPosition(conId, sf::Joystick::V);

		std::cout << "Button count: " << buttonCount << std::endl;
		std::cout << "x pos: " << x << std::endl;
		std::cout << "y pos " << y << std::endl;

		std::string s = "\n";

		if (sf::Joystick::isButtonPressed(conId, 0)) {//"A" button on the XBox 360 controller
			s += "A button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 1)) {//"A" button on the XBox 360 controller
			s += "B button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 2)) {//"A" button on the XBox 360 controller
			s += "X button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 3)) {//"A" button on the XBox 360 controller
			s += "Y button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 4)) {//"A" button on the XBox 360 controller
			s += "4? button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 5)) {//"A" button on the XBox 360 controller
			s += "5? button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 6)) {//"A" button on the XBox 360 controller
			s += "6? button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 7)) {//"A" button on the XBox 360 controller
			s += "7? button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 8)) {//"A" button on the XBox 360 controller
			s += "8? button pressed\n";
		}
		if (sf::Joystick::isButtonPressed(conId, 9)) {//"A" button on the XBox 360 controller
			s += "9? button pressed\n";
		}

		debugText.setString(controller +
			"\nButton count:" + std::to_string(buttonCount) +
			"\nx-axis:" + std::to_string(x) +
			"\ny-axis:" + std::to_string(y) +
			"\nz-axis:" + std::to_string(z) +
			"\nr-axis:" + std::to_string(r) +
			"\nu-axis:" + std::to_string(u) +
			"\nv-axis:" + std::to_string(v) +
			s + "\n" + std::to_string(sf::Joystick::Count)
		);
	}
	else {
		debugText.setString(controller + "\nNo Controller Connected...\n" + std::to_string(sf::Joystick::Count));
	}
}

void DrawDebug(RenderTarget &renderTarget) {
	renderTarget.draw(debugText);
}