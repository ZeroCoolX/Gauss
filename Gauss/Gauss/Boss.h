#pragma once

#include "GameObject.h"
#include "Bullet.h"

enum body_texture { BODY01 = 0 };
enum gun_texture { GUN01 = 0, GUN02 };
enum bullet_texture { BULLET01 = 0};

// TEXTURE RULE: 0 for body, 1 ofr gun, 2 for regular bullet 

class Boss
{
private:
	// BOSS GUN CLASS DEF
	class BossGun {
	private:
		Sprite gunSprite;

		float shootTimerMax;
		float shootTimer;
	public:
		BossGun(Texture *texture, Vector2f position, float shootTimerMax) {
			// TIMERS init
			this->shootTimerMax = shootTimerMax;
			this->shootTimer = this->shootTimerMax;

			// Sprite init
			this->gunSprite.setTexture(*texture);
			this->gunSprite.setPosition(position);
		}
		~BossGun() {}

		void Update(const float &dt) {

		}

		void Draw(RenderTarget &renderTarget) {
			renderTarget.draw(this->gunSprite);
		}
	};
	// BOSS GUN CLASS DEF

private:
	int type;
	int numberOfGuns;

	dArr<Texture> *bodyTextures;
	dArr<Texture> *gunTextures;
	dArr<Texture> *bulletTextures;

	Sprite sprite;
	dArr<BossGun> guns;
	dArr<Bullet> bullets;

	int damageMin;
	int damageMax;

	int hpMax;
	int hp;

public:
	Boss(dArr<Texture> &bodyTextures, 
		dArr<Texture> &gunTextures, 
		dArr<Texture> &bulletTextures, 
		Vector2f position, 
		int type) {
		
		this->type = type;

		this->bodyTextures = &bodyTextures;
		this->gunTextures = &gunTextures;
		this->bulletTextures = &bulletTextures;

		switch (this->type) {
		case 0: // FIRST EASY BOSS
		{
			// check textures.size and type
			this->sprite.setTexture((*this->bodyTextures)[BODY01]);

			// 3 guns - TODO: refactor
			this->guns.Add(BossGun(&(*this->gunTextures)[GUN01], Vector2f(0.f, 0.f), 20.f));
			this->guns.Add(BossGun(&(*this->gunTextures)[GUN01], Vector2f(0.f, 0.f), 20.f));
			this->guns.Add(BossGun(&(*this->gunTextures)[GUN01], Vector2f(0.f, 0.f), 20.f));

			this->sprite.setPosition(position);

			break;
		}
		case 1:
		{
			break;
		}
		}
	}
	virtual ~Boss();

	void Movement();
	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

