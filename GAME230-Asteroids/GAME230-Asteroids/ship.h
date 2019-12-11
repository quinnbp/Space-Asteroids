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
	Ship(Vector2f position, Vector2f velocity, Vector2f size);
	void draw(RenderWindow* window);
	void update(int dt_ms, int windowWidth, int windowHeight);
	Vector2f getPosition();
	float getRadius();
	Vector2f getVelocity();
	void setVelocity(Vector2f newvel);
private:
	Vector2f velocity;
	Vector2f position;
	RectangleShape shape;
	Vector2f size;
	float angle;
};