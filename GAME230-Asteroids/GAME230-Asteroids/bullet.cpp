#include "bullet.h"

/*
	Generates a new bullet at <position> moving in <direction>
*/
Bullet::Bullet(Vector2f position, Vector2f direction) {
	this->position = position;
	this->direction = direction;

	this->speed = 0.8f;
	this->radius = 3.0f;

	this->shape = CircleShape(this->radius);
	this->shape.setFillColor(Color::White);
}

void Bullet::update(int dt_ms, int windowWidth, int windowHeight) {
	this->position.x += dt_ms * this->speed * this->direction.x;
	this->position.y += dt_ms * this->speed * this->direction.y;

	if (this->position.x < 0 || this->position.x > windowWidth) {
		this->active = false;
	}
	if (this->position.y < 0 || this->position.y > windowHeight) {
		this->active = false;
	}
}

void Bullet::draw(RenderWindow* window) {
	this->shape.setPosition(this->position);
	window->draw(this->shape);
}

void Bullet::setPosition(Vector2f newpos) {
	this->position = newpos;
}

void Bullet::setDirection(Vector2f newdir) {
	this->direction = newdir;
}

void Bullet::setActive(bool state) {
	this->active = state;
}

bool Bullet::isActive() {
	return this->active;
}

Vector2f Bullet::getPosition() {
	return this->position;
}

float Bullet::getRadius() {
	return this->radius;
}

void Bullet::setVelocity(Vector2f newvel) {
	// strip magnitude and assign to direction
	// this is bad im aware
	float magnitude = sqrt(newvel.x * newvel.x + newvel.y * newvel.y);
	this->direction = Vector2f(newvel.x / magnitude, newvel.y / magnitude);
}