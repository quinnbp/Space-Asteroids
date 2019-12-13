#include "PowerUp.h"

PowerUp::PowerUp() {
	this->position = Vector2f(0, 0);
	this->active = false;

	this->type = 1;
	this->radius = 10;
	this->shape = CircleShape(this->radius);
	this->shape.setFillColor(Color::Red);
}

void PowerUp::draw(RenderWindow* window) {
	this->shape.setPosition(this->position);
	window->draw(this->shape);
}

void PowerUp::setActive(bool state) {
	this->active = state;
}

bool PowerUp::isActive() {
	return this->active;
}

void PowerUp::setPosition(Vector2f newpos) {
	this->position = newpos;
}

void PowerUp::setType(int newtype) {
	this->type = newtype;
	if (this->type == 1) {
		this->shape.setFillColor(Color::Red);
	}
	else if (this->type == 2) {
		this->shape.setFillColor(Color::Blue);
	}
	else {
		this->shape.setFillColor(Color::Green);
	}
}

Vector2f PowerUp::getPosition() {
	return this->position;
}

float PowerUp::getRadius() {
	return this->radius;
}

int PowerUp::getType() {
	return this->type;
}
