#include "Field.h"

Field::Field(sf::Font &fc) :
		value(0), focus(false), lock(false), size(sf::Vector2f(80.0f, 80.0f)), defaultColor(sf::Color(50, 50, 50)), focusColor(sf::Color(100, 100, 50)), lockColor(
				sf::Color(50, 100, 230)), fontConsolas(fc), valueTextOffset(sf::Vector2f(25, 7))
{
	rect.setSize(sf::Vector2f(size));
	color = defaultColor;
	rect.setFillColor(color);

	valueText.setFont(fontConsolas);
	valueText.setCharacterSize(50);
	valueText.setFillColor(sf::Color::White);

	setValue(value);
	setDrawPosition(sf::Vector2f(10, 10));

	for (size_t i = 0; i < aPossibility.size(); i++)
	{
		if (i > 0)
		{
			aPossibility[i].drawPosition = sf::Vector2f(
					drawPosition + sf::Vector2f(((i - 1) % 3) * size.x / 3 + size.x / 10, (i - 1) / 3 * size.y / 3 + size.y / 30));
			aPossibility[i].possibleValueText.setPosition(aPossibility[i].drawPosition);
			aPossibility[i].possibleValueText.setFont(fontConsolas);
			aPossibility[i].possibleValueText.setCharacterSize(18);

			aPossibility[i].possibleValue = i;
			aPossibility[i].isPossible = true;
			std::stringstream ss;
			ss << std::fixed << aPossibility[i].possibleValue;
			aPossibility[i].possibleValueText.setString(ss.str());
		}
	}
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

void Field::setPosition(sf::Vector2i position)
{
	this->position = position;
}

sf::Vector2i Field::getPosition() const
{
	return position;
}
void Field::toggleLock()
{
	lock = !lock;

	if (lock)
		color = lockColor;
	else
		color = defaultColor;

	rect.setFillColor(color);
}

void Field::setDrawPosition(sf::Vector2f position)
{
	this->drawPosition = position;
	rect.setPosition(position);
	valueText.setPosition(position + valueTextOffset);

	for (size_t i = 0; i < aPossibility.size(); i++)
	{
		if (i > 0)
		{
			aPossibility[i].drawPosition = sf::Vector2f(
					drawPosition + sf::Vector2f(((i - 1) % 3) * size.x / 3 + size.x / 10, (i - 1) / 3 * size.y / 3 + size.y / 30));
			std::stringstream ss;
			ss << std::fixed << i;
			aPossibility[i].possibleValueText.setPosition(aPossibility[i].drawPosition);
		}
	}
}

bool Field::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	if (!(mousePos.x > drawPosition.x && mousePos.x < drawPosition.x + size.x && mousePos.y > drawPosition.y && mousePos.y < drawPosition.y + size.y))
		focus = false;
	else
		focus = true;

	if (focus)
	{
		color = focusColor;
		rect.setFillColor(color);
		return true;
	}
	else
	{
		if (!lock)
			color = defaultColor;
		else
			color = lockColor;

		rect.setFillColor(color);
		return false;
	}
}

void Field::draw(sf::RenderTarget &target) const
{
	target.draw(rect);
	target.draw(valueText);

	for (auto p : aPossibility)
	{
		if (p.isPossible)
			target.draw(p.possibleValueText);
	}
}

std::string Field::getText() const
{
	return valueText.getString();
}

