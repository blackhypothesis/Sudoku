#include "Field.h"

Field::Field(sf::Font& fc) :
		value(0),
		focus(false),
		lock(false),
		size(sf::Vector2f(80.0f, 80.0f)),
		defaultColor(sf::Color(50, 50, 50)),
		focusColor(sf::Color(100, 100, 50)),
		lockColor(sf::Color(50, 100, 230)),
		valueTextOffset(sf::Vector2f(25, 7))
{
	this->fontConsolas = fc;
	rect.setSize(sf::Vector2f(size));
	color = defaultColor;
	setColor();

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


void Field::toggleLock()
{
	lock = !lock;

	if (lock)
		color = lockColor;
	else
		color = defaultColor;

	setColor();

	std::cout << "lock toggled: " << lock << std::endl;

}


void Field::setPosition(sf::Vector2f position)
{
	this->position = position;
	rect.setPosition(position);
	valueText.setPosition(position + valueTextOffset);
}


bool Field::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	if (!(mousePos.x > position.x && mousePos.x < position.x + size.x && mousePos.y > position.y && mousePos.y < position.y + size.y))
	{
		focus = false;
	}
	else
	{
		focus = true;
	}

	if (focus)
	{
		color = focusColor;
		setColor();
		return true;
	}
	else
	{
		if(!lock)
		{
			color = defaultColor;
		}
		else
		{
			color = lockColor;
		}
		setColor();
		return false;
	}
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


void Field::setColor()
{
	rect.setFillColor(color);
}

