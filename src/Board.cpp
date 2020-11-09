#include "Board.h"

Board::Board()
{
    // graphic
    // ----------------------------------------------------------------------------
    offset = sf::Vector2f(20, 20);
    regionSeparatorColor = sf::Color(100, 150, 150);
    cellSize = sf::Vector2f(80.0f, 80.0f);
    cellDistance = sf::Vector2f(7.0f, 7.0f);

    for (size_t i = 0; i < board.size(); i++)
    {
        sf::Vector2i pos = calculateXY(i);
        board[i] = std::make_unique<Cell>(fontConsolas.fontConsolas);
        board[i]->setIndex(i);
        board[i]->setDrawPosition(
                sf::Vector2f(cellSize.x * pos.x + pos.x * cellDistance.x, cellSize.y * pos.y + pos.y * cellDistance.y) + offset + cellDistance);
        if (i % 2)
            board[i]->setValue(i % 9);
    }

    for (int x = 0; x < 4; x++)
    {
        sf::RectangleShape r;
        r.setPosition(offset + sf::Vector2f(3 * x * (cellSize.x + cellDistance.x), 0));
        r.setSize(sf::Vector2f(cellDistance.x, 9 * cellSize.y + (9 + 1) * cellDistance.y));
        r.setFillColor(regionSeparatorColor);
        vecRegionSeparator.push_back(r);
    }

    for (int x = 0; x < 4; x++)
    {
        sf::RectangleShape r;
        r.setPosition(offset + sf::Vector2f(0, 3 * x * (cellSize.x + cellDistance.x)));
        r.setSize(sf::Vector2f(9 * cellSize.x + (9 + 1) * cellDistance.x, cellDistance.y));
        r.setFillColor(regionSeparatorColor);
        vecRegionSeparator.push_back(r);
    }
}

//board representation
// ----------------------------------------------------------------------------
sf::Vector2i Board::calculateXY(int index)
{
    return sf::Vector2i(index % 9, index / 9);
}

void Board::checkIntegrity()
{
    // check if within a cluster (horizontal, vertical, region), a value exists more than once.
    // if this is the case, there is an ERROR.
    // array  [0]  -> horizontal, [1] -> vertical, [2] -> square
    // size 9 for the number of cluster within each type.
    // size 9 for 9 values, index 0 -> value=1, ...
    std::array<std::array<std::array<int, 9>, 9>, 3> aClusterValueCount =
    { };

    for (auto &cell : board)
    {
        std::array<int, 3> cellClusterNumber = cell->getClusterNumbers();
        CellState state = cell->getState();
        if (state == SOLVED || state == ERROR)
        {
            // count (by increment) the number of each value for each cluster
            for (size_t clusterType = 0; clusterType < aClusterValueCount.size(); clusterType++)
                aClusterValueCount[clusterType][cellClusterNumber[clusterType]][cell->getValue() - 1]++;
        }
    }

    for (auto &cell : board)
    {
        std::array<int, 3> cellClusterNumber = cell->getClusterNumbers();
        CellState state = cell->getState();
        if (state == SOLVED || state == ERROR)
        {
            bool error = false;

            for (size_t clusterType = 0; clusterType < aClusterValueCount.size(); clusterType++)
            {
                if (aClusterValueCount[clusterType][cellClusterNumber[clusterType]][cell->getValue() - 1] > 1)
                {
                    cell->setState(ERROR);
                    error = true;
                }
            }

            if (error == false)
            {
                cell->setState(SOLVED);
            }
        }
    }
}

// user interaction (mouse, keyboard)
// ----------------------------------------------------------------------------
bool Board::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
    for (size_t i = 0; i < board.size(); i++)
    {

        if (board[i]->mouseAction(mousePos, buttonPressed, buttonReleased))
        {
            std::cout << "cell " << i << " mouse action" << std::endl;
        }
    }

    return true;
}

void Board::setCellValue(int value)
{
    for (size_t i = 0; i < board.size(); i++)
        board[i]->setValue(value);

    checkIntegrity();
}

// graphic
// ----------------------------------------------------------------------------
void Board::draw(sf::RenderTarget &target) const
{
    for (size_t i = 0; i < board.size(); i++)
    {
        board[i]->draw(target);
    }

    for (auto &vs : vecRegionSeparator)
        target.draw(vs);
}
