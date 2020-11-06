#pragma once

#include <iostream>

#include <vector>
#include <SFML/Graphics.hpp>

#include "Resources.h"
#include "Field.h"

class Board
{
public:
	Board();

private:
	std::array<std::array<std::unique_ptr<Field>, 9>, 9> board;
	std::vector<sf::RectangleShape> vecSeparator;
	sf::Color separatorColor;
	const sf::Vector2f fieldSize;
	const sf::Vector2f offset;
	const sf::Vector2f distance;
	FontConsolas fontConsolas;
	bool enableSetValue;


public:
	bool mouseAction(sf::Vector2i, bool, bool, int);
	void draw(sf::RenderTarget& target) const;

};
