#include "asteroid.h"

Asteroid::Asteroid(Vector2f position, Vector2f velocity, float radius) {
	this->position = position;
	this->velocity = velocity;
	this->radius = radius;

	this->shape = CircleShape(this->radius, 60);
}