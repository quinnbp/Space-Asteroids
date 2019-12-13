#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Asteroid {
public:
	Asteroid(Vector2f position, Vector2f velocity, float radius, Texture* texture);
	void draw(RenderWindow* window);
	void update(int dt_ms, int windowWidth, int windowHeight);
	Vector2f getPosition();
	float getRadius();
	Vector2f getVelocity();
	void setVelocity(Vector2f newvel);
	void setColor(Color color);
	void setRadius(float newrad);
	void setActive(bool state);
	bool isActive();
	void setPosition(Vector2f newpos);
	void setSize(int newsize);
	int getSize();
	void setMany(bool state);
	bool isMany();
private:
	Vector2f velocity;
	Vector2f position;
	CircleShape shape;
	float radius;
	bool active;
	int size;
	bool many;
	float angle;
	int angleModifier;
	float rotationRate;
};