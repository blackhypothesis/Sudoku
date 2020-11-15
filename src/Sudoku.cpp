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
    std::string actionParam;

    sf::RenderWindow window;
    window.create(sf::VideoMode(windowX, windowY), "Sudoku");
    window.setFramerateLimit(nFPS);

    FontConsolas fontConsolas;

    Board board;
    board.loadGame(std::array<int, 81>
    //{ 4, 1, 0, 0, 7, 0, 0, 0, 5, 0, 8, 0, 0, 0, 6, 0, 9, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 7, 4, 0, 1, 3, 0, 0, 5, 3, 0, 0, 0, 0, 0, 1, 2, 0, 0, 4, 3, 0, 8,
    //        7, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 9, 0, 8, 0, 0, 0, 7, 0, 7, 0, 0, 0, 6, 0, 0, 2, 8});

    //        { 4, 0, 0, 6, 0, 0, 3, 0, 0, 0, 0, 2, 8, 0, 0, 4, 0, 0, 3, 0, 0, 5, 9, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 3, 0, 0, 1, 5, 1, 0, 0, 9,
    //                0, 0, 0, 0, 4, 0, 0, 0, 1, 7, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 0, 9, 0, 0, 0, 0, 0,  0, 3 });

    //        { 0, 8, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 6, 0, 9, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 9, 1, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 5, 0, 6, 1, 0,
    //                0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 8, 4, 7, 0, 0, 2, 0, 0, 0, 9, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0 });

    //        { 0, 0, 4, 7, 1, 0, 0, 6, 0, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 8, 0, 0, 4, 0, 0, 0, 0, 3, 0, 2, 9, 0, 0, 0, 0, 0, 4, 0, 0, 6, 5,
    //                8, 0, 9, 0, 0, 0, 6, 3, 0, 0, 0, 8, 0, 7, 0, 0, 8, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0 });

            { 2, 0, 4, 7, 1, 0, 3, 6, 0, 6, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 5, 0, 6, 0, 0, 0, 0, 8, 7, 1, 4, 9, 2, 6, 5, 3, 5, 2, 9, 0, 0, 0, 0, 8, 4, 0, 0, 6, 5,
                    8, 0, 9, 0, 2, 0, 6, 3, 0, 0, 0, 8, 9, 7, 0, 5, 8, 0, 0, 0, 2, 4, 1, 0, 0, 2, 0, 0, 0, 5, 3, 6 });

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
            actionParam = "0";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            actionParam = "1";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            actionParam = "2";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            actionParam = "3";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            actionParam = "4";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
            actionParam = "5";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
            actionParam = "6";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
            actionParam = "7";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
            actionParam = "8";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
            actionParam = "9";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))        // perform all actions
            actionParam = "A";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))        // perform next action (step)
            actionParam = "N";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))        // all NAKED_SINGLE and HIDDEN_SINGLE to SOLVED
            actionParam = "M";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))        // reset actions
            actionParam = "R";

        board.mouseAction(mousePos, buttonPressed, buttonReleased);
        board.performAction(actionParam);
        actionParam = " ";

        window.clear();

        // board.draw(window);
        board.draw(window);

        window.display();

    }

    return (EXIT_SUCCESS);
}
