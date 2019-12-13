#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Explosion {
public:
	Explosion();
	void draw(RenderWindow* window);
	void update();
	bool isActive();
	void setActive(bool state);
	void setPosition(Vector2f position);
private:
	int duration;
	float radius;
	CircleShape shape;
	Vector2f position;
	bool active;
};