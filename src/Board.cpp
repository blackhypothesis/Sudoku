#include "Board.h"

Board::Board()
{
    // graphic
    // ----------------------------------------------------------------------------
    offset = sf::Vector2f(20, 20);
    regionSeparatorColor = sf::Color(207, 224, 248);
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
        if (cell->getPossibleValues().size() == 0 && cell->getState() == EMPTY)
        {
            cell->setState(E_NOPOSSIBLEVALUE);
        }
    }
}

/*
 *     search
 */

bool Board::searchForNakedSingles()
{
    bool foundSingle = false;

    // search for NAKED_SINGLE
    for (auto &cell : board)
    {
        if (cell->getPossibleValues().size() == 1)
        {
            std::array<unsigned int, 3> cellClusterType = cell->getClusterNumbers();
            cell->setState(NAKED_SINGLE);
            cell->setPossibleValuesApproved(std::vector<int>
            { cell->getPossibleValues()[0] });
            foundSingle = true;

            // mark the values in the cells which are in the same cluster as the NAKED_SINGLE as discarded
            for (unsigned int clusterType = 0; clusterType < cellClusterType.size(); clusterType++)
            {
                for (auto &cell2 : board)
                {
                    if (cell2->getState() == EMPTY && cellClusterType[clusterType] == cell2->getClusterNumbers()[clusterType])
                    {
                        cell2->addPossibleValuesDiscarded(std::vector<int>
                        { cell->getPossibleValuesApproved()[0] });
                    }
                }
            }
        }
    }
    return foundSingle;
}

bool Board::searchForHiddenSingles()
{
    bool foundSingle = false;

    // search for HIDDEN_SINGLE
    std::array<std::array<std::array<unsigned int, 9>, 9>, 3> aClusterPossibleValueCount = countOccurenceOfPossibleValueInEachCluster();

    // go through each cluster and check, if a possible value occures only 1 time
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
                        std::array<unsigned int, 3> cellClusterType = cell->getClusterNumbers();
                        CellState state = cell->getState();
                        if (state == EMPTY)
                        {
                            int pv = possibleValue + 1;
                            // if the cell is in the same clustertype and clusternumber AND if the cell contains the possible value, which occures
                            // only one time in the cluster
                            if (clusterNumber == cellClusterType[clusterType] && cell->containsPossibleValues(std::vector<int>
                            { pv }))
                            {
                                cell->setState(HIDDEN_SINGLE);
                                cell->setPossibleValuesApproved(std::vector<int>
                                { pv });
                                foundSingle = true;

                                // mark the values in the cells which are in the same cluster as the HIDDEN_SINGLE as discarded
                                for (unsigned int clusterType = 0; clusterType < cellClusterType.size(); clusterType++)
                                {
                                    for (auto &cell2 : board)
                                    {
                                        if (cell2->getState() == EMPTY && cellClusterType[clusterType] == cell2->getClusterNumbers()[clusterType])
                                        {
                                            cell2->addPossibleValuesDiscarded(std::vector<int>
                                            { cell->getPossibleValuesApproved()[0] });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return foundSingle;
}

bool Board::searchForNakedPairs()
{
    bool foundPair = false;

    std::vector<std::shared_ptr<Cell>> vecSuitableCells;

    // search for NAKED_PAIR

    // search all cells, which have the exact number of possible values
    for (auto cell : board)
    {
        if (cell->getPossibleValues().size() == 2)
        {
            vecSuitableCells.push_back(cell);
        }
    }

    // check for this cells, if they are in the same cluster and have the same possible values
    for (auto &cellFirst : vecSuitableCells)
    {
        for (auto &cellSecond : vecSuitableCells)
        {
            if (cellFirst->getIndex() != cellSecond->getIndex())
            {
                std::array<unsigned int, 3> cellFirstClusterType = cellFirst->getClusterNumbers();
                std::array<unsigned int, 3> cellSecondClusterType = cellSecond->getClusterNumbers();

                for (size_t clusterTypeNumber = 0; clusterTypeNumber < cellFirstClusterType.size(); clusterTypeNumber++)
                {
                    // are the 2 cell in the same cluster type and the same clusternumber AND have the same possible values?
                    // then a NAKED_PAIR is found
                    if (cellFirstClusterType[clusterTypeNumber] == cellSecondClusterType[clusterTypeNumber]
                            && cellFirst->getPossibleValues() == cellSecond->getPossibleValues())
                    {
                        cellFirst->setState(NAKED_PAIR);
                        cellFirst->setPossibleValuesApproved(cellFirst->getPossibleValues());
                        cellSecond->setState(NAKED_PAIR);
                        cellSecond->setPossibleValuesApproved(cellSecond->getPossibleValues());

                        // mark the values as discarded which are in the NAKED_PAIR from the other cells within the same cluster
                        for (auto &cell : board)
                        {
                            if (cell->getClusterNumbers()[clusterTypeNumber] == cellFirstClusterType[clusterTypeNumber] && cell->getState() == EMPTY)
                            {
                                cell->setPossibleValuesDiscarded(cellFirst->getPossibleValues());
                            }
                        }

                        foundPair = true;
                    }
                }
            }
        }
    }

    return foundPair;
}

bool Board::searchForHiddenPairs()
{
    bool foundPair = false;

    std::array<std::array<std::array<unsigned int, 9>, 9>, 3> aClusterPossibleValueCount = countOccurenceOfPossibleValueInEachCluster();

    // go through each cluster and check, if a possible value occures exactly 2 times
    for (unsigned int clusterType = 0; clusterType < aClusterPossibleValueCount.size(); clusterType++)
    {
        for (unsigned int clusterNumber = 0; clusterNumber < aClusterPossibleValueCount[0].size(); clusterNumber++)
        {
            std::vector<std::vector<std::shared_ptr<Cell>>> vecHiddenPairCandidates;

            // if exactly 2 cells with the same 2 possible values exists in a cluster, then it is a HIDDEN_PAIR
            for (int v1 = 1; v1 < 10; v1++)
            {
                for (int v2 = v1; v2 < 10; v2++)
                {
                    if (v1 != v2)
                    {
                        std::vector<std::shared_ptr<Cell>> vecCellHiddenPairCandidates;

                        for (auto &cell : board)
                        {
                            std::array<unsigned int, 3> cellClusterNumber = cell->getClusterNumbers();
                            CellState state = cell->getState();

                            if (state == EMPTY && clusterNumber == cellClusterNumber[clusterType] && cell->containsPossibleValues(std::vector<int>
                            { v1, v2 }))
                            {
                                vecCellHiddenPairCandidates.push_back(cell);
                                cell->setPossibleValuesHidden(std::vector<int>
                                { v1, v2 });
                            }
                        }
                        // if exactly 2 cells have the same 2 values -> found a HIDDEN_PAIR candidate
                        if (vecCellHiddenPairCandidates.size() == 2)
                            vecHiddenPairCandidates.push_back(vecCellHiddenPairCandidates);
                    }
                }
            }
            // if there is only one HIDDEN_PAIR candiate, then it is an approved HIDDEN_PAIR
            if (vecHiddenPairCandidates.size() == 1)
            {
                vecHiddenPairCandidates[0][0]->setState(HIDDEN_PAIR);
                vecHiddenPairCandidates[0][1]->setState(HIDDEN_PAIR);
            }
        }
    }

    return foundPair;
}

void Board::removePossibleValuesDiscarded()
{
    for (auto &cell : board)
    {
        cell->removePossibeValuesDiscarded();
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

        currentAction = 0;
    }

//perform all actions
    if (action == "A")
    {
        cleanupPossibleValues();
        checkCellValueIntegrity();
        searchForNakedSingles();
        searchForNakedPairs();
        removePossibleValuesDiscarded();
        // searchForHiddenPairs();
    }

// perform actions step by step
    if (action == "N")
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
            searchForNakedSingles();
            break;
        case 3:
            removePossibleValuesDiscarded();
            break;
        case 4:
            searchForHiddenSingles();
            break;
        case 5:
            removePossibleValuesDiscarded();
            break;
        case 6:
            searchForNakedPairs();
            break;
        case 7:
            removePossibleValuesDiscarded();
            break;
        case 8:
            searchForHiddenPairs();
            break;
        case 9:
            removePossibleValuesDiscarded();
            break;
        }

        std::cout << currentAction << std::endl;

        currentAction++;
        sleep = true;
    }

    if (action == "M")
    {
        for (auto &cell : board)
        {
            if (cell->getState() == NAKED_SINGLE || cell->getState() == HIDDEN_SINGLE)
            {
                cell->setValue(cell->getPossibleValuesApproved()[0]);
            }
        }

        setCellsToDefault();
        currentAction = 0;
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

std::array<std::array<std::array<unsigned int, 9>, 9>, 3> Board::countOccurenceOfPossibleValueInEachCluster()
{
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

    return aClusterPossibleValueCount;
}
