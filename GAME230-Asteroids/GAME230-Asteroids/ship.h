#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Ship {
public:
	Ship(Vector2f position, Vector2f velocity, int size, Texture* texture);
	void draw(RenderWindow* window);
	void update(int dt_ms, int windowWidth, int windowHeight, bool left, bool right, bool up);
	Vector2f getPosition();
	void setAcceleration(float newacc);
	float getCollisionRadius();
	void setColor(Color color);
	Vector2f getDirection();
	void setPosition(Vector2f newpos);
	void setShield(bool state);
	void setMultiShot(bool state);
	bool getShield();
	bool getMultiShot();
private:
	RectangleShape shape;
	CircleShape shieldCircle;
	Vector2f size;
	float collisionRadius;

	bool shield;
	bool multiShot;

	float angle;
	float speed;
	float acceleration;
	Vector2f direction;
	Vector2f position;

	float baseAcceleration;
	float maxSpeed;

	void updateAngle(int dt_ms, bool left, bool right);
	void updateDirection(); // uses angle
	void updateAcceleration(bool up);
	void updateSpeed(int dt_ms); // uses acc
	void updatePosition(int dt_ms, int windowWidth, int windowHeight); // uses speed, direction
};