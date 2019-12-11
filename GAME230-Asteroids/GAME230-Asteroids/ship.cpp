#include"ship.h"

Ship::Ship(Vector2f position, Vector2f velocity, Vector2f size) {
	this->position = position;
	this->velocity = velocity;
	this->angle = 0.0f;

	this->shape = RectangleShape(size);
	this->shape.setFillColor(Color::White);
}

void Ship::draw(RenderWindow* window) {
	this->shape.setPosition(this->position);
	window->draw(this->shape);
}

void Ship::update(int dt_ms, int windowWidth, int windowHeight) {
	this->position.x += this->velocity.x * dt_ms;
	this->position.y += this->velocity.y * dt_ms;
}

Vector2f getPosition();
float getRadius();
Vector2f getVelocity();
void setVelocity(Vector2f newvel);