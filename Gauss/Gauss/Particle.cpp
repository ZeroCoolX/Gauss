#include "Particle.h"

dArr<Texture> Particle::particleTextures;

void Particle::InitTextures() {
	Texture temp;
	temp.loadFromFile("Textures/Particles/particle01.png");
	Particle::particleTextures.Add(Texture(temp));
}

Particle::Particle(Vector2f pos,
	int textureIndex,
	Vector2f dir,
	float maxVelocity,
	float maxRotation,
	float lifetime,
	Color color)
{
	// Sprite
	this->sprite.setTexture(Particle::particleTextures[textureIndex]);
	this->sprite.setPosition(pos);
	this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
	//this->sprite.setScale(Vector2f(static_cast<float>(rand() % 2 + 1), static_cast<float>(rand() % 2 + 1)));
	this->sprite.setScale(Vector2f(1.f, 1.f));
	this->sprite.setColor(color);

	this->color = color;

	// Normalize random dir
	this->dir.x = static_cast<float>((rand() % 20 + static_cast<int>(dir.x)) - 10);
	this->dir.y = static_cast<float>((rand() % 20 + static_cast<int>(dir.y)) - 10);
	this->dir = this->normalize(this->dir, this->vectorLength(this->dir));

	// Movement data
	this->deceleration = 0.2f;
	this->maxVelocity = maxVelocity;
	this->velocity.x = this->maxVelocity * this->dir.x;
	this->velocity.y = this->maxVelocity * this->dir.y;
	this->maxRotation = maxRotation;

	// Lifetime
	this->lifetime = lifetime;
}


Particle::~Particle()
{
}

void Particle::Update(const float &dt) {
	// Lifetime
	if (this->lifetime > 0) {
		this->lifetime -= 1.f * dt * DeltaTime::dtMultiplier;

		// Reduce the alpha over time
		if (this->sprite.getColor().a > 0) {
			this->sprite.setColor(Color(
				this->color.r, 
				this->color.g, 
				this->color.b, 
				this->sprite.getColor().a - 1) // TODO: this is not framerate independent
			);
			// Once it drops below 0 it goes above 255 because unsigned int
			if (this->sprite.getColor().a > 255) {
				this->sprite.setColor(Color(
					this->color.r, 
					this->color.g, 
					this->color.b, 
					0)
				);
			}
		}
		//Movement
		// Slow down
		this->velocity.x += this->deceleration * -this->dir.x * dt * DeltaTime::dtMultiplier;
		this->velocity.y += this->deceleration * -this->dir.y * dt * DeltaTime::dtMultiplier;
		// Move
		this->sprite.move(
			this->velocity.x * dt * DeltaTime::dtMultiplier,
			this->velocity.y * dt * DeltaTime::dtMultiplier);
		// Rotate
		this->sprite.rotate(this->maxRotation * dt * DeltaTime::dtMultiplier);
	}
}

void Particle::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
