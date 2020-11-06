#include "Field.h"

Field::Field(sf::Font& fc) :
		value(0),
		size(sf::Vector2f(80.0f, 80.0f)),
		defaultColor(sf::Color(50, 50, 50)),
		valueTextOffset(sf::Vector2f(25, 7))
{
	this->fontConsolas = fc;
	rect.setSize(sf::Vector2f(size));
	color = defaultColor;
	rect.setFillColor(defaultColor);

    // fontConsolas.loadFromFile("/home/marcel/workspace/Sudoku/assets/Consolas.ttf");

    valueText.setFont(fontConsolas);

    valueText.setCharacterSize(50);
    valueText.setFillColor(sf::Color::White);


    setValue(value);
	setPosition(sf::Vector2f(10, 10));
}

void Field::setValue(int value)
{
	this->value = value;

    if (value > 0)
    {
		std::stringstream ss;
		ss << std::fixed << value;
		valueText.setString(ss.str());
    }
    else
    {
    	valueText.setString(" ");
    }
}

int Field::getValue() const
{
	return value;
}

void Field::setPosition(sf::Vector2f position)
{
	rect.setPosition(position);
	valueText.setPosition(position + valueTextOffset);
}

void Field::draw(sf::RenderTarget& target) const
{
	target.draw(rect);
	target.draw(valueText);
}


std::string Field::getText() const
{
	return valueText.getString();
}
