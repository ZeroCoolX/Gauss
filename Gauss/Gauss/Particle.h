#pragma once

#include "GameObject.h"

class Particle {
	public:
		Sprite sprite;
		Vector2f dir;
		Vector2f velocity;
		float maxVelocity;
		float deceleration;
		float maxRotation;
		float lifetime;
		Color color;

		Particle(
			Vector2f pos,
			int textureIndex,
			Vector2f dir,
			float maxVelocity,
			float maxRotation,
			float lifetime,
			Color color = Color::White);
		~Particle();

		inline bool canDelete() const { return this->lifetime <= 0.f; }
		// Vector Utility
		inline float vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
		inline Vector2f normalize(Vector2f v, float length) {
			if (length == 0) { return Vector2f(0.f, 0.f); }
			return (v / length);
		}

		void Update(const float &dt);
		void Draw(RenderTarget &renderTarget);

		// Static define
		static dArr<Texture> particleTextures;
		static void InitTextures();
};
