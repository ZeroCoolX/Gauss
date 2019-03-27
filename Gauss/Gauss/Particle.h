#pragma once

#include "GameObject.h"

class Particle {
	public:
		Sprite sprite;
		Vector2f dir;
		float maxVelocity;
		float maxRotation;
		float lifetime;

		Particle(
			Vector2f pos,
			int textureIndex,
			Vector2f dir,
			float maxVelocity,
			float maxRotation,
			float lifetime);
		~Particle();

		inline bool canDelete() const { return this->lifetime <= 0.f; }

		void Update(const float &dt);
		void Draw(RenderTarget &renderTarget);

		static dArr<Texture> particleTextures;

};
