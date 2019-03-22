#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include "DeltaTime.h"
#include "dArr.h"

using namespace sf;

class Lifeform
{
private:
	int healthPoints;
	int healthPointsMax;

	int damage;
	int damageMax;

	// Optional
	Vector2f velocity;
	float speed;
	float speedMax;	

public:
	Lifeform();
	~Lifeform();
};

