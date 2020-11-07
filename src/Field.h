#pragma once

#include <string>

#include <sstream>
#include <SFML/Graphics.hpp>


//#include <iostream>

class Field
{
public:
	Field(sf::Font&);

private:
	int value;
	sf::Vector2i position;

	struct Possibility
	{
		sf::Vector2f drawPosition;
		int possibleValue;
		bool isPossible;
		sf::Text possibleValueText;
	};

	std::array<Possibility, 10> aPossibility;

	bool focus;
	bool lock;

	sf::Vector2f drawPosition;
	const sf::Vector2f size;

	sf::RectangleShape rect;
    sf::Text valueText;
	const sf::Color defaultColor;
	const sf::Color focusColor;
	const sf::Color lockColor;
	sf::Color color;
    const sf::Font fontConsolas;
    const sf::Vector2f valueTextOffset;

public:
	void setValue(int);
	int getValue() const;

	void setPosition(sf::Vector2i);
	sf::Vector2i getPosition() const;

	void toggleLock();
	void setDrawPosition(sf::Vector2f);
	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;

	std::string getText() const;
};
