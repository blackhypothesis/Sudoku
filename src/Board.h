#pragma once

#include <iostream>

#include <vector>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Cell.h"
#include "Resources.h"

class Board
{
public:
	Board();

private:
	std::array<std::array<std::unique_ptr<Cell>, 9>, 9> board;
	std::vector<sf::RectangleShape> vecSeparator;
	sf::Color separatorColor;
	const sf::Vector2f fieldSize;
	const sf::Vector2f offset;
	const sf::Vector2f distance;
	FontConsolas fontConsolas;
	bool setValueAllowed;


public:
	bool mouseAction(sf::Vector2i, bool, bool, int);
	void initCellsToDefault();
	void calculateIfSinglePossibleCellValue();
	void calculateIfSinglePossibleClusterValues();
	void calculatePairValue();
	int getSquareClusterNumber(int, int);
	void draw(sf::RenderTarget& target) const;

};
