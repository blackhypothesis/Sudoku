#include <cstdlib>
#include <array>
#include <SFML/Graphics.hpp>

#include "Board.h"

int main()
{
	int nFPS = 10;
	sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(900, 900), "Sudoku");
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

		window.clear();

		board.draw(window);

		window.display();

	}

	return(EXIT_SUCCESS);
}


