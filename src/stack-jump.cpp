//============================================================================
// Name        : stack-jump.cpp
// Author      : Abir Ganguly.
// Version     :
// Copyright   : Your copyright notice
// Description : Stack jump game application entry point.
//============================================================================
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "settings.h"

/**
 *
 * @Author : Abir Ganguly.
 *
 * License :
 *
 */

class Player : public sf::Drawable
{
public:
	float velocityY = 0.0;
	sf::Sprite playerSprite;
	float initialPositionY;
	const std::vector<sf::IntRect> &animationTextureRects;
	unsigned int animationIndex = 0;
	unsigned int time_animation = 0;
	float animation_fps = 5;
	bool jumping = false;
	bool lastJumping = false;
	Player(sf::Texture &playerTexture, const std::vector<sf::IntRect> &animationTextureRects) : playerSprite(playerTexture), animationTextureRects(animationTextureRects)
	{
		playerSprite.setTextureRect(animationTextureRects.at(0));
		playerSprite.setPosition((WINDOW_WIDTH / 2.0) - (playerSprite.getLocalBounds().width / 2.0), (WINDOW_HEIGHT / 2.0) - (playerSprite.getLocalBounds().height)); // place at bottom-middle of window
		//std::cout << playerSprite.setPosition((WINDOW_WIDTH / 2.0) - (playerSprite.getLocalBounds().width / 2.0), (WINDOW_HEIGHT - playerSprite.getLocalBounds().height)) << std::endl;
		// initialPositionY = playerSprite.getGlobalBounds().top;
		initialPositionY = (WINDOW_HEIGHT / 2.0) - (playerSprite.getLocalBounds().height);
	}
	void animate(int dt)
	{
		time_animation += dt;

		if (time_animation > 1000 / animation_fps)
		{
			time_animation = 0;
			playerSprite.setTextureRect(animationTextureRects.at(animationIndex));

			animationIndex++;

			if (animationIndex >= (animationTextureRects.size() - 1))
				animationIndex = 0;
		}
	}

	void jump(int dt)
	{
		velocityY = -PLAYER_JUMP_VELOCITY;
		// animate(dt);
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		target.draw(playerSprite);
	}
};

class Ground : public sf::Drawable
{
public:
	sf::Sprite groundSprite;
	float initialPositionY;
	Ground(sf::Texture &groundTexture) : groundSprite(groundTexture)
	{
		// groundSprite.setTextureRect(animationTextureRects.at(0));
		groundSprite.setPosition((WINDOW_WIDTH / 2.0) - (groundSprite.getLocalBounds().width / 2.0), (WINDOW_HEIGHT / 2.0)); // place at bottom-middle of window
		//std::cout << playerSprite.setPosition((WINDOW_WIDTH / 2.0) - (playerSprite.getLocalBounds().width / 2.0), (WINDOW_HEIGHT - playerSprite.getLocalBounds().height)) << std::endl;
		// initialPositionY = playerSprite.getGlobalBounds().top;
		initialPositionY = (WINDOW_HEIGHT / 2.0);
	}
	void pushDown()
	{
		groundSprite.setPosition(groundSprite.getPosition().x, groundSprite.getPosition().y + PLANK_TEXTURE_WIDTH);
	}
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		target.draw(groundSprite);
	}
};
class Plank : public sf::Drawable
{
public:
	sf::Sprite plankSprite;
	float initialPositionY;
	float initialPositionX;
	float velocityX = 0.0;
	Plank()
	{
		// plankSprite = nullptr;
		velocityX = 0.0;
	}
	Plank(sf::Texture &plankTexture) : plankSprite(plankTexture)
	{
		// groundSprite.setTextureRect(animationTextureRects.at(0));
		plankSprite.setPosition(WINDOW_WIDTH, (WINDOW_HEIGHT / 2.0) - (plankSprite.getLocalBounds().height));
		// place at bottom-middle of window

		// std::cout << playerSprite.setPosition((WINDOW_WIDTH / 2.0) - (playerSprite.getLocalBounds().width / 2.0), (WINDOW_HEIGHT - playerSprite.getLocalBounds().height)) << std::endl;
		initialPositionX = WINDOW_WIDTH;

		initialPositionY = (WINDOW_HEIGHT / 2.0) - (plankSprite.getLocalBounds().height);
		// initialPositionY = (WINDOW_HEIGHT / 2.0);
	}
	void pushDown()
	{
		plankSprite.setPosition(plankSprite.getPosition().x, plankSprite.getPosition().y + PLANK_TEXTURE_WIDTH);
	}
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		target.draw(plankSprite);
	}
};
// int update(int dt)
// {
// 	std::cout << "Updating" << std::endl;

// 		///////UPDATING VELOCITY & POSITION///////
// 		player.velocityY += ACCELERATION_DUE_TO_GRAVITY * dt;
// 		player.playerSprite.setPosition(player.playerSprite.getPosition().x, player.playerSprite.getPosition().y + player.velocityY * dt);

// 		///////COLLISION DETECTION///////
// 		if (player.playerSprite.getPosition().y >= (player.initialPositionY))
// 		{
// 			player.playerSprite.setPosition(player.playerSprite.getPosition().x, player.initialPositionY);
// 			player.velocityY = 0;
// 			player.animationIndex = 0;
// 			player.playerSprite.setTextureRect(player.animationTextureRects.at(player.animationIndex));
// 			if (player.jumping)
// 				std::cout << "hit the ground" << std::endl;

// 			player.jumping = false;
// 		}
// 		else
// 		{
// 			player.jumping = true;
// 			player.animate(dt);
// 		}
// }

int main()
{

	///////RENDER WINDOW///////
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Close);
	//window->setVerticalSyncEnabled(true); //Or else use : window->setFramerateLimit(60); (But don't use both at the same time)
	//window.setFramerateLimit(10);

	///////BACKGROUND///////
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile(BACKGROUND_TEXTURE_FILE))
	{
		std::cerr << "Failed to load texture " << BACKGROUND_TEXTURE_FILE << "\n";
		return EXIT_FAILURE;
	}
	sf::Sprite background(backgroundTexture);

	///////GROUND///////
	sf::Texture groundTexture;
	if (!groundTexture.loadFromFile(GROUND_TEXTURE_FILE))
	{
		std::cerr << "Failed to load texture " << GROUND_TEXTURE_FILE << "\n";
		return EXIT_FAILURE;
	}
	Ground ground(groundTexture);

	///////PLANK///////
	sf::Texture plankTexture;
	if (!plankTexture.loadFromFile(PLANK_TEXTURE_FILE))
	{
		std::cerr << "Failed to load texture " << PLANK_TEXTURE_FILE << "\n";
		return EXIT_FAILURE;
	}

	Plank plank_ar[NO_OF_PLANKS];
	for (int i = 0; i < NO_OF_PLANKS; i++)
	{
		plank_ar[i] = Plank(plankTexture);
	}
	unsigned int plankIndex = 0;
	float plankVelocityX = INITIAL_PLANK_VELOCITY_X;
	// Plank plank = plank_ar[0];
	// Plank lastPlank = plank_ar[0];
	// Plank plankForRender[10];

	// for (int i = 0; i < 10; i++)
	// {
	// 	window.draw(plank_ar[i]);
	// }
	// std::cerr << plank_ar[5].plankSprite.getGlobalBounds().height << PLANK_TEXTURE_FILE << "\n";

	plank_ar[plankIndex].velocityX = -plankVelocityX;

	///////PLAYER///////
	sf::Texture playerTexture;
	if (!playerTexture.loadFromFile(PLAYER_TEXTURE_FILE))
	{
		std::cerr << "Failed to load texture " << PLAYER_TEXTURE_FILE << "\n";
		return EXIT_FAILURE;
	}
	const std::vector<sf::IntRect> playerAnimationTextureRects{
		sf::IntRect(0, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT),
		sf::IntRect(PLAYER_TEXTURE_WIDTH, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT),
		sf::IntRect(0, PLAYER_TEXTURE_HEIGHT, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT),
		sf::IntRect(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT),
	};
	Player player(playerTexture, playerAnimationTextureRects);

	///////FPS///////
	sf::Clock clock;
	unsigned int frameCounter = 0, currentFPS;
	unsigned int dt = 0, time = 0, time_fps = 0;

	///////MAIN LOOP///////
	while (window.isOpen())
	{
		//CODE FOR FPS COUNTER
		if (time_fps >= 1000)
		{
			time_fps = 0;
			std::cout << "FPS : " << frameCounter << "\n";
			currentFPS = frameCounter;
			frameCounter = 0;
			// for (int i = 0; i < 10; i++)
			// {
			// 	std::cout << plank_ar[i].plankSprite.getPosition().x << "," << plank_ar[i].plankSprite.getPosition().y << std::endl;
			// 	// window.draw(plank_ar[i]);
			// }
			// std::cout << ground.groundSprite.getPosition().y << std::endl;
		}

		/////UPDATE TIME/////
		float dt = clock.getElapsedTime().asMilliseconds();
		time += dt;
		time_fps += dt;

		/////RESTARTING CLOCK/////
		clock.restart();

		///////EVENT HANDLING///////
		sf::Event event;

		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				if (event.key.code == sf::Keyboard::Space)
					player.jump(dt);
			}

			/*if (event.type == sf::Event::KeyReleased) playerTextureRect.left = 0;*/

			/*if (event.type == sf::Event::MouseButtonPressed) std::cout << event.mouseButton.button << " is pressed\n";

			if (event.type == sf::Event::MouseMoved) std::cout << "Mouse moved\n";*/
		}

		///////UPDATE///////
		// std::cout << "Updating" << std::endl;

		///////UPDATING VELOCITY & POSITION///////
		// plank = plank_ar[plankIndex];

		player.velocityY += ACCELERATION_DUE_TO_GRAVITY * dt;
		player.playerSprite.setPosition(player.playerSprite.getPosition().x, player.playerSprite.getPosition().y + player.velocityY * dt);
		plank_ar[plankIndex].plankSprite.setPosition(plank_ar[plankIndex].plankSprite.getPosition().x + plank_ar[plankIndex].velocityX * dt, plank_ar[plankIndex].plankSprite.getPosition().y);

		// plank_ar[plankIndex] = plank;

		// std::cout << plank_ar[plankIndex].plankSprite.getPosition().y << std::endl;
		// std::cout << plankIndex << std::endl;

		if (plank_ar[plankIndex].plankSprite.getPosition().x < 0)
			plank_ar[plankIndex].velocityX = +plankVelocityX;
		if (plank_ar[plankIndex].plankSprite.getPosition().x + plank_ar[plankIndex].plankSprite.getLocalBounds().width > WINDOW_WIDTH)
			plank_ar[plankIndex].velocityX = -plankVelocityX;

		///////COLLISION DETECTION///////
		if (player.playerSprite.getPosition().y >= (player.initialPositionY))
		{
			player.playerSprite.setPosition(player.playerSprite.getPosition().x, player.initialPositionY);
			player.velocityY = 0;
			player.animationIndex = 0;
			player.playerSprite.setTextureRect(player.animationTextureRects.at(player.animationIndex));
			if (player.jumping)
			{
				if (plank_ar[plankIndex].plankSprite.getPosition().x < (player.playerSprite.getPosition().x + player.playerSprite.getGlobalBounds().width) &&			 //left
					player.playerSprite.getPosition().x < (plank_ar[plankIndex].plankSprite.getPosition().x + plank_ar[plankIndex].plankSprite.getGlobalBounds().width)) //right
				{
					std::cout << "You hit the plank" << std::endl;
					plank_ar[plankIndex].velocityX = 0;
					ground.pushDown();
					for (int i = 0; i < NO_OF_PLANKS; i++)
					{
						plank_ar[i].pushDown();
					}

					// plank_ar[plankIndex].pushDown();
					// plank_ar[plankIndex].plankSprite.setPosition(plank_ar[plankIndex].plankSprite.getPosition().x, plank_ar[plankIndex].plankSprite.getPosition().y + PLANK_TEXTURE_WIDTH);

					// lastPlank = plank;
					// plank_ar[plankIndex] = plank;
					// plank_ar[plankIndex].plankSprite.setPosition(plank.plankSprite.getPosition().x, plank.plankSprite.getPosition().y);
					plankIndex++;
					// plank = plank_ar[plankIndex];
					if (plankIndex >= NO_OF_PLANKS)
						plankIndex = 0;

					plankVelocityX+=RATE_OF_INCREASE_IN_PLANK_VELOCITY_X*dt;
					plank_ar[plankIndex].velocityX = -plankVelocityX;
					plank_ar[plankIndex].plankSprite.setPosition(plank_ar[plankIndex].initialPositionX, plank_ar[plankIndex].initialPositionY);
				}
			}

			if ((plank_ar[plankIndex].plankSprite.getPosition().x < (player.playerSprite.getPosition().x + player.playerSprite.getGlobalBounds().width) && //left
				 player.playerSprite.getPosition().x < (plank_ar[plankIndex].plankSprite.getPosition().x + plank_ar[plankIndex].plankSprite.getGlobalBounds().width)) &&
				!player.jumping)
			{
				std::cout << "You fall off the stack" << std::endl;
				return EXIT_SUCCESS;
			}

			player.jumping = false;
		}
		else
		{

			player.animate(dt);
			player.jumping = true;
		}
		// player.lastJumping=player.jumping;

		///////////////////////////////////////////

		///////CLEARING SCREEN AND RENDERING///////
		// window.clear();
		window.draw(background);
		window.draw(ground);

		for (int i = 0; i < NO_OF_PLANKS; i++)
		{
			window.draw(plank_ar[i]);
		}
		// std::cout << plank_ar[0].plankSprite.getGlobalBounds().height << std::endl;

		// window.draw(plank);
		// window.draw(lastPlank);

		// window.draw(plank_ar[0]);

		window.draw(player);

		window.display();
		frameCounter += 1;
	}
	return EXIT_SUCCESS;
}
