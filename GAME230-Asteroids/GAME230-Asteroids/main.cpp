#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "asteroid.h"

using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;

/*
	Returns the pythagorean distance between two points
*/
float pythagDist(Vector2f position1, Vector2f position2) {
	return sqrt((position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y));
}

/*
	Returns a boolean indicating whether the two circles collided
*/
bool circlesCollided(Vector2f position1, Vector2f position2, float radius1, float radius2) {
	if (pythagDist(position1, position2) <= radius1 + radius2) {
		return true;
	}
	else {
		return false;
	}
}

int main() {

	// timing
	int dt_ms = 0;
	Clock clock;

	int activeAsteroids = 0;
	vector<Asteroid> asteroids;
	for (int i = 0; i < 20; i++) {
		Vector2f position = Vector2f(((float)rand() / (RAND_MAX)) * WINDOW_WIDTH, ((float)rand() / (RAND_MAX) * WINDOW_HEIGHT));
		Vector2f velocity = Vector2f(((float)rand() / (RAND_MAX)) / 3, ((float)rand() / (RAND_MAX)) / 3);
		asteroids.push_back(Asteroid(position, velocity, 20));
		activeAsteroids++;
	}

	RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Asteroids");
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		dt_ms = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		for (Asteroid& a : asteroids) {
			a.update(dt_ms, WINDOW_WIDTH, WINDOW_HEIGHT);
		}

		for (int i = 0; i < activeAsteroids; i++) {
			for (int j = i + 1; j < activeAsteroids; j++) {
				Asteroid* a = &asteroids[i];
				Asteroid* b = &asteroids[j];
				if (circlesCollided(a->getPosition(), b->getPosition(), a->getRadius(), b->getRadius())) {
					// exchange velocities (generalization of 1d equal mass collision)
					Vector2f v1 = a->getVelocity();
					a->setVelocity(b->getVelocity());
					b->setVelocity(v1);
				}
			}
		}

		window.clear();

		for (Asteroid& a : asteroids) {
			a.draw(&window);
		}

		window.display();
	}

	return 0;
}
