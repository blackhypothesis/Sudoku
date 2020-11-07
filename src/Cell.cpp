#include "Cell.h"

Cell::Cell(sf::Font &fc) :
		value(0), foundValue(0), foundPair(false), focus(false), lock(false), size(sf::Vector2f(80.0f, 80.0f)), focusColor(sf::Color(100, 100, 50)), lockColor(
				sf::Color(50, 100, 230)), foundValueColor(sf::Color(0, 50, 0)), foundPairValueCellColor(sf::Color(0, 0, 50)), fontConsolas(fc), valueTextOffset(
				sf::Vector2f(25, 7))
{
	rect.setSize(sf::Vector2f(size));
	defaultColor = sf::Color(50, 50, 50);
	color = defaultColor;
	rect.setFillColor(color);

	valueText.setFont(fontConsolas);
	valueText.setCharacterSize(50);
	valueText.setFillColor(sf::Color::Yellow);

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
			aPossibility[i].possibleValueText.setFillColor(sf::Color(255, 255, 255));

			aPossibility[i].possibleValue = i;
			aPossibility[i].isPossible = true;
			std::stringstream ss;
			ss << std::fixed << aPossibility[i].possibleValue;
			aPossibility[i].possibleValueText.setString(ss.str());
		}
		else
		{
			aPossibility[i].possibleValue = 0;
			aPossibility[i].isPossible = false;
		}
	}
}

void Cell::setValue(int value)
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

	foundValue = 0;
}

int Cell::getValue() const
{
	return value;
}

void Cell::setPosition(sf::Vector2i position)
{
	this->position = position;
}

sf::Vector2i Cell::getPosition() const
{
	return position;
}

void Cell::setPossibleValues(std::vector<int> vecPValues, bool state)
{
	for (auto pValue : vecPValues)
		aPossibility[pValue].isPossible = state;
}

std::vector<int> Cell::getPossibleValues() const
{
	std::vector<int> vecPValues;

	for (auto p : aPossibility)
		if (p.isPossible)
			vecPValues.push_back(p.possibleValue);

	return vecPValues;
}

void Cell::setFoundValue(int v)
{
	foundValue = v;
	setColor(foundValueColor);
}

int Cell::getFoundValue() const
{
	return foundValue;
}

void Cell::setFoundPair()
{
	foundPair = true;
	setColor(foundPairValueCellColor);
}

void Cell::initCellToDefault()
{
	defaultColor = sf::Color(50, 50, 50);
	color = defaultColor;
	rect.setFillColor(color);
	foundPair = false;
	foundValue = 0;
	for (auto p : aPossibility)
	{
		p.isPossible = true;
	}
	aPossibility[0].isPossible = false;
	aPossibility[0].possibleValue = 0;
}

void Cell::setColor(sf::Color c)
{
	defaultColor = c;
	color = defaultColor;
	rect.setFillColor(color);
}

void Cell::toggleLock()
{
	lock = !lock;

	if (lock)
		color = lockColor;
	else
		color = defaultColor;

	rect.setFillColor(color);
}

void Cell::setDrawPosition(sf::Vector2f position)
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

bool Cell::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	if (!(mousePos.x > drawPosition.x && mousePos.x < drawPosition.x + size.x && mousePos.y > drawPosition.y && mousePos.y < drawPosition.y + size.y))
		focus = false;
	else
		focus = true;

	if (focus)
	{
		color = focusColor;
		rect.setFillColor(color);
		if (aPossibility[0].possibleValue > 0)
		{
			value = aPossibility[0].possibleValue;
			aPossibility[0].possibleValue = 0;
		}
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

void Cell::draw(sf::RenderTarget &target) const
{
	target.draw(rect);
	target.draw(valueText);

	for (auto p : aPossibility)
	{
		if (p.isPossible)
			target.draw(p.possibleValueText);
	}
}

std::string Cell::getText() const
{
	return valueText.getString();
}

