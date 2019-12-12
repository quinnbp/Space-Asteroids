#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "asteroid.h"
#include "ship.h"

using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;

const float PI = 3.1415926535;

enum {MENU, GAMEPLAY, DEAD};

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

	// key booleans
	bool left = false;
	bool right = false;
	bool up = false;
	bool space = true;

	// state
	int state = GAMEPLAY;

	Ship ship = Ship(Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f), Vector2f(0, 0), 20);

	int asteroidSpeedReduction = 5; // slowdown for initial speed
	int numAsteroids = 10; // how many asteroids to have
	int activeAsteroids = 0; // how many are currently active
	vector<Asteroid> asteroids;
	for (int i = 0; i < numAsteroids; i++) {
		Vector2f position = Vector2f(((float)rand() / (RAND_MAX)) * WINDOW_WIDTH, ((float)rand() / (RAND_MAX) * WINDOW_HEIGHT));
		Vector2f velocity = Vector2f(((float)rand() / (RAND_MAX)) / asteroidSpeedReduction, ((float)rand() / (RAND_MAX)) / asteroidSpeedReduction);
		asteroids.push_back(Asteroid(position, velocity, 30));
		activeAsteroids++;
	}

	RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Asteroids");
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Key::Up) {
					up = true;
				}
				else if (event.key.code == Keyboard::Key::Left) {
					left = true;
				}
				else if (event.key.code == Keyboard::Key::Right) {
					right = true;
				}
				else if (event.key.code == Keyboard::Key::Space) {
					space = true;
				}
			}
			else if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Key::Up) {
					up = false;
				}
				else if (event.key.code == Keyboard::Key::Left) {
					left = false;
				}
				else if (event.key.code == Keyboard::Key::Right) {
					right = false;
				}
				else if (event.key.code == Keyboard::Key::Space) {
					space = false;
				}
			}
		}

		// frame timing
		dt_ms = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		if (state == GAMEPLAY) {

			// UPDATE
			ship.update(dt_ms, WINDOW_WIDTH, WINDOW_HEIGHT, left, right, up);
			for (Asteroid& a : asteroids) {
				a.update(dt_ms, WINDOW_WIDTH, WINDOW_HEIGHT);
			}

			// COLLISION
			for (int i = 0; i < activeAsteroids; i++) {
				Asteroid* a = &asteroids[i];
				for (int j = i + 1; j < activeAsteroids; j++) {
					Asteroid* b = &asteroids[j];
					if (circlesCollided(a->getPosition(), b->getPosition(), a->getRadius(), b->getRadius())) {
						// exchange velocities (generalization of 1d equal mass collision)
						Vector2f v1 = a->getVelocity();
						a->setVelocity(b->getVelocity());
						b->setVelocity(v1);
					}
				}

				if (circlesCollided(a->getPosition(), ship.getPosition(), a->getRadius(), ship.getCollisionRadius())) {
					a->setColor(Color::Red);
					// TODO: kill player
				}
			}

			window.clear();

			// DRAW
			ship.draw(&window);
			for (Asteroid& a : asteroids) {
				a.draw(&window);
			}

		}

		window.display();
	}

	return 0;
}
