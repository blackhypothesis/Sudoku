#include <cstdlib>
#include <array>
#include <SFML/Graphics.hpp>

#include "Board.h"

int main()
{
	const unsigned int windowX = 900;
	const unsigned int windowY = 900;

	int nFPS = 30;
	sf::Clock clock;

	sf::Vector2i mousePos;
	bool buttonPressed;
	bool buttonReleased;
	int fieldValue = 0;

	sf::RenderWindow window;
	window.create(sf::VideoMode(windowX, windowY), "Sudoku");
	window.setFramerateLimit(nFPS);

	FontConsolas fontConsolas;

	Board board;

	while (window.isOpen())
	{
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		if (event.type == sf::Event::MouseMoved)
			mousePos = { event.mouseMove.x, event.mouseMove.y };

		buttonPressed = false;
		buttonReleased = false;

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				buttonPressed = true;
				mousePos = { event.mouseButton.x, event.mouseButton.y };
			}
			else
				buttonPressed = false;
		}

		if (event.type == sf::Event::MouseButtonReleased)
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				buttonReleased = true;
				mousePos = { event.mouseButton.x, event.mouseButton.y };
			}

		if (event.type == sf::Event::KeyPressed)
		{
			std::cout << "key pressed" << std::endl;

			if (event.key.code == sf::Keyboard::A)
				fieldValue = 0;
			if (event.key.code == sf::Keyboard::Num1)
				fieldValue = 1;
			if (event.key.code == sf::Keyboard::Num2)
				fieldValue = 2;
			if (event.key.code == sf::Keyboard::Num3)
				fieldValue = 3;
			if (event.key.code == sf::Keyboard::Num4)
				fieldValue = 4;
			if (event.key.code == sf::Keyboard::Num5)
				fieldValue = 5;
			if (event.key.code == sf::Keyboard::Num6)
				fieldValue = 6;
			if (event.key.code == sf::Keyboard::Num7)
				fieldValue = 7;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		    fieldValue = 1;

		std::cout << "fieldValue = " << fieldValue << std::endl;

		board.mouseAction(mousePos, buttonPressed, buttonReleased, fieldValue);


		window.clear();

		board.draw(window);

		window.display();

	}

	return(EXIT_SUCCESS);
}


