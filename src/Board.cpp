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
        board[i]->setDPossibleValuesrawPosition(
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

    // user interaction (mouse, keyboard)
    // ----------------------------------------------------------------------------
    mouseActionEnabled = true;
    currentAction = 0;
}

//board representation
// ----------------------------------------------------------------------------
sf::Vector2i Board::calculateXY(int index)
{
    return sf::Vector2i(index % 9, index / 9);
}

void Board::loadGame(std::array<int, 81> aGame)
{
    for (size_t i = 0; i < aGame.size(); i++)
        board[i]->setValue(aGame[i]);

    setCellsToDefault();
}

void Board::setCellsToDefault()
{
    // first set all possible values
    for (auto &cell : board)
    {
        if (cell->getState() != SOLVED && cell->getState() != E_MULTIPLEVALUES)
            cell->setState(EMPTY);
        // reset ERRORS
        if (cell->getState() == E_MULTIPLEVALUES)
            cell->setState(SOLVED);
    }
}

void Board::cleanupPossibleValues()
{
    for (auto &cellFirst : board)
    {
        // if cell is SOLVED, then remove all possible values
        if (cellFirst->getState() == SOLVED)
        {
            cellFirst->removePossibleValues(std::vector<int>
            { 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            std::array<unsigned int, 3> cellFirstClusterNumber = cellFirst->getClusterNumbers();

            for (auto &cellSecond : board)
            {
                if (cellSecond->getState() == EMPTY)
                {
                    std::array<unsigned int, 3> cellSecondClusterNumber = cellSecond->getClusterNumbers();

                    bool clusterMatch = false;

                    // check if 1st cell is in the same cluster number as the 2nd cell, for each clustertype
                    for (size_t i = 0; i < cellFirstClusterNumber.size(); i++)
                        if (cellFirstClusterNumber[i] == cellSecondClusterNumber[i])
                            clusterMatch = true;

                    if (clusterMatch)
                        cellSecond->removePossibleValues(std::vector<int>
                        { cellFirst->getValue() });
                }
            }
        }
    }
}

void Board::checkCellValueIntegrity()
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
        std::array<unsigned int, 3> cellClusterNumber = cell->getClusterNumbers();
        CellState state = cell->getState();
        if (state == SOLVED || state == E_MULTIPLEVALUES)
        {
            // count (by increment) the number of each value for each cluster
            for (size_t clusterType = 0; clusterType < aClusterValueCount.size(); clusterType++)
                aClusterValueCount[clusterType][cellClusterNumber[clusterType]][cell->getValue() - 1]++;
        }
    }

    for (auto &cell : board)
    {
        std::array<unsigned int, 3> cellClusterNumber = cell->getClusterNumbers();
        CellState state = cell->getState();
        if (state == SOLVED || state == E_MULTIPLEVALUES)
        {
            for (size_t clusterType = 0; clusterType < aClusterValueCount.size(); clusterType++)
            {
                // check, if the number the occurence of a value within a cluster is greater than 1. In this case,
                // there is an ERROR.
                if (aClusterValueCount[clusterType][cellClusterNumber[clusterType]][cell->getValue() - 1] > 1)
                {
                    cell->setState(E_MULTIPLEVALUES);
                }
            }
        }
    }

    // check if there is a cell with no possible value
    for (auto &cell : board)
    {
        if (cell->getPossibleValues().size() == 0 && cell->getState() != SOLVED && cell->getState() != E_MULTIPLEVALUES)
        {
            cell->setState(E_NOPOSSIBLEVALUE);
        }
    }
}

void Board::searchSingles()
{
    // search for NAKED_SINGLE
    for (auto &cell : board)
    {
        if (cell->getPossibleValues().size() == 1)
        {
            cell->setState(NAKED_SINGLE);
            cell->setPossibleValuesApproved(std::vector<int>
            { cell->getPossibleValues()[0] });
        }
    }

    // search for HIDDEN_SINGLE
    std::array<std::array<std::array<unsigned int, 9>, 9>, 3> aClusterPossibleValueCount =
    { };

    // for each cell, check in which cluster it is and cumulate all possible values of the cell
    // within each cluster
    for (auto &cell : board)
    {
        std::array<unsigned int, 3> cellClusterNumber = cell->getClusterNumbers();
        CellState state = cell->getState();
        if (state != SOLVED && state != E_MULTIPLEVALUES)
        {
            // count (by increment) how many times a possible value occures in each cluster
            std::vector<int> vecPossibleValues = cell->getPossibleValues();

            for (auto pv : vecPossibleValues)
            {
                for (size_t clusterType = 0; clusterType < aClusterPossibleValueCount.size(); clusterType++)
                {
                    aClusterPossibleValueCount[clusterType][cellClusterNumber[clusterType]][pv - 1]++;
                }
            }
        }
    }

    // go through each cluster and check, if a possible value occures onle 1 time
    for (unsigned int clusterType = 0; clusterType < aClusterPossibleValueCount.size(); clusterType++)
    {
        for (unsigned int clusterNumber = 0; clusterNumber < aClusterPossibleValueCount[0].size(); clusterNumber++)
        {
            for (unsigned int possibleValue = 0; possibleValue < aClusterPossibleValueCount[0][0].size(); possibleValue++)
            {
                // if the possible value occures only 1 time, then this is a HIDDEN_SINGLE
                if (aClusterPossibleValueCount[clusterType][clusterNumber][possibleValue] == 1)
                {
                    // check each cell ...
                    for (auto &cell : board)
                    {
                        std::array<unsigned int, 3> cellClusterNumber = cell->getClusterNumbers();
                        CellState state = cell->getState();
                        if (state != SOLVED && state != E_MULTIPLEVALUES)
                        {
                            int pv = possibleValue + 1;
                            // if the cell is in the same clustertype and clusternumber AND if the cell contains the possible value, which occures
                            // only one time in the cluster
                            if (clusterNumber == cellClusterNumber[clusterType] && cell->containsPossibleValues(std::vector<int>{ pv }))
                            {
                                cell->setState(HIDDEN_SINGLE);
                                cell->setPossibleValuesApproved(std::vector<int>
                                { pv });
                            }
                        }
                    }
                }
            }
        }
    }
}

// user interaction (mouse, keyboard)
// ----------------------------------------------------------------------------
bool Board::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
    for (auto &cell : board)
    {
        if (cell->mouseAction(mousePos, buttonPressed, buttonReleased) && buttonPressed)
        {
            if (cell->getState() == NAKED_SINGLE || cell->getState() == HIDDEN_SINGLE)
            {
                mouseActionEnabled = false;
                performAction(std::to_string(cell->getPossibleValuesApproved()[0]));
            }
        }
    }

    if (mouseActionEnabled == false && buttonReleased)
    {
        mouseActionEnabled = true;
        setCellsToDefault();
    }

    return true;
}

void Board::performAction(std::string action)
{
    int value;
    bool sleep = false;

    try
    {
        value = std::stoi(action);
    } catch (std::invalid_argument&)
    {
        value = -1;
    }

    // set value of cell
    if (value != -1)
    {
        for (auto &cell : board)
            if (cell->getFocus())
                cell->setValue(value);

        setCellsToDefault();
        currentAction = 0;
    }

    //perform all actions
    //if (action == "A")
    {
        cleanupPossibleValues();
        checkCellValueIntegrity();
        searchSingles();
    }

    // perform actions step by step
    if (action == "S")
    {
        switch (currentAction)
        {
        case 0:
            cleanupPossibleValues();
            break;
        case 1:
            checkCellValueIntegrity();
            break;
        case 2:
            searchSingles();
            break;
        }
        currentAction++;
        sleep = true;
    }

    if (action == "R")
    {
        setCellsToDefault();
        currentAction = 0;
        sleep = true;
    }

    // sleep, otherwise the actions will be executed too fast
    if (sleep)
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
