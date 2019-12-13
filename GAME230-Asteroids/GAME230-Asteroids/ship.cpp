#include"ship.h"

const float PI = 3.1415926535;

Ship::Ship(Vector2f position, Vector2f velocity, int dimension, Texture* texture) {
	// frame-updated traits for movement
	this->position = position;
	this->angle = -90.0f;
	this->speed = 0.0f;
	this->direction = Vector2f(1, 0);

	// tweakable traits governing min/max behavior of the ship
	this->baseAcceleration = 0.01f;
	this->maxSpeed = 0.3f;

	// collision parameters
	this->collisionRadius = dimension / 2;

	// shape and color
	this->size = Vector2f(dimension, dimension);
	this->shape = RectangleShape(this->size);
	this->shape.setFillColor(Color::White);
	this->shape.setOrigin(Vector2f(this->size.x / 2.0f, this->size.y / 2.0f));
	this->shape.setTexture(texture);

	// shield
	this->shield = false;
	this->shieldCircle = CircleShape(dimension);
	this->shieldCircle.setFillColor(Color::Transparent);
	this->shieldCircle.setOutlineColor(Color::White);
	this->shieldCircle.setOutlineThickness(3);
	this->shieldCircle.setOrigin(Vector2f(dimension, dimension));
}

void Ship::draw(RenderWindow* window) {
	this->shape.setPosition(Vector2f(this->position.x + this->size.x / 2.0f, this->position.y + this->size.y / 2.0f));
	this->shape.setRotation(this->angle);
	window->draw(this->shape);
	
	if (this->shield) {
		this->shieldCircle.setPosition(this->shape.getPosition());
		window->draw(this->shieldCircle);
	}
}

/*
	Delegator for all update functions for the ship. Updates angle, direction, acceleration, speed and position IN ORDER.
*/
void Ship::update(int dt_ms, int windowWidth, int windowHeight, bool left, bool right, bool up) {
	this->updateAngle(dt_ms, left, right);
	this->updateDirection(); // uses angle
	this->updateAcceleration(up);
	this->updateSpeed(dt_ms); // uses acc
	this->updatePosition(dt_ms, windowWidth, windowHeight); // uses speed, direction
}

/*
	Updates the angle of the ship's rotation based on user keyboard input.
*/
void Ship::updateAngle(int dt_ms, bool left, bool right) {
	int rotationMult = 0;
	if (left && !right) {
		rotationMult = -1.0f;
	}
	else if (!left && right) {
		rotationMult = 1.0f;
	}

	// based on one-second turning circle
	this->angle += dt_ms * 36 / 100 * rotationMult;
}

/*
	Updates the direction based on the angle (unit vector in given direction).
*/
void Ship::updateDirection() {
	this->direction = Vector2f(
		cos(this->angle * PI / 180),
		sin(this->angle * PI / 180)
	);
}

/*
	Updates the acceleration based on the user keyboard input.
*/
void Ship::updateAcceleration(bool up) {
	if (up) {
		this->acceleration = this->baseAcceleration;
		this->shape.setFillColor(Color::Red);
	}
	else {
		this->acceleration = -1.0 * this->baseAcceleration;
		this->shape.setFillColor(Color::White);
	}
}


/*
	Updates the current speed of the ship based on the acceleration.
*/
void Ship::updateSpeed(int dt_ms) {
	this->speed += this->acceleration;
	if (this->speed < 0) {
		this->speed = 0;
	}
	else if (speed > this->maxSpeed) {
		speed = this->maxSpeed;
	}
}


/*
	Updates the current position based on the speed and direction
*/
void Ship::updatePosition(int dt_ms, int windowWidth, int windowHeight) {
	this->position.x += dt_ms * this->speed * this->direction.x;
	this->position.y += dt_ms * this->speed * this->direction.y;

	if (this->position.x > windowWidth) {
		this->position.x = 0;
	}
	else if (this->position.x < 0) {
		this->position.x = windowWidth;
	}

	if (this->position.y > windowHeight) {
		this->position.y = 0;
	}
	else if (this->position.y < 0) {
		this->position.y = windowHeight;
	}
}

void Ship::setAcceleration(float newacc) {
	this->acceleration = newacc;
}

float Ship::getCollisionRadius() {
	return (float) this->collisionRadius;
}

void Ship::setColor(Color color) {
	this->shape.setFillColor(color);
}

Vector2f Ship::getPosition() {
	return this->position;
}

Vector2f Ship::getDirection() {
	return this->direction;
}

void Ship::setPosition(Vector2f newpos) {
	this->position = newpos;
}

void Ship::setShield(bool state) {
	this->shield = state;
}

void Ship::setMultiShot(bool state) {
	this->multiShot = state;
}

bool Ship::getShield() {
	return this->shield;
}

bool Ship::getMultiShot() {
	return this->multiShot;
}