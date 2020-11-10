#pragma once

#include <iostream>

#include <array>
#include <vector>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Resources.h"
#include "State.h"
#include "Cell.h"

class Board
{
public:
    Board();

private:
    //board representation
    // ----------------------------------------------------------------------------
    std::array<std::unique_ptr<Cell>, 81> board;


    // user interaction (mouse, keyboard)


    // graphic
    // ----------------------------------------------------------------------------
    sf::Vector2f offset;                                    // offset to drawing window origin
    std::vector<sf::RectangleShape> vecRegionSeparator;     // used to highlight the 9 regions / squares
    sf::Color regionSeparatorColor;
    sf::Vector2f cellSize;
    sf::Vector2f cellDistance;
    FontConsolas fontConsolas;                              // font used to display numbers


public:
    //board representation
    // ----------------------------------------------------------------------------
    sf::Vector2i calculateXY(int);
    void setCellsToDefault();                               //
    void checkCellValueIntegrity();                         // checks, if 2 or more cells within a cluster have the same value
    void cleanupPossibleValues();                           //
    void searchNakedSingles();


    // user interaction
    // ----------------------------------------------------------------------------
    bool mouseAction(sf::Vector2i, bool, bool);
    void setCellValue(int);


    // graphic
    // ----------------------------------------------------------------------------
    void draw(sf::RenderTarget& target) const;

};
