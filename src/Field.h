#pragma once

#include <string>

#include <sstream>
#include <SFML/Graphics.hpp>


#include <iostream>

class Field
{
public:
	Field(sf::Font&);

private:
	int value;
	std::vector<int> possibilities;
	bool focus;
	bool lock;

	sf::Vector2f position;
	sf::Vector2f size;

	sf::RectangleShape rect;
    sf::Text valueText;
	sf::Color defaultColor;
	sf::Color focusColor;
	sf::Color lockColor;
	sf::Color color;
    sf::Font fontConsolas;
    sf::Vector2f valueTextOffset;

    void setColor();

public:
	void setValue(int);
	int getValue() const;
	void toggleLock();
	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition() const;
	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;

	std::string getText() const;
};
