#include "KeyManager.h"

std::map<int, std::string> KeyManager::KeyNames;

std::string KeyManager::KeyName(int code) {
	if (code < 0 || KeyManager::KeyNames.find(code) == KeyManager::KeyNames.end()) {
		std::cerr << "Keycode: " << std::to_string(code) << " is not supported " << std::endl;
		return "UNSUPPORTED";
	}
	return KeyManager::KeyNames[code];
}

void KeyManager::InitKeyNames() {
	// Alphas
	KeyManager::KeyNames[Keyboard::Q] = "Q";
	KeyManager::KeyNames[Keyboard::W] = "W";
	KeyManager::KeyNames[Keyboard::E] = "E";
	KeyManager::KeyNames[Keyboard::R] = "R";
	KeyManager::KeyNames[Keyboard::T] = "T";
	KeyManager::KeyNames[Keyboard::Y] = "Y";
	KeyManager::KeyNames[Keyboard::U] = "U";
	KeyManager::KeyNames[Keyboard::I] = "I";
	KeyManager::KeyNames[Keyboard::O] = "O";
	KeyManager::KeyNames[Keyboard::P] = "P";
	KeyManager::KeyNames[Keyboard::A] = "A";
	KeyManager::KeyNames[Keyboard::S] = "S";
	KeyManager::KeyNames[Keyboard::D] = "D";
	KeyManager::KeyNames[Keyboard::F] = "F";
	KeyManager::KeyNames[Keyboard::G] = "G";
	KeyManager::KeyNames[Keyboard::H] = "H";
	KeyManager::KeyNames[Keyboard::J] = "J";
	KeyManager::KeyNames[Keyboard::K] = "K";
	KeyManager::KeyNames[Keyboard::L] = "L";
	KeyManager::KeyNames[Keyboard::Z] = "Z";
	KeyManager::KeyNames[Keyboard::X] = "X";
	KeyManager::KeyNames[Keyboard::C] = "C";
	KeyManager::KeyNames[Keyboard::V] = "V";
	KeyManager::KeyNames[Keyboard::B] = "B";
	KeyManager::KeyNames[Keyboard::N] = "N";
	KeyManager::KeyNames[Keyboard::M] = "M";
	// Right side
	KeyManager::KeyNames[Keyboard::LBracket] = "[";
	KeyManager::KeyNames[Keyboard::RBracket] = "]";
	KeyManager::KeyNames[Keyboard::SemiColon] = ";";
	KeyManager::KeyNames[Keyboard::Quote] = "'";
	KeyManager::KeyNames[Keyboard::Comma] = ",";
	KeyManager::KeyNames[Keyboard::Period] = ".";
	KeyManager::KeyNames[Keyboard::Slash] = "/";
	//KeyManager::KeyNames[Keyboard::BackSpace] = "Backspace";
	KeyManager::KeyNames[Keyboard::BackSlash] = "\\";
	KeyManager::KeyNames[Keyboard::Return] = "Enter";
	KeyManager::KeyNames[Keyboard::RShift] = "Right Shift";
	KeyManager::KeyNames[Keyboard::RAlt] = "Right Alt";
	KeyManager::KeyNames[Keyboard::RControl] = "Right Control";
	KeyManager::KeyNames[Keyboard::Space] = "Space";
	// Left side
	KeyManager::KeyNames[Keyboard::Tab] = "Tab";
	KeyManager::KeyNames[Keyboard::LShift] = "Left Shift";
	KeyManager::KeyNames[Keyboard::LAlt] = "Left Alt";
	KeyManager::KeyNames[Keyboard::LControl] = "Left Control";
	// Number line
	KeyManager::KeyNames[Keyboard::Tilde] = "~";
	KeyManager::KeyNames[Keyboard::Num1] = "Number 1";
	KeyManager::KeyNames[Keyboard::Num2] = "Number 2";
	KeyManager::KeyNames[Keyboard::Num3] = "Number 3";
	KeyManager::KeyNames[Keyboard::Num4] = "Number 4";
	KeyManager::KeyNames[Keyboard::Num5] = "Number 5";
	KeyManager::KeyNames[Keyboard::Num6] = "Number 6";
	KeyManager::KeyNames[Keyboard::Num7] = "Number 7";
	KeyManager::KeyNames[Keyboard::Num8] = "Number 8";
	KeyManager::KeyNames[Keyboard::Num9] = "Number 9";
	KeyManager::KeyNames[Keyboard::Num0] = "Number 0";
	// Arrows
	KeyManager::KeyNames[Keyboard::Up] = "Up Arrow";
	KeyManager::KeyNames[Keyboard::Down] = "Down Arrow";
	KeyManager::KeyNames[Keyboard::Left] = "Left Arrow";
	KeyManager::KeyNames[Keyboard::Right] = "Right Arrow";
	// Num Pad
	KeyManager::KeyNames[Keyboard::Numpad1] = "Numpad 1";
	KeyManager::KeyNames[Keyboard::Numpad2] = "Numpad 2";
	KeyManager::KeyNames[Keyboard::Numpad3] = "Numpad 3";
	KeyManager::KeyNames[Keyboard::Numpad4] = "Numpad 4";
	KeyManager::KeyNames[Keyboard::Numpad5] = "Numpad 5";
	KeyManager::KeyNames[Keyboard::Numpad6] = "Numpad 6";
	KeyManager::KeyNames[Keyboard::Numpad7] = "Numpad 7";
	KeyManager::KeyNames[Keyboard::Numpad8] = "Numpad 8";
	KeyManager::KeyNames[Keyboard::Numpad9] = "Numpad 9";
	KeyManager::KeyNames[Keyboard::Numpad0] = "Numpad 0";
	// Mouse
	KeyManager::KeyNames[(Mouse::Left + Keyboard::KeyCount + 1)] = "Mouse Left";
	KeyManager::KeyNames[(Mouse::Right + Keyboard::KeyCount + 1)] = "Mouse Right";
	KeyManager::KeyNames[(Mouse::Middle + Keyboard::KeyCount + 1)] = "Mouse Middle";
	KeyManager::KeyNames[(Mouse::XButton1 + Keyboard::KeyCount + 1)] = "Mouse X1";
	KeyManager::KeyNames[(Mouse::XButton2 + Keyboard::KeyCount + 1)] = "Mouse X2";
}

KeyManager::KeyManager() 
{
	// Populate defaults
	std::unordered_map<std::string, int> defaultKeys;
	defaultKeys["UP"] = KeyManager::DEF_UP;
	defaultKeys["DOWN"] = KeyManager::DEF_DOWN;
	defaultKeys["LEFT"] = KeyManager::DEF_LEFT;
	defaultKeys["RIGHT"] = KeyManager::DEF_RIGHT;
	defaultKeys["LASER"] = KeyManager::DEF_LASER;
	defaultKeys["MAC"] = KeyManager::DEF_MAC;
	defaultKeys["SHIELD"] = KeyManager::DEF_SHIELD;
	defaultKeys["STATS"] = KeyManager::DEF_STATS;
	this->keys.Add(defaultKeys);

	this->LoadBindingsFromFile();
}


KeyManager::~KeyManager()
{
}

void KeyManager::SaveBindingsToFile() {
	// Save bindings
	std::ofstream fout;
	std::string filename("Keybinding/keybindings.txt");
	fout.open(filename, std::ofstream::out | std::ofstream::trunc);
	if (fout.is_open()) {
		for (std::unordered_map<std::string, int>::iterator it = this->keys[0].begin(); it != this->keys[0].end();) {
			fout << it->first << " " << it->second;
			++it;
			if (it != this->keys[0].end()) {
				fout << "\n";
			}
		}
	}
	else {
		std::cerr << "Failure to open keybindings file" << std::endl;
	}
	fout.close();
}

void KeyManager::LoadBindingsFromFile() {
	// Load bindings
	std::ifstream fin;
	std::string line;
	fin.open("Keybinding/keybindings.txt");
	if (fin.is_open()) {
		while (getline(fin, line)) {
			std::string bindKeyName;
			int bindKeyCode;
			std::stringstream ss;
			// extract the binding
			ss.str(line);
			ss >> bindKeyName;
			ss >> bindKeyCode;
			// use the binding from file
			this->keys[0][bindKeyName] = bindKeyCode;
		}
	}
	else {
		std::cerr << "Failure to open Keybinding/keybindings.txt" << std::endl;
		this->_initDefaults();
	}
	fin.close();
}

void KeyManager::_initDefaults() {
	this->keys[0]["UP"] = KeyManager::DEF_UP;
	this->keys[0]["DOWN"] = KeyManager::DEF_DOWN;
	this->keys[0]["LEFT"] = KeyManager::DEF_LEFT;
	this->keys[0]["RIGHT"] = KeyManager::DEF_RIGHT;
	this->keys[0]["LASER"] = KeyManager::DEF_LASER;
	this->keys[0]["MAC"] = KeyManager::DEF_MAC;
	this->keys[0]["SHIELD"] = KeyManager::DEF_SHIELD;
	this->keys[0]["STATS"] = KeyManager::DEF_STATS;
}

