#include "Board.h"

Board::Board() :
		separatorColor(sf::Color(100, 150, 150)), fieldSize(sf::Vector2f(80.0f, 80.0f)), offset(sf::Vector2f(20.0f, 20.0f)), distance(sf::Vector2f(7.0f, 7.0f)), setValueAllowed(
				true)
{
	for (size_t x = 0; x < board[x].size(); x++)
	{

		for (size_t y = 0; y < board[y].size(); y++)
		{
			board[x][y] = std::make_unique<Cell>(fontConsolas.fontConsolas);
			board[x][y]->setPosition(sf::Vector2i(x, y));
			board[x][y]->setDrawPosition(sf::Vector2f(fieldSize.x * x + x * distance.x, fieldSize.y * y + y * distance.y) + offset + distance);
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

bool Board::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased, int cellValue)
{
	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)
		{

			if (board[x][y]->mouseAction(mousePos, buttonPressed, buttonReleased) && cellValue != -1)
			{
				if (cellValue < 10)
				{
					board[x][y]->setValue(cellValue);
					initCellsToDefault();
					calculateIfSinglePossibleCellValue();
					calculateIfSinglePossibleClusterValues();
					calculatePairValue();
				}
				else
					board[x][y]->toggleLock();
			}

			if (board[x][y]->mouseAction(mousePos, buttonPressed, buttonReleased) && buttonPressed && setValueAllowed)
			{
				if (board[x][y]->getFoundValue() > 0)
				{
					board[x][y]->setValue(board[x][y]->getFoundValue());
					initCellsToDefault();
					calculateIfSinglePossibleCellValue();
					calculateIfSinglePossibleClusterValues();
					calculatePairValue();
					setValueAllowed = false;
				}
			}

			if (buttonReleased)
			{
				setValueAllowed = true;
			}
		}
	}

	return true;
}

void Board::initCellsToDefault()
{
	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)
		{
			board[x][y]->initCellToDefault();
		}
	}
}

void Board::calculateIfSinglePossibleCellValue()
{
	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)
		{
			if (board[x][y]->getPossibleValues().size() == 1)
				board[x][y]->setFoundValue(board[x][y]->getPossibleValues()[0]);
		}

	}
}

void Board::calculateIfSinglePossibleClusterValues()
{
	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)
		{
			// first set all possible values of enabled
			board[x][y]->setPossibleValues(std::vector<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, true);
			// if Cell has a value, then all possible values are disabled;
			if (board[x][y]->getValue() > 0)
			{
				board[x][y]->setPossibleValues(std::vector<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, false);
			}
			else
			{
				for (size_t xx = 0; xx < board[xx].size(); xx++)
				{
					for (size_t yy = 0; yy < board[yy].size(); yy++)
					{
						// ckeck if cell is in the same clusters x, y and square

						if (x == xx || y == yy || getSquareClusterNumber(x, y) == getSquareClusterNumber(xx, yy))
						{
							// if the cell [x][y] has a value, remove this value to all cells [xx][yy] within the same clusters.
							if (board[xx][yy]->getValue() > 0)
							{
								board[x][y]->setPossibleValues(std::vector<int> { board[xx][yy]->getValue() }, false);
							}
						}
					}
				}
			}
		}
	}

	// check if within a cluster (horizontal, vertical, square), a possible value exists only one time.
	// if this is the case this value is certain.
	// array  [0]  -> horizontal, [1] -> vertical, [2] -> square
	// size 10 for 9 values, because 0 is not a value, therefore index 1 to 9 are used.
	// size 9 for the number of cluster within each type.
	std::array<std::array<std::array<int, 10>, 9>, 3> aClusterValueCount = { };

	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)
		{

			// get all possible values from the cell in a vector and count it
			auto xxx = board[x][y]->getPossibleValues();
			for (auto pv : board[x][y]->getPossibleValues())
			{
				// for each of the 9 horizontal clusters
				aClusterValueCount[0][y][pv]++;
				// for each of the 9 vertical clusters
				aClusterValueCount[1][x][pv]++;
				// for each of the 9 square clusters
				aClusterValueCount[2][getSquareClusterNumber(x, y)][pv]++;
			}
		}
	}

	for (size_t clusterType = 0; clusterType < aClusterValueCount.size(); clusterType++)
	{
		for (size_t clusterNumber = 0; clusterNumber < aClusterValueCount[clusterType].size(); clusterNumber++)
		{
			for (size_t valueIndex = 1; valueIndex < aClusterValueCount[clusterType][clusterNumber].size(); valueIndex++)
			{
				// if the possible value is only once in a cluster, then it is a certain value
				if (aClusterValueCount[clusterType][clusterNumber][valueIndex] == 1)
				{
					// check if the cell is in the right cluster type, cluster number
					for (size_t x = 0; x < board[x].size(); x++)
					{
						for (size_t y = 0; y < board[y].size(); y++)
						{
							bool isCellInCluster = false;

							// horizontal
							if (clusterType == 0 && y == clusterNumber)
								isCellInCluster = true;
							// vertical
							if (clusterType == 1 && x == clusterNumber)
								isCellInCluster = true;
							// square
							if (clusterType == 2 && getSquareClusterNumber(x, y) == static_cast<int>(clusterNumber))
								isCellInCluster = true;

							// if cell is in the right cluster number, check if cell has a unique possible number
							if (isCellInCluster)
							{
								std::vector<int> vecPossibleValues = board[x][y]->getPossibleValues();

								// if the cell has this value as a possible value, then this is a certain value
								if (std::find(vecPossibleValues.begin(), vecPossibleValues.end(), valueIndex) != vecPossibleValues.end())
								{
									//std::cout << "x " << x << "  y " << y << "   clusterType " << clusterType << "   clusterNumber " << clusterNumber
									//		<< "   valueIndex " << valueIndex << " " << aClusterValueCount[clusterType][clusterNumber][valueIndex] << std::endl;

									board[x][y]->setFoundValue(valueIndex);
									board[x][y]->setPossibleValues(std::vector<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, false);
									std::vector<int> vecValue;
									vecValue.push_back(valueIndex);
									board[x][y]->setPossibleValues(vecValue, true);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Board::calculatePairValue()
{
	// check if within a cluster (horizontal, vertical, square), a possible value exists only 2 times.
	// if this is the case, check if there is a pair with the same possible values.
	// array  [0]  -> horizontal, [1] -> vertical, [2] -> square
	// size 10 for 9 values, because 0 is not a value, therefore index 1 to 9 are used.
	// size 9 for the number of cluster within each type.
	std::array<std::array<std::array<int, 10>, 9>, 3> aClusterValueCount = { };

	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)
		{

			// get all possible values from the cell in a vector and count it
			for (auto pv : board[x][y]->getPossibleValues())
			{
				// for each of the 9 horizontal clusters
				aClusterValueCount[0][y][pv]++;
				// for each of the 9 vertical clusters
				aClusterValueCount[1][x][pv]++;
				// for each of the 9 square clusters
				aClusterValueCount[2][getSquareClusterNumber(x, y)][pv]++;
			}
		}
	}

	for (size_t clusterType = 0; clusterType < aClusterValueCount.size(); clusterType++)
	{
		for (size_t clusterNumber = 0; clusterNumber < aClusterValueCount[clusterType].size(); clusterNumber++)
		{
			// check if the cell is in the right cluster type, cluster number

			std::vector<Cell> vecTwoPossibleValueCells;

			for (size_t x = 0; x < board[x].size(); x++)
			{
				for (size_t y = 0; y < board[y].size(); y++)
				{
					bool isCellInCluster = false;

					// horizontal
					if (clusterType == 0 && y == clusterNumber)
						isCellInCluster = true;
					// vertical
					if (clusterType == 1 && x == clusterNumber)
						isCellInCluster = true;
					// square
					if (clusterType == 2 && getSquareClusterNumber(x, y) == static_cast<int>(clusterNumber))
						isCellInCluster = true;

					// if cell is in the right cluster number and has exactly 2 valid possible values, store the cell in a vector
					if (isCellInCluster)
					{
						if (board[x][y]->getPossibleValues().size() == 2)
							vecTwoPossibleValueCells.push_back(*(board[x][y]));
					}
				}
			}

			// check if there are exactly 2 cells with 2 valid possible values
			if (vecTwoPossibleValueCells.size() == 2)
			{
				// check if the 2 possible values in the 2 cells are exactly the same
				if (vecTwoPossibleValueCells[0].getPossibleValues() == vecTwoPossibleValueCells[1].getPossibleValues())
				{

					// set the 2 cells with the value pairs in the state: foundPair = true
					board[vecTwoPossibleValueCells[0].getPosition().x][vecTwoPossibleValueCells[0].getPosition().y]->setFoundPair();
					board[vecTwoPossibleValueCells[1].getPosition().x][vecTwoPossibleValueCells[1].getPosition().y]->setFoundPair();

					std::cout << "found Pair  ";

					for (auto v : vecTwoPossibleValueCells[0].getPossibleValues())
					{
						std::cout << v << " ";
					}
					std::cout << std::endl;
				}
			}
		}
	}
}

int Board::getSquareClusterNumber(int x, int y)
{
	/* square cluster are 3 x 3 clusters in the sudoku
	 * they are numbered shown below
	 *   +-----+
	 *   |0|1|2|
	 *   +-+-+-+
	 *   |3|4|5|
	 *   +-+-+-+
	 *   |6|7|8|
	 *   +-+-+-+
	 */

	int offsetA = std::floor(x / 3);
	int offsetB = std::floor(y / 3);

	return offsetA + offsetB * 3;
}

void Board::draw(sf::RenderTarget &target) const
{
	for (size_t x = 0; x < board[x].size(); x++)
	{
		for (size_t y = 0; y < board[y].size(); y++)

		{
			board[x][y]->draw(target);
		}
	}

	for (int x = 0; x < 8; x++)
	{
		target.draw(vecSeparator[x]);
	}
}
