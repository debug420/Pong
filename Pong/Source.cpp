#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>

static const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
static const sf::Vector2f middleOfScreen = sf::Vector2f(desktopMode.width / 2, desktopMode.height / 2);
static const float playerSpeed = 18.f;
static const float ballSpeed = 3.8f;

template <typename T> static void RenderVector(sf::RenderWindow& windowl, std::vector<T>& vector);
template <typename T> static void Draw(T vArray, std::vector<T>& vector);
static sf::VertexArray getQuad(sf::Vector2f pos, sf::Vector2f size);
static sf::CircleShape getCircle(sf::Vector2f pos, float radius);
static int random(int min, int max);

static struct bindingY
{
	sf::Keyboard::Key yUp;
	sf::Keyboard::Key yDown;
	bindingY(sf::Keyboard::Key u = sf::Keyboard::Key::W, sf::Keyboard::Key d = sf::Keyboard::Key::S)
	{
		yUp = u;
		yDown = d;
	}
};

static class ball
{
public:
	sf::Vector2f pos;
	float xVelocity = ballSpeed;
	float yVelocity = ballSpeed;

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

	ball()
	{
		resetVelocity();
		pos = middleOfScreen;
	}

	void update()
	{
		pos += sf::Vector2f(xVelocity, yVelocity) * ballSpeed;
	}
};

static class player
{
public:

	sf::Vector2f pos;
	void move(sf::Vector2f incFactor)
	{
		pos += incFactor;
	};

	bool nControl;
	unsigned int score = 0;
	bindingY bindings;

	bool canGoUp()
	{
		if (pos.y - 80 > 0)
			return true;
		else return false;
	}

	bool canGoDown()
	{
		if (pos.y + 80 < desktopMode.height)
			return true;
		else return false;
	}

	player(sf::Vector2f p = middleOfScreen, bool normalControl = true)
	{
		pos = p;
		nControl = normalControl;
		if (!nControl)
		{
			bindings.yDown = sf::Keyboard::Key::Down;
			bindings.yUp = sf::Keyboard::Key::Up;
		}
	}

};

// Start of program
int main()
{

	// Var declerations and the creation of window
	sf::RenderWindow mainWindow;
	std::vector<sf::VertexArray> playerRenders;
	std::vector<sf::CircleShape> circleRenders;
	std::vector<sf::Text> textRenders;
	mainWindow.create(sf::VideoMode(desktopMode.width, desktopMode.height), "Pong", sf::Style::Fullscreen);
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

	player p1(middleOfScreen - sf::Vector2f((desktopMode.width / 2) - 100, 0), true);
	player p2(middleOfScreen + sf::Vector2f((desktopMode.width / 2) - 100, 0), false);
	ball mainBall;

	while (mainWindow.isOpen())
	{
		sf::Event mainEvent;
		if (mainWindow.pollEvent(mainEvent))
		{
			switch (mainEvent.type)
			{
			case sf::Event::Closed:
				mainWindow.close();
			}
		}

		// Player 1 movement handler
		if (sf::Keyboard::isKeyPressed(p1.bindings.yUp) && p1.canGoUp())
		{
			p1.move(sf::Vector2f(0, -playerSpeed));
		}
		else if (sf::Keyboard::isKeyPressed(p1.bindings.yDown) && p1.canGoDown())
		{
			p1.move(sf::Vector2f(0, playerSpeed));
		}

		// Player 2 movement handler
		if (sf::Keyboard::isKeyPressed(p2.bindings.yUp) && p2.canGoUp())
		{
			p2.move(sf::Vector2f(0, -playerSpeed));
		}
		else if (sf::Keyboard::isKeyPressed(p2.bindings.yDown) && p2.canGoDown())
		{
			p2.move(sf::Vector2f(0, playerSpeed));
		}

		// Handle ball physics and rebounding
		if (mainBall.pos.x <= 0 || mainBall.pos.x >= desktopMode.width)
		{
			p1.pos = middleOfScreen - sf::Vector2f((desktopMode.width / 2) - 100, 0);
			p2.pos = middleOfScreen + sf::Vector2f((desktopMode.width / 2) - 100, 0);
			mainBall.pos = middleOfScreen;
			mainBall.resetVelocity();
			//std::this_thread::sleep_for(std::chrono::seconds(1));

			p1.score = 0;
			p2.score = 0;
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

		mainWindow.clear(sf::Color::Black);
		mainBall.update();

		Draw<sf::VertexArray>(getQuad(p1.pos, sf::Vector2f(5, 80)), playerRenders);
		Draw<sf::VertexArray>(getQuad(p2.pos, sf::Vector2f(5, 80)), playerRenders);
		Draw<sf::CircleShape>(getCircle(mainBall.pos, 10), circleRenders);

		// Process scoreboard
		sf::Text scoreBoard;
		scoreBoard.setFont(mainFont);
		scoreBoard.setString(std::to_string(p1.score) + " | " + std::to_string(p2.score));
		scoreBoard.setOrigin(scoreBoard.getLocalBounds().width / 2, scoreBoard.getLocalBounds().height / 2);
		scoreBoard.setPosition(middleOfScreen);
		Draw<sf::Text>(scoreBoard, textRenders);

		// Handles collision with the sticks
		sf::FloatRect gPosofBall = circleRenders[0].getGlobalBounds();
		if (gPosofBall.intersects(playerRenders[0].getBounds()))
		{
			mainBall.xVelocity = -mainBall.xVelocity;
			p1.score++;
		}

		if (gPosofBall.intersects(playerRenders[1].getBounds()))
		{
			mainBall.xVelocity = -mainBall.xVelocity;
			p2.score++;
		}

		RenderVector<sf::VertexArray>(mainWindow, playerRenders);
		RenderVector<sf::CircleShape>(mainWindow, circleRenders);
		RenderVector<sf::Text>(mainWindow, textRenders);
		mainWindow.display();
	}

	return 0;
}

template <typename T>
static void RenderVector(sf::RenderWindow& window, std::vector<T>& vector)
{
	typename std::vector<T>::iterator vectorIt = vector.begin();
	for (; vectorIt != vector.end(); vectorIt++)
	{
		window.draw(*vectorIt);
	}
	std::vector<T> tempVector;
	vector = tempVector;
}

template <typename T >
static void Draw(T obj, std::vector<T>& vector)
{
	vector.push_back(obj);
}

static sf::VertexArray getQuad(sf::Vector2f pos, sf::Vector2f size)
{
	sf::VertexArray VArr(sf::Quads, (unsigned)4);
	VArr[0].position = pos + sf::Vector2f(-(size.x), -(size.y));
	VArr[1].position = pos + sf::Vector2f(size.x, -(size.y));
	VArr[2].position = pos + sf::Vector2f(size.x, size.y);
	VArr[3].position = pos + sf::Vector2f(-(size.x), size.y);
	return VArr;
}

static sf::CircleShape getCircle(sf::Vector2f pos, float radius)
{
	sf::CircleShape circle;
	circle.setRadius(radius);
	circle.setPosition(pos);
	circle.setOrigin(circle.getLocalBounds().width / 2, circle.getLocalBounds().height / 2);
	return circle;
}

static int random(int min, int max)
{
	std::default_random_engine gen(std::random_device{}());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}