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
#include "Explosion.h"
#include "PowerUp.h"

using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;

const int maxBullets = 100;
const int numAsteroids = 10;
const int asteroidStartRadius = 30;
const int levelMultiplier = 5;
const float powerDrop = 0.15;
const float powerSlowFactor = 0.2;

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

/*
	Allocate a new array of asteroids for each level.
*/
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
	for (int i = 0; i < level; i++) {
		asteroids[i]->setMany(true);
	}
	for (int i = level; i < 2 * level; i++) {
		asteroids[i]->setSpray(true);
	}

	return asteroids;
}

int main() {

	// PARAMETERS
	// initial values
	int state = MENU; // for FSM
	int currentLevel = 1;
	int playerLives = 3;
	int score = 0;

	// key booleans
	bool left = false;
	bool right = false;
	bool up = false;
	bool space = false;

	// timing
	int dt_ms = 0;
	Clock clock;
	int thrustSoundTimer = 0;
	int doubleCollideTimer = 0;

	// SFX IMPORT
	SoundBuffer victoryBuffer;
	if (!victoryBuffer.loadFromFile("sweet sweet victory yeah.wav")) {
		exit(-1);
	}
	Sound sfx_victory;
	sfx_victory.setBuffer(victoryBuffer);

	SoundBuffer thrustBuffer;
	if (!thrustBuffer.loadFromFile("thrust1.wav")) {
		exit(-1);
	}
	Sound sfx_thrust;
	sfx_thrust.setBuffer(thrustBuffer);

	SoundBuffer explosionBuffer;
	if (!explosionBuffer.loadFromFile("explosion.wav")) {
		exit(-1);
	}
	Sound sfx_explosion;
	sfx_explosion.setBuffer(explosionBuffer);

	SoundBuffer dieBuffer;
	if (!dieBuffer.loadFromFile("thrust2.wav")) {
		exit(-1);
	}
	Sound sfx_hit;
	sfx_hit.setBuffer(dieBuffer);

	SoundBuffer shootBuffer;
	if (!shootBuffer.loadFromFile("shoot1.wav")) {
		exit(-1);
	}
	Sound sfx_shoot;
	sfx_shoot.setBuffer(shootBuffer);

	// TEXTURE IMPORT
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

	// STATIC OBJECTS
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

	// DYNAMIC OBJECTS
	Ship ship = Ship(Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f), Vector2f(0, 0), 20, &shipTexture);
	vector<Asteroid*> asteroids = prepAsteroids(currentLevel, &asteroidTexture);

	vector<Bullet> bullets;
	for (int i = 0; i < maxBullets; i++) {
		Bullet newBullet = Bullet(Vector2f(0, 0), Vector2f(0, 0));
		newBullet.setActive(false);
		bullets.push_back(newBullet);
	}

	vector<Explosion*> explosions;
	for (int i = 0; i < 20; i++) {
		explosions.push_back(new Explosion());
	}

	vector<PowerUp*> powerups;
	for (int i = 0; i < 10; i++) {
		powerups.push_back(new PowerUp());
	}

	// WINDOW
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
		
		// MENU STATE
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
				window.setKeyRepeatEnabled(false);

				// prep new asteroids and move to gameplay
				asteroids = prepAsteroids(currentLevel, &asteroidTexture);
				state = GAMEPLAY;
			}
		}
		// DEAD STATE
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
		// GAMEPLAY STATE
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
			for (int i = 0; i < explosions.size(); i++) {
				if (explosions[i]->isActive()) {
					explosions[i]->update();
				}
			}

			for (int i = 0; i < bullets.size(); i++) {
					if (space && !bullets[i].isActive()) { // bullet is inactive and we need one
					space = false;
					bullets[i].setPosition(ship.getPosition());
					bullets[i].setDirection(ship.getDirection());
					bullets[i].setActive(true);
					sfx_shoot.play();
				}

				if (bullets[i].isActive()) { // bullet is active
					bullets[i].update(dt_ms, WINDOW_WIDTH, WINDOW_HEIGHT);
				}
			}

			// COLLISION
			//powerups -> ship
			for (int i = 0; i < powerups.size(); i++) {
				if (powerups[i]->isActive()) {
					if (circlesCollided(powerups[i]->getPosition(), ship.getPosition(), powerups[i]->getRadius(), ship.getCollisionRadius())) {
						if (powerups[i]->getType() == 1) {
							window.setKeyRepeatEnabled(true); // rapid fire lol
						}
						else if (powerups[i]->getType() == 2) { // shield
							ship.setShield(true);
						}
						else if (powerups[i]->getType() == 3) { // slow all asteroids
							for (int i = 0; i < asteroids.size(); i++) {
								if (asteroids[i]->isActive()) {
									asteroids[i]->setVelocity(Vector2f(
										asteroids[i]->getVelocity().x * powerSlowFactor, 
										asteroids[i]->getVelocity().y * powerSlowFactor
									));
								}
							}
						}
						score += rand() % 90 + 10;
						powerups[i]->setActive(false);
					}
				}
			}

			// asteroids -> all
			for (int i = 0; i < asteroids.size(); i++) { // asteroids -> asteroids
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
					// asteroids -> ship
					if (doubleCollideTimer > 500) { // prevent multiple collisions over few frames
						if (circlesCollided(a->getPosition(), ship.getPosition(), a->getRadius(), ship.getCollisionRadius())) {
							if (!ship.getShield()) {
								playerLives--;
								for (int i = 0; i < explosions.size(); i++) {
									if (!explosions[i]->isActive()) {
										explosions[i]->setPosition(a->getPosition());
										explosions[i]->setActive(true);
										break;
									}
								}
								//ship.setPosition(Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
								if (playerLives <= 0) {
									state = DEAD;
								}
							}
							else {
								ship.setShield(false);
							}

							doubleCollideTimer = 0;
							sfx_hit.play();
						}
					}
					else {
						ship.setColor(Color::Blue);
						doubleCollideTimer++;
					}

					// asteroids -> bullets
					for (int i = 0; i < bullets.size(); i++) {
						if (bullets[i].isActive()) {
							if (circlesCollided(a->getPosition(), bullets[i].getPosition(), a->getRadius(), bullets[i].getRadius())) {
								score += rand() % 90 + 10;
								bullets[i].setActive(false);
								sfx_explosion.play();
								for (int i = 0; i < explosions.size(); i++) {
									if (!explosions[i]->isActive()) {
										explosions[i]->setPosition(a->getPosition());
										explosions[i]->setActive(true);
										break;
									}
								}
								float randVal = (float)rand() / RAND_MAX;
								if (randVal < powerDrop) { 
									float genType = rand() % 3;
									for (int i = 0; i < powerups.size(); i++) {
										if (!powerups[i]->isActive()) {
											powerups[i]->setPosition(a->getPosition());
											powerups[i]->setType(genType);
											powerups[i]->setActive(true);
										}
									}
								}

								if (a->getSize() > 1) {
									// get asteroid current pos and new radius
									Vector2f currentPos = a->getPosition();
									float newRadius = a->getRadius() / 2.0f;

									// generate two new positions and velocities
									Vector2f newpos1 = Vector2f(currentPos.x + a->getRadius() / 2.0f, currentPos.y + a->getRadius() / 2.0f);
									Vector2f newpos2 = Vector2f(currentPos.x - a->getRadius() / 2.0f, currentPos.y - a->getRadius() / 2.0f);

									Vector2f newvel1 = a->getVelocity();
									Vector2f newvel2 = Vector2f(-1.0f * a->getVelocity().x, -1.0f * a->getVelocity().y);

									if (a->isSpray()) { // spray asteroid
										vector<Bullet*> tempBullets;
										for (int i = 0; i < 4; i++) { // find four inactive bullets
											for (int j = 0; j < bullets.size(); j++) {
												if (!bullets[j].isActive()) {
													bullets[j].setActive(true);
													bullets[j].setPosition(a->getPosition()); // TODO: variable position?
													tempBullets.push_back(&bullets[j]);
													break;
												}
											}
										}
										
										// have to catch for if we don't find an active bullet
										if (tempBullets.size() > 0) {
											tempBullets[0]->setVelocity(a->getVelocity());
										}
										if (tempBullets.size() > 1) {
											tempBullets[1]->setVelocity(Vector2f(a->getVelocity().x, -1.0f * a->getVelocity().y));
										}
										if (tempBullets.size() > 2) {
											tempBullets[2]->setVelocity(Vector2f(-1.0f * a->getVelocity().x, a->getVelocity().y));
										}
										if (tempBullets.size() > 3) {
											tempBullets[3]->setVelocity(Vector2f(-1.0f * a->getVelocity().x, -1.0f * a->getVelocity().y));
										}

										a->setSpray(false);
									}

									if (a->isMany()) { // multi-asteroid
										Vector2f newpos3 = Vector2f(currentPos.x + a->getRadius() / 2.0f, currentPos.y - a->getRadius() / 2.0f);
										Vector2f newpos4 = Vector2f(currentPos.x - a->getRadius() / 2.0f, currentPos.y + a->getRadius() / 2.0f);

										Vector2f newvel3 = Vector2f(a->getVelocity().x, -1.0f * a->getVelocity().y);
										Vector2f newvel4 = Vector2f(-1.0f * a->getVelocity().x, a->getVelocity().y);

										Asteroid* spawned2;
										spawned2 = new Asteroid(newpos3, newvel3, newRadius, &asteroidTexture);
										spawned2->setSize(a->getSize() - 1);
										asteroids.push_back(spawned2);

										Asteroid* spawned3;
										spawned3 = new Asteroid(newpos4, newvel4, newRadius, &asteroidTexture);
										spawned3->setSize(a->getSize() - 1);
										asteroids.push_back(spawned3);

										a->setMany(false);
									}

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

			// WIN CHECK
			bool activeAsteroid = false;
			for (int i = 0; i < asteroids.size(); i++) {
				if (asteroids[i]->isActive()) {
					activeAsteroid = true;
					break;
				}
			}

			if (!activeAsteroid) {
				currentLevel++;

				// destroy current asteroids
				for (int i = 0; i < asteroids.size(); i++) {
					delete(asteroids[i]);
				}

				// create for next level
				asteroids = prepAsteroids(currentLevel, &asteroidTexture);
				
				// reset params
				doubleCollideTimer = 0;
				window.setKeyRepeatEnabled(false);

				sfx_victory.play();
			}

			// DRAW
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
			for (int i = 0; i < explosions.size(); i++) {
				if (explosions[i]->isActive()) {
					explosions[i]->draw(&window);
				}
			}
			for (int i = 0; i < powerups.size(); i++) {
				if (powerups[i]->isActive()) {
					powerups[i]->draw(&window);
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
