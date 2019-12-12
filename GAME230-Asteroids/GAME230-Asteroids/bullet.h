#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Bullet {
public:
	Bullet(Vector2f position, Vector2f direction);
	void update(int dt_ms, int windowWidth, int windowHeight);
	void draw(RenderWindow* window);
	void setActive(bool state);
	bool isActive();
	void setPosition(Vector2f newpos);
	void setDirection(Vector2f newdir);
	Vector2f getPosition();
	float getRadius();
private:
	Vector2f position;
	Vector2f direction;
	CircleShape shape;
	float radius;
	float speed;
	bool active;
};