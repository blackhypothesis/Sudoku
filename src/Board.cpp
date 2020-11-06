#include "Board.h"


Board::Board() :
		separatorColor(sf::Color(100, 150, 150)),
		fieldSize(sf::Vector2f(80.0f, 80.0f)), offset(sf::Vector2f(20.0f, 20.0f)),
		distance(sf::Vector2f(7.0f, 7.0f)),
		enableSetValue(true)
{
	for (size_t x = 0; x < board[x].size(); x++)
	{

		for (size_t y = 0; y < board[y].size(); y++)
		{
			board[x][y] = std::make_unique<Field>(fontConsolas.fontConsolas);

			board[x][y]->setPosition(sf::Vector2f(fieldSize.x * x + x * distance.x, fieldSize.y * y + y * distance.y) + offset + distance);
			//board[x][y]->setValue(0);
		}
	}

	for (int x = 0; x < 4; x++)
	{
		sf::RectangleShape r;
		r.setPosition(offset + sf::Vector2f(3 * x * (fieldSize.x + distance.x), 0));
		r.setSize(sf::Vector2f(distance.x, 9 * fieldSize.y + (9 + 1) * distance.y));
		r.setFillColor(separatorColor);
		vecSeparator.push_back(r);
	}

	for (int x = 0; x < 4; x++)
	{
		sf::RectangleShape r;
		r.setPosition(offset + sf::Vector2f(0, 3 * x * (fieldSize.x + distance.x)));
		r.setSize(sf::Vector2f(9 * fieldSize.x + (9 + 1) * distance.x, distance.y));
		r.setFillColor(separatorColor);
		vecSeparator.push_back(r);
	}
}


bool Board::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased, int fieldValue)
{
	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)
		{
			if(board[x][y]->mouseAction(mousePos, buttonPressed, buttonReleased) && fieldValue != -1)
			{
				if (fieldValue < 10)
					board[x][y]->setValue(fieldValue);
				else
					board[x][y]->toggleLock();
			}

			if(board[x][y]->mouseAction(mousePos, buttonPressed, buttonReleased) && buttonPressed && enableSetValue)
			{
				int v = board[x][y]->getValue();
				v++;
				if (v > 9) v = 0;
				board[x][y]->setValue(v);
				enableSetValue = false;
			}

			if (buttonReleased)
			{
				enableSetValue = true;
			}
		}
	}

	return true;
}

void Board::draw(sf::RenderTarget &target) const
{
	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)

		{
			// target.draw(board[x][y].rect);
			//target.draw(board[x][y].valueText);
			// board[x][y].draw(target);;
			// tmp.draw(target);

			board[x][y]->draw(target);

			// std::cout << "x = " << x << "  y = " << y << "    " << board[x][y]->getText() << std::endl;

		}
	}

	//board[1][1].draw(target);

	for (int x = 0; x < 8; x++)
	{
		target.draw(vecSeparator[x]);
	}
}
