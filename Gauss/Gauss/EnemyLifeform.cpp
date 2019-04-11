#include "EnemyLifeform.h"

dArr<Bullet> EnemyLifeform::bullets;
dArr<Texture> EnemyLifeform::enemyTextures;

void EnemyLifeform::InitializeTextures() {
	Texture temp;
	temp.loadFromFile("Textures/Ships/enemyMoveLeft_v2.png");
	EnemyLifeform::enemyTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Ships/enemyFollow_v2.png");
	EnemyLifeform::enemyTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Ships/enemyMoveLeftShoot_v2.png");
	EnemyLifeform::enemyTextures.Add(Texture(temp));
}

EnemyLifeform::~EnemyLifeform()
{
}

void EnemyLifeform::InitSpritePoisition(const View& view) {
	const float xPos((float)view.getCenter().x + (view.getSize().x / 2));
	const float yPos((float)(rand() % static_cast<int>((view.getCenter().y + view.getSize().y)) + static_cast<int>((view.getCenter().y - view.getSize().y))));
	this->sprite.setPosition(xPos, yPos);
}

void EnemyLifeform::Collision() {
	this->damageTimer = this->damageTimerMax;
}

void EnemyLifeform::TakeDamage(int damage) {
	this->hp = std::max(0, (this->hp - damage));

	// Damage effect
	this->damageTimer = this->damageTimerMax;
}

void EnemyLifeform::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}

void EnemyLifeform::Update(const float &dt, Vector2f target) {
	if (this->damageTimer > 0.f) {
		this->sprite.setColor(Color::Red);
		this->damageTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		// Knockback
		this->sprite.move(this->moveSpeed * -this->normalizedMoveDirection.x * this->damageTimer * dt * DeltaTime::dtMultiplier,
			this->moveSpeed * -this->normalizedMoveDirection.y * this->damageTimer * dt * DeltaTime::dtMultiplier);
	}
	else {
		this->sprite.setColor(Color::White);
	}
}

