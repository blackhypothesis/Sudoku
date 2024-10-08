#pragma once

#include <iostream>

#include <chrono>
#include <thread>
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
    std::array<std::shared_ptr<Cell>, 81> board;


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
    void loadGame(std::array<int, 81>);
    void setCellsToDefault();                               //
    void cleanupPossibleValues();                           //
    void checkCellValueIntegrity();                         // checks, if 2 or more cells within a cluster have the same value
    bool searchForNakedSingles();                           // search for NAKED_SINGLE and HIDDEN_SINGLE
    bool searchForHiddenSingles();                          // search for NAKED_SINGLE and HIDDEN_SINGLE
    bool searchForNakedPairs();                             // search for NAKED_PAIR
    bool searchForHiddenPairs();
    void removePossibleValuesDiscarded();                   // remove possible values


    // user interaction
    // ----------------------------------------------------------------------------
    bool mouseAction(sf::Vector2i, bool, bool);
    bool mouseActionEnabled;
    int currentAction;
    void performAction(std::string);


    // graphic
    // ----------------------------------------------------------------------------
    void draw(sf::RenderTarget& target) const;

private:
    //board representation
    // ----------------------------------------------------------------------------

    // helper functions
    std::array<std::array<std::array<unsigned int, 9>, 9>, 3> countOccurenceOfPossibleValueInEachCluster();

};
