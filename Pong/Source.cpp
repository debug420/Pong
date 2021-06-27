#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include "Classes.h"

const std::string titleOfGame = "Pong (C++)";

// Predefine variables and functions (prototypes)
const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
const sf::Vector2f middleOfScreen = sf::Vector2f(desktopMode.width / 2, desktopMode.height / 2);
const float playerSpeed = 15.f;
const float ballSpeed = 3.5f;

sf::CircleShape getCircle(sf::Vector2f pos, float radius);
sf::RectangleShape getRectangle(sf::Vector2f pos, float x, float y);
sf::Text getText(sf::Vector2f pos, sf::Font &font, std::string textStr);
int random(int min, int max);
void resetGame(player& p1, player& p2, ball& b, const sf::Vector2f& p1StartPos
	, const sf::Vector2f& p2StartPos);

////////////////////////////////////////////////////////////////////
/// Start of Main
////////////////////////////////////////////////////////////////////

int main()
{

	// Creates window
	sf::RenderWindow mainWindow;
	mainWindow.create(sf::VideoMode(desktopMode.width, desktopMode.height), titleOfGame, sf::Style::Fullscreen);
	mainWindow.setFramerateLimit(60);
	mainWindow.setVerticalSyncEnabled(true);

	// Load font (ARIAL)
	// First attempts to find fond from windows fonts. If it cant find it there, it will
	// look in the folder the exe is inside.
	sf::Font mainFont;
	if (!mainFont.loadFromFile("C:/Windows/fonts/arial.ttf"))
	{
		if (!mainFont.loadFromFile("arial.ttf"))
		{
			std::cout << "Attempted to look for arial font in both windows fonts and game folder. None found. Exitting..." << std::endl;
			return 0;
		}
	}

	// Create the 2 players and the ball class as well as any other renderable instances
	const sf::Vector2f p1StartPos = middleOfScreen - sf::Vector2f((desktopMode.width / 2) - 100, 0);
	const sf::Vector2f p2StartPos = middleOfScreen + sf::Vector2f((desktopMode.width / 2) - 100, 0);
	//--------------------------------------------------------//
	player p1(p1StartPos, getRectangle(p1StartPos, 7, 120));
	player p2(p2StartPos, getRectangle(p2StartPos, 7, 120));
	ball mainBall(getCircle(middleOfScreen, 10));
	//--------------------------------------------------------//
	sf::Text scoreBoard = getText(middleOfScreen, mainFont, "0 | 0");
	sf::Text title = getText(middleOfScreen - sf::Vector2f(0, 40), mainFont, titleOfGame);
	title.setCharacterSize(25);

	////////////////////////////////////////////////////////////////////
	/// Main Loop
	////////////////////////////////////////////////////////////////////

	while (mainWindow.isOpen())
	{

		// Event handler
		sf::Event mainEvent;
		if (mainWindow.pollEvent(mainEvent))
		{
			switch (mainEvent.type)
			{
			case sf::Event::Closed:
				mainWindow.close();
				break;
			}
		}

		//--------------------------------------------------------//

		// Player 1 movement handler
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && p1.canGoUp())
		{
			p1.move(sf::Vector2f(0, -playerSpeed));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && p1.canGoDown())
		{
			p1.move(sf::Vector2f(0, playerSpeed));
		}

		// Player 2 movement handler
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && p2.canGoUp())
		{
			p2.move(sf::Vector2f(0, -playerSpeed));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && p2.canGoDown())
		{
			p2.move(sf::Vector2f(0, playerSpeed));
		}

		//--------------------------------------------------------//
		// Handle ball physics and rebounding

		if (mainBall.pos.x <= 0)
		{
			p2.score++;
			resetGame(p1, p2, mainBall, p1StartPos, p2StartPos);
		}
		if (mainBall.pos.x >= desktopMode.width)
		{
			p1.score++;
			resetGame(p1, p2, mainBall, p1StartPos, p2StartPos);
		}

		if (mainBall.pos.y <= 0 || mainBall.pos.y >= desktopMode.height)
		{
			mainBall.yVelocity = -mainBall.yVelocity;
			/*
			if (mainBall.xVelocity > 0)
				mainBall.xVelocity = random(290, 310) / 100;
			else
				mainBall.xVelocity = -(random(290, 310) / 100);
			*/
		}

		//--------------------------------------------------------//
		// Update objects for rendering
		mainBall.update();
		mainBall.shape.setPosition(mainBall.pos);
		p1.shape.setPosition(p1.pos);
		p2.shape.setPosition(p2.pos);
		scoreBoard.setString(std::to_string(p1.score) + " | " + std::to_string(p2.score));

		// Handles collision with the sticks
		sf::FloatRect gPosofBall = mainBall.shape.getGlobalBounds();
		if (gPosofBall.intersects(p1.shape.getGlobalBounds())
			|| gPosofBall.intersects(p2.shape.getGlobalBounds()))
		{
			mainBall.xVelocity = -mainBall.xVelocity;
		}

		//--------------------------------------------------------//
		// Render onto screen
		mainWindow.clear(sf::Color::Black);

		mainWindow.draw(mainBall.shape);
		mainWindow.draw(p1.shape);
		mainWindow.draw(p2.shape);
		mainWindow.draw(scoreBoard);
		mainWindow.draw(title);

		mainWindow.display();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////
/// Function Definitions
////////////////////////////////////////////////////////////////////

sf::RectangleShape getRectangle(sf::Vector2f pos, float x, float y)
{
	sf::RectangleShape rectangle;
	rectangle.setPosition(pos);
	rectangle.setSize(sf::Vector2f(x, y));
	rectangle.setFillColor(sf::Color::White);
	return rectangle;
}

sf::CircleShape getCircle(sf::Vector2f pos, float radius)
{
	sf::CircleShape circle;
	circle.setRadius(radius);
	circle.setPosition(pos);
	circle.setOrigin(circle.getLocalBounds().width / 2, circle.getLocalBounds().height / 2);
	return circle;
}

sf::Text getText(sf::Vector2f pos, sf::Font &font, std::string textStr)
{
	sf::Text text;
	text.setFont(font);
	text.setString(textStr);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(pos);
	return text;
}

int random(int min, int max)
{
	std::default_random_engine gen(std::random_device{}());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

void resetGame(player& p1, player& p2, ball& b, const sf::Vector2f &p1StartPos
	, const sf::Vector2f& p2StartPos)
{
	p1.pos = p1StartPos;
	p2.pos = p2StartPos;
	b.pos = middleOfScreen;
	b.resetVelocity();
}