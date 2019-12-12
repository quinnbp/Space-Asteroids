#include "asteroid.h"

Asteroid::Asteroid(Vector2f position, Vector2f velocity, float radius) {
	this->position = position;
	this->velocity = velocity;
	this->radius = radius;

	this->shape = CircleShape(this->radius, 100);
	this->shape.setFillColor(Color::White);
}

void Asteroid::draw(RenderWindow* window) {
	this->shape.setPosition(Vector2f(this->position.x - this->radius, this->position.y - this->radius));
	window->draw(this->shape);
}

void Asteroid::update(int dt_ms, int windowWidth, int windowHeight) {
	this->position.x += this->velocity.x * dt_ms;
	this->position.y += this->velocity.y * dt_ms;


	if (this->position.x - this->radius >= windowWidth && this->velocity.x > 0) {
		// headed right and off screen right
		this->position.x = -this->radius;
	}
	else if (this->position.x + this->radius <= windowWidth && this->velocity.x < 0) {
		// headed left and off screen left
		this->position.x = windowWidth + this->radius;
	}

	if (this->position.y + this->radius <= 0 && this->velocity.y < 0) {
		// headed up and off screen above
		this->position.y = windowHeight + this->radius;
	}
	else if (this->position.y - this->radius >= windowHeight && this->velocity.y > 0) {
		// headed down and off screen below
		this->position.y = -this->radius;
	}
}

Vector2f Asteroid::getPosition() {
	return this->position;
}

float Asteroid::getRadius() {
	return this->radius;
}

Vector2f Asteroid::getVelocity() {
	return this->velocity;
}

void Asteroid::setVelocity(Vector2f newvel) {
	this->velocity = newvel;
}

void Asteroid::setColor(Color color) {
	this->shape.setFillColor(color);
}