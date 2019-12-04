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
	Asteroid(Vector2f position, Vector2f velocity, float radius);
private:
	Vector2f velocity;
	Vector2f position;
	CircleShape shape;
	float radius;
};