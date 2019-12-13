#include "asteroid.h"
#include<iostream>

using namespace std;

Asteroid::Asteroid(Vector2f position, Vector2f velocity, float radius, Texture* texture) {
	this->position = position;
	this->velocity = velocity;
	this->radius = radius;
	this->size = 3;

	this->rotationRate = 36 / 200;
	if (rand() % 2 == 0) {
		this->angleModifier = -1;
	}
	else {
		this->angleModifier = 1;
	}

	this->shape = CircleShape(this->radius, 100);
	this->shape.setFillColor(Color::White);
	this->shape.setTexture(texture);

	this->many = false;
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
	else if (this->position.x + this->radius <= 0 && this->velocity.x < 0) {
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

	//this->angle += dt_ms * this->rotationRate * this->angleModifier;
}

Vector2f Asteroid::getPosition() {
	return this->position;
}

void Asteroid::setPosition(Vector2f newpos) {
	this->position = newpos;
}

float Asteroid::getRadius() {
	return this->radius;
}

void Asteroid::setRadius(float newrad) {
	if (newrad > 0) {
		this->radius = newrad;
		this->shape.setRadius(newrad);
	}
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

void Asteroid::setActive(bool state) {
	this->active = state;
}

bool Asteroid::isActive() {
	return this->active;
}

int Asteroid::getSize() {
	return this->size;
}

void Asteroid::setSize(int newsize) {
	this->size = newsize;
}

void Asteroid::setMany(bool state) {
	this->many = state;
	if (state) {
		this->shape.setFillColor(Color::Blue);
	}
	else {
		this->shape.setFillColor(Color::White);
	}
}
bool Asteroid::isMany() {
	return this->many;
}