#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include<iostream>

#include "asteroid.h"
#include "ship.h"
#include "bullet.h"

using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;

const int maxBullets = 50;
const int numAsteroids = 10;
const int asteroidStartRadius = 30;
const int levelMultiplier = 10;

const float PI = 3.1415926535;

enum { MENU, GAMEPLAY, DEAD };

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

vector<Asteroid*> prepAsteroids(int level, Texture* asteroidTexture) {
	vector<Asteroid*> asteroids;
	int numAsteroids = level * levelMultiplier;
	int asteroidSpeedReduction = 6 - level;
	if (asteroidSpeedReduction < 1) {
		asteroidSpeedReduction = 1;
	}
	for (int i = 0; i < numAsteroids; i++) {
		Vector2f position = Vector2f(((float)rand() / (RAND_MAX)) * WINDOW_WIDTH, ((float)rand() / (RAND_MAX)*WINDOW_HEIGHT));
		Vector2f velocity = Vector2f(((float)rand() / (RAND_MAX)) / asteroidSpeedReduction, ((float)rand() / (RAND_MAX)) / asteroidSpeedReduction);
		Asteroid* newAsteroid = new Asteroid(position, velocity, asteroidStartRadius, asteroidTexture);
		newAsteroid->setActive(true);
		asteroids.push_back(newAsteroid);
	}
	return asteroids;
}

int main() {

	int currentLevel = 1;
	int playerLives = 3;
	int score = 0;

	SoundBuffer thrustBuffer;
	if (!thrustBuffer.loadFromFile("thrust1.wav")) {
		exit(-1);
	}
	Sound sfx_thrust;
	sfx_thrust.setBuffer(thrustBuffer);

	SoundBuffer dieBuffer;
	if (!dieBuffer.loadFromFile("thrust2.wav")) {
		exit(-1);
	}
	Sound sfx_die;
	sfx_die.setBuffer(dieBuffer);

	SoundBuffer shootBuffer;
	if (!shootBuffer.loadFromFile("shoot1.wav")) {
		exit(-1);
	}
	Sound sfx_shoot;
	sfx_shoot.setBuffer(shootBuffer);

	Texture shipTexture;
	if (!shipTexture.loadFromFile("ship_texture.png")) {
		exit(-1);
	}
	shipTexture.setSmooth(true);

	Texture asteroidTexture;
	if (!asteroidTexture.loadFromFile("asteroid_texture.jpg")) {
		exit(-1);
	}
	asteroidTexture.setSmooth(true);

	Texture gameBg;
	if (!gameBg.loadFromFile("spacebg2.png")) {
		exit(-1);
	}

	Texture menuBg;
	if (!menuBg.loadFromFile("spacebg.png")) {
		exit(-1);
	}

	Font arial;
	if (!arial.loadFromFile("arial.ttf")) {
		exit(-1);
	}

	Font spaceFont;
	if (!spaceFont.loadFromFile("spacefont.otf")) {
		exit(-1);
	}

	RectangleShape menuBgImage = RectangleShape(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	menuBgImage.setPosition(Vector2f(0, 0));
	menuBgImage.setTexture(&menuBg);

	RectangleShape gameBgImage = RectangleShape(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	gameBgImage.setPosition(Vector2f(0, 0));
	gameBgImage.setTexture(&gameBg);

	RectangleShape midline = RectangleShape(Vector2f(4.0f, WINDOW_HEIGHT));
	midline.setPosition(Vector2f(WINDOW_WIDTH / 2.0f - 2.0f, 0));
	RectangleShape midline2 = RectangleShape(Vector2f(WINDOW_WIDTH, 4.0f));
	midline2.setPosition(Vector2f(0, WINDOW_HEIGHT / 2.0f - 2.0f));

	Text titleText;
	titleText.setFont(spaceFont);
	titleText.setString("Space Asteroids");
	titleText.setRotation(5.0f);
	titleText.setCharacterSize(50);
	Text titleTextShadow = titleText;
	titleText.setPosition(Vector2f(WINDOW_WIDTH / 2.0f - 290, WINDOW_HEIGHT / 2.0f - 200.0f));
	titleTextShadow.setPosition(Vector2f(WINDOW_WIDTH / 2.0f - 294.0f, WINDOW_HEIGHT / 2.0f - 204.0f));
	titleTextShadow.setFillColor(Color::Red);
	
	Text playText;
	playText.setFont(spaceFont);
	playText.setPosition(Vector2f(WINDOW_WIDTH / 2.0f - 250.0f, WINDOW_HEIGHT / 2.0f));
	playText.setRotation(-5.0f);
	Text spaceText = playText;
	spaceText.setFillColor(Color::Red);
	playText.setString("press       to play");
	spaceText.setString("      space");

	Text gameOverText = titleText;
	Text returnToMenuText = playText;
	gameOverText.setString("game over");
	returnToMenuText.setString("press esc to \nreturn to menu");

	Text authorText;
	authorText.setFont(spaceFont);
	authorText.setPosition(20, WINDOW_HEIGHT - 50.0f);
	authorText.setString("qbp");

	Text livesText;
	livesText.setFont(arial);
	Text livesNumber = livesText;
	livesText.setString("Lives: ");
	livesText.setPosition(10, WINDOW_HEIGHT - 50);
	livesNumber.setString("3");
	livesNumber.setPosition(100, WINDOW_HEIGHT - 50);

	Text scoreText;
	scoreText.setFont(arial);
	Text scoreNumber = livesText;
	scoreText.setString("Score: ");
	scoreText.setPosition(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 50);
	scoreNumber.setString("0");
	scoreNumber.setPosition(WINDOW_WIDTH - 100, WINDOW_HEIGHT - 50);

	// timing
	int dt_ms = 0;
	Clock clock;
	int thrustSoundTimer = 0;
	int doubleCollideTimer = 0;

	// key booleans
	bool left = false;
	bool right = false;
	bool up = false;
	bool space = false;

	// state
	int state = MENU;

	// game objects
	Ship ship = Ship(Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f), Vector2f(0, 0), 20, &shipTexture);
	vector<Asteroid*> asteroids = prepAsteroids(currentLevel, &asteroidTexture);

	vector<Bullet> bullets;
	for (int i = 0; i < maxBullets; i++) {
		Bullet newBullet = Bullet(Vector2f(0, 0), Vector2f(0, 0));
		newBullet.setActive(false);
		bullets.push_back(newBullet);
	}

	// window setup
	RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Asteroids");
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	// GAME LOOP
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
				else if (event.key.code == Keyboard::Key::Escape) {
					space = false;
					state = MENU;
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
			}
		}

		// frame timing
		dt_ms = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		window.clear();

		if (state == MENU) {
			window.draw(menuBgImage);
			window.draw(titleTextShadow);
			window.draw(titleText);
			window.draw(playText);
			window.draw(spaceText);
			window.draw(authorText);

			if (space) { // if starting game
				space = false;

				// reset vars
				playerLives = 3;
				currentLevel = 1;
				for (int i = 0; i < bullets.size(); i++) {
					bullets[i].setActive(false);
				}
				ship.setPosition(Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

				// prep new asteroids and move to gameplay
				asteroids = prepAsteroids(currentLevel, &asteroidTexture);
				state = GAMEPLAY;
			}
		}
		else if (state == DEAD) {
			window.draw(gameBgImage);
			for (int i = 0; i < asteroids.size(); i++) {
				if (asteroids[i]->isActive()) {
					asteroids[i]->draw(&window);
				}
			}

			window.draw(gameOverText);
			window.draw(returnToMenuText);
			window.draw(scoreText);
			window.draw(scoreNumber);
		}
		else if (state == GAMEPLAY) {

			// UPDATE
			ship.update(dt_ms, WINDOW_WIDTH, WINDOW_HEIGHT, left, right, up);
			if (up && thrustSoundTimer > 5) {
				thrustSoundTimer = 0;
				sfx_thrust.play();
			}
			else if (up) {
				thrustSoundTimer++;
			}

			for (int i = 0; i < asteroids.size(); i++) {
				asteroids[i]->update(dt_ms, WINDOW_WIDTH, WINDOW_HEIGHT);
			}

			for (int i = 0; i < bullets.size(); i++) {
				if (space && !bullets[i].isActive()) { // bullet is inactive and we need one
					space = false;
					bullets[i].setPosition(ship.getPosition()); // TODO: center bullet
					bullets[i].setDirection(ship.getDirection());
					bullets[i].setActive(true);
					sfx_shoot.play();
				}

				if (bullets[i].isActive()) { // bullet is active
					bullets[i].update(dt_ms, WINDOW_WIDTH, WINDOW_HEIGHT);
				}
			}

			// COLLISION
			for (int i = 0; i < asteroids.size(); i++) {
				if (asteroids[i]->isActive()) {
					Asteroid* a = asteroids[i];
					for (int j = i + 1; j < asteroids.size(); j++) {
						if (asteroids[j]->isActive()) {
							Asteroid* b = asteroids[j];
							if (circlesCollided(a->getPosition(), b->getPosition(), a->getRadius(), b->getRadius())) {
								// exchange velocities (generalization of 1d equal mass collision)
								Vector2f v1 = a->getVelocity();
								a->setVelocity(b->getVelocity());
								b->setVelocity(v1);
							}
						}
					}
					
					if (doubleCollideTimer > 500) { // prevent multiple collisions over few frames
						if (circlesCollided(a->getPosition(), ship.getPosition(), a->getRadius(), ship.getCollisionRadius())) {
							playerLives--;
							doubleCollideTimer = 0;
							ship.loseLife();
							sfx_die.play();
							ship.setPosition(Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
							if (playerLives <= 0) { // TODO: fix multi-collide
								state = DEAD;
							}
						}
					}
					else {
						ship.setColor(Color::Blue);
						doubleCollideTimer++;
					}

					for (int i = 0; i < bullets.size(); i++) {
						if (bullets[i].isActive()) {
							if (circlesCollided(a->getPosition(), bullets[i].getPosition(), a->getRadius(), bullets[i].getRadius())) {
								score += rand() % 90 + 10;
								bullets[i].setActive(false);
								if (a->getSize() > 1) {
									// get asteroid current pos and new radius
									Vector2f currentPos = a->getPosition();
									float newRadius = a->getRadius() / 2.0f;

									// generate two new positions and velocities
									Vector2f newpos1 = Vector2f(currentPos.x + a->getRadius() / 2.0f, currentPos.y + a->getRadius() / 2.0f);
									Vector2f newpos2 = Vector2f(currentPos.x - a->getRadius() / 2.0f, currentPos.y - a->getRadius() / 2.0f);
									Vector2f newvel1 = a->getVelocity();
									Vector2f newvel2 = Vector2f(-1.0f * a->getVelocity().x, -1.0f * a->getVelocity().y);

									// spawn a new asteroid with one of each and push back
									Asteroid* spawned1;
									spawned1 = new Asteroid(newpos1, newvel1, newRadius, &asteroidTexture);
									spawned1->setSize(a->getSize() - 1);
									asteroids.push_back(spawned1);

									// set a to other two (reuse)
									a->setPosition(newpos2);
									a->setVelocity(newvel2);
									a->setRadius(newRadius);
									a->setSize(a->getSize() - 1);
								}
								else {
									// smallest size elminiation
									a->setActive(false);
								}

							}
						}
					}
				}
			}

			bool activeAsteroid = false;
			for (int i = 0; i < asteroids.size(); i++) {
				if (asteroids[i]->isActive()) {
					activeAsteroid = true;
					break;
				}
			}

			if (!activeAsteroid) {
				currentLevel++;
				asteroids = prepAsteroids(currentLevel, &asteroidTexture);
			}

			// DRAW (GAMEPLAY)
			window.draw(gameBgImage);

			ship.draw(&window);
			for (int i = 0; i < asteroids.size(); i++) {
				if (asteroids[i]->isActive()) {
					asteroids[i]->draw(&window);
				}
			}
			for (int i = 0; i < bullets.size(); i++) {
				if (bullets[i].isActive()) {
					bullets[i].draw(&window);
				}
			}

			scoreNumber.setString(to_string(score));
			livesNumber.setString(to_string(playerLives));

			window.draw(livesText);
			window.draw(livesNumber);
			window.draw(scoreText);
			window.draw(scoreNumber);

		}

		window.display();
	}

	return 0;
}
