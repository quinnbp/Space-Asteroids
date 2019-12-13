#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

// bullet spray, penetrating bullet, invincible ship (1-hit)

class PowerUp {
public:
	PowerUp();
	void setPosition(Vector2f newpos);
	void setActive(bool state);
	bool isActive();
	void setType(int type);
	float getRadius();
	Vector2f getPosition();
	void draw(RenderWindow* window);
	int getType();
private:
	Vector2f position;
	int type;
	float radius;
	CircleShape shape;
	bool active;
};