#pragma once

#include <string>

#include <sstream>
#include <SFML/Graphics.hpp>


class Field
{
public:
	Field(sf::Font&);

private:
	sf::RectangleShape rect;
    sf::Text valueText;
	int value;
	std::vector<int> possibilities;

	sf::Vector2f size;
	sf::Color defaultColor;
	sf::Color color;
    sf::Font fontConsolas;
    sf::Vector2f valueTextOffset;

public:
	void setValue(int);
	int getValue() const;
	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition() const;
	void draw(sf::RenderTarget&) const;

	std::string getText() const;
};
