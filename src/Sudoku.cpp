/*
 * Sudoku solver
 */

#include <cstdlib>
#include <array>
#include <SFML/Graphics.hpp>

#include "Board.h"

int main()
{
    const unsigned int windowX = 900;
    const unsigned int windowY = 900;

    int nFPS = 60;
    sf::Clock clock;

    sf::Vector2i mousePos;
    bool buttonPressed;
    bool buttonReleased;
    int cellValue = -1;

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
            mousePos =
            { event.mouseMove.x, event.mouseMove.y };

        buttonPressed = false;
        buttonReleased = false;

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                buttonPressed = true;
                mousePos =
                { event.mouseButton.x, event.mouseButton.y };
            }
            else
                buttonPressed = false;
        }

        if (event.type == sf::Event::MouseButtonReleased)
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                buttonReleased = true;
                mousePos =
                { event.mouseButton.x, event.mouseButton.y };
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
            cellValue = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            cellValue = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            cellValue = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            cellValue = 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            cellValue = 4;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
            cellValue = 5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
            cellValue = 6;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
            cellValue = 7;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
            cellValue = 8;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
            cellValue = 9;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            cellValue = 10; // toggle Lock of field

        board.mouseAction(mousePos, buttonPressed, buttonReleased);
        if (cellValue >= 0)
            board.setCellValue(cellValue);
        cellValue = -1;

        window.clear();

        // board.draw(window);
        board.draw(window);

        window.display();

    }

    return (EXIT_SUCCESS);
}
