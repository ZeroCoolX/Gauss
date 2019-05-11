#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "Gauss.h"
#include "KeyManager.h"

class KeyBindingMenu
{
private:
	KeyManager *keyManager;

	bool active;

	int keybindPollingId;
	bool keybindPolling;
	bool keyRefreshNeeded;

	float pressTimeMax;
	float pressTime;
	float keyTimeMax;
	float keyTime;

	Font font;
	Text lastChangeText;
	Texture backgroundTexture;
	RectangleShape background;
	RenderWindow *window;

	// MOUSE 
	Vector2i mousePosWindow;
	Vector2f mousePosWorld;
	Vector2i mousePosGrid;

	dArr<MenuButton*> buttons;
	dArr<std::string> buttonKeyNames;

	void _resetControlToDefault();
	void _goBack();

public:
	KeyBindingMenu(Font &font, RenderWindow *window, KeyManager *keyManager);
	virtual ~KeyBindingMenu();

	inline const bool isKeyRefreshNeeded() { return this->active && this->keyRefreshNeeded; }
	inline void resetKeyRefreshNeeded() { this->keyRefreshNeeded = false; }

	inline bool isActive() const { return this->active; }
	void activate() { this->active = true; }
	void deactivate() { this->active = false; }

	// Init
	void Init();
	void InitButtons();
	void InitBackground();

	// Update
	void Update(const float &dt, const Event *event);
	void UpdateButtons(const float &dt);
	void UpdateMousePosition();
	void UpdateTimers(const float &dt);
	void UpdateKeybindPolling(const float &dt, const Event *event);

	// Draw
	void Draw(RenderTarget &renderTarget);
	void DrawButtons(RenderTarget &renderTarget);
	void DrawBackground(RenderTarget &renderTarget);


	// statics
	enum ButtonTypes {
		BTN_CONTROL_UP = 0,
		BTN_CONTROL_DOWN,
		BTN_CONTROL_LEFT,
		BTN_CONTROL_RIGHT,
		BTN_CONTROL_LASER,
		BTN_CONTROL_MAC,
		BTN_CONTROL_SHIELD,
		BTN_CONTROL_STATS,
		BTN_BACK,
		BTN_RESET
	};
	
};

