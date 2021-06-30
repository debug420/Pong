#pragma once
#include <SFML/Graphics.hpp>

extern const float ballSpeed;
extern const float playerSpeed;
extern const sf::Vector2f middleOfScreen;
extern const sf::VideoMode desktopMode;

extern int random(int, int);

////////////////////////////////////////////////////////////////////
/// Ball class
////////////////////////////////////////////////////////////////////

class ball
{
public:

	// ball vars
	sf::Vector2f pos;
	float xVelocity = ballSpeed;
	float yVelocity = ballSpeed;
	sf::CircleShape shape;

	// ball methods
	void resetVelocity()
	{
		switch (random(1, 3))
		{
		case 1:
			xVelocity = -xVelocity;
		case 2:
			yVelocity = -yVelocity;
		case 3:
			xVelocity = -xVelocity;
			yVelocity = -yVelocity;
		}
	}

	void update()
	{
		pos += sf::Vector2f(xVelocity, yVelocity) * ballSpeed;
	}

	ball(sf::CircleShape circle)
		: pos(middleOfScreen),
		shape(circle)
	{ resetVelocity(); }

};

////////////////////////////////////////////////////////////////////
/// Player Class
////////////////////////////////////////////////////////////////////

class player
{

public:

	// player vars
	sf::Vector2f pos;
	unsigned int score = 0;
	sf::RectangleShape shape;

	// player methods
	void move(sf::Vector2f incFactor)
	{
		pos += incFactor;
	};

	bool canGoUp()
	{
		if (pos.y >= 0)
			return true;
		else return false;
	}

	bool canGoDown()
	{
		if (pos.y + this->shape.getSize().y <= desktopMode.height)
			return true;
		else return false;
	}

	player(sf::Vector2f p, sf::RectangleShape rect)
		: pos(p),
		shape(rect)
	{};

};