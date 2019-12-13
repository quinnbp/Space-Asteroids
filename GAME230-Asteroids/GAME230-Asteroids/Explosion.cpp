#include "Explosion.h"

#include<iostream>

using namespace std;

Explosion::Explosion() {
	this->position = Vector2f(0,0);

	this->duration = 100;
	this->radius = 0;

	this->shape = CircleShape(this->radius);
	this->shape.setFillColor(Color::Transparent);
	this->shape.setOutlineColor(Color::Red);
	this->shape.setOutlineThickness(3);

	this->active = false;
}

void Explosion::update() {
	this->radius++;
	if (this->radius >= 30) {
		this->active = false;
	}
}

void Explosion::draw(RenderWindow* window) {
	this->shape.setPosition(this->position);
	this->shape.setRadius(this->radius);
	window->draw(this->shape);
}
bool Explosion::isActive() {
	return this->active;
}

void Explosion::setPosition(Vector2f newpos) {
	this->position = newpos;
}

void Explosion::setActive(bool state) {
	this->active = state;
}
