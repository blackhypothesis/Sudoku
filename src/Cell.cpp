#include "Cell.h"

Cell::Cell(sf::Font &fc) :
        fontConsolas(fc)
{
    //representation
    index = 0;
    calculateXY();
    value = 0;
    state = EMPTY;
    clusterMember =
    { 0, 0, 0 };
    vecPossibleValues = std::vector<int>
    { 1, 2, 3, 4, 5, 6, 7, 8, 9 };   // all 9 values are possible

    // user interaction
    focus = false;

    // graphic
    aStateColor =
    { sf::Color(30, 30, 30), // EMPTY
    sf::Color(80, 20, 20),   // E_MULTIPLEVALUES
    sf::Color(80, 30, 30),   // E_NOPOSSIBLEVALUE
    sf::Color(20, 40, 20),   // SOLVED
    sf::Color(20, 60, 20),  // NAKED_SINGLE
    sf::Color(20, 80, 20),   // HIDDEN_SINGLE
    sf::Color(20, 20, 120),  // NAKED_PAIR
    sf::Color(20, 20, 80),   // HIDDEEN_PAIR
    sf::Color(20, 120, 120), // NAKED_TRIPPLE
    sf::Color(20, 80, 80),   // HIDDEN_TRIPPLE
    sf::Color(20, 80, 20),   // NAKED_QUAD
    sf::Color(20, 80, 20)    // HIDDEN_QUAD
            };

    cellColor = aStateColor[state];
    focusColor = sf::Color(100, 100, 100);
    valueColor = sf::Color(sf::Color::Yellow);
    possibleValueColor = sf::Color(200, 200, 200);
    possibleValueApprovedColor = sf::Color(20, 200, 20);

    cellSize = sf::Vector2f(80, 80);
    cellDrawPosition = sf::Vector2f(0, 0);

    cellRect.setSize(sf::Vector2f(cellSize));
    cellRect.setFillColor(cellColor);

    valueTextOffset = sf::Vector2f(25, 7);
    valueText.setFont(fontConsolas);
    valueText.setCharacterSize(50);
    valueText.setFillColor(valueColor);
    valueText.setString("0");

    setDPossibleValuesrawPosition(sf::Vector2f(0, 0));
}

// representation
// ----------------------------------------------------------------------------
void Cell::setIndex(int index)
{
    this->index = index;
    calculateXY();
    clusterMember[0] = index / 9;   // horizontal cluster number
    clusterMember[1] = index % 9;   // vertical cluster number
    int offsetA = position.x / 3;
    int offsetB = position.y / 3;
    clusterMember[2] = offsetA + offsetB * 3;
}

int Cell::getIndex() const
{
    return index;
}

void Cell::calculateXY()
{
    position.x = index % 9;
    position.y = index / 9;
}

void Cell::setValue(int value)
{
    //if (focus)
    {
        std::stringstream ss;
        this->value = value;

        if (value > 0)
        {
            state = SOLVED;
            vecPossibleValues = std::vector<int>
            { };

            ss << std::fixed << value;
            valueText.setString(ss.str());
        }
        else
        {
            state = EMPTY;
            vecPossibleValues = std::vector<int>
            { 1, 2, 3, 4, 5, 6, 7, 8, 9 };   // all 9 values are possible
            valueText.setString(" ");
        }
        cellRect.setFillColor(aStateColor[state]);
    }
}

int Cell::getValue() const
{
    return value;
}

bool Cell::setState(CellState state)
{
    this->state = state;
    cellRect.setFillColor(aStateColor[state]);

    if (state == SOLVED)
    {
        vecPossibleValues = std::vector<int>
        { };
        return true;
    }
    else if (state == EMPTY)
    {
        // all 9 values are possible
        setPossibleValues(std::vector<int>
        { 1, 2, 3, 4, 5, 6, 7, 8, 9 });
        // but no approved possible values
        setPossibleValuesApproved(std::vector<int>
        { });

        return true;
    }
    else
        return false;
}

CellState Cell::getState() const
{
    return state;
}

std::array<int, 3> Cell::getClusterNumbers()
{
    return clusterMember;
}

std::vector<int> Cell::getPossibleValues() const
{
    return vecPossibleValues;
}

void Cell::setPossibleValues(std::vector<int> vecPossibleValues)
{
    if (state != SOLVED || state != E_MULTIPLEVALUES)
        this->vecPossibleValues = vecPossibleValues;

    for (auto pv : vecPossibleValues)
    {
        aDrawPossibleValue[pv - 1].valueColor = possibleValueColor;
        aDrawPossibleValue[pv - 1].valueText.setFillColor(possibleValueColor);
    }
}

void Cell::removePossibleValues(std::vector<int> vecRemoveValues)
{
    for (auto i = vecPossibleValues.begin(); i < vecPossibleValues.end(); i++)
    {
        for (auto r : vecRemoveValues)
        {
            if (*i == r)
            {
                vecPossibleValues.erase(i);
            }
        }
    }
}

std::vector<int> Cell::getPossibleValuesApproved() const
{
    return vecPossibleValuesApproved;
}

void Cell::setPossibleValuesApproved(std::vector<int> vecPossibleValuesApproved)
{
    if (state != SOLVED || state != E_MULTIPLEVALUES)
        this->vecPossibleValuesApproved = vecPossibleValuesApproved;

    for (auto pv : vecPossibleValuesApproved)
    {
        aDrawPossibleValue[pv - 1].valueColor = possibleValueColor;
        aDrawPossibleValue[pv - 1].valueText.setFillColor(possibleValueApprovedColor);
    }
}

// user interaction
// ----------------------------------------------------------------------------

bool Cell::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
    if (!(mousePos.x > cellDrawPosition.x && mousePos.x < cellDrawPosition.x + cellSize.x && mousePos.y > cellDrawPosition.y
            && mousePos.y < cellDrawPosition.y + cellSize.y))
    {
        focus = false;
        cellRect.setFillColor(aStateColor[state]);
        return false;
    }
    else
    {
        focus = true;
        cellRect.setFillColor(focusColor);
        return true;
    }
}

bool Cell::getFocus() const
{
    return focus;
}

// graphic
// ----------------------------------------------------------------------------
void Cell::setDPossibleValuesrawPosition(sf::Vector2f position)
{
    this->cellDrawPosition = position;
    cellRect.setPosition(position);
    valueText.setPosition(position + valueTextOffset);

    //this are the 9 possible values, shown as little text in the cell
    for (size_t i = 0; i < aDrawPossibleValue.size(); i++)
    {
        aDrawPossibleValue[i].drawPosition = sf::Vector2f(
                cellDrawPosition + sf::Vector2f(i % 3 * cellSize.x / 3 + cellSize.x / 10, i / 3 * cellSize.y / 3 + cellSize.y / 30));
        aDrawPossibleValue[i].valueText.setFont(fontConsolas);
        aDrawPossibleValue[i].valueText.setCharacterSize(18);
        aDrawPossibleValue[i].valueColor = possibleValueColor;
        aDrawPossibleValue[i].valueText.setFillColor(possibleValueColor);
        aDrawPossibleValue[i].valueText.setPosition(aDrawPossibleValue[i].drawPosition);

        std::stringstream ss;
        ss << std::fixed << i + 1;
        aDrawPossibleValue[i].valueText.setString(ss.str());
    }
}

void Cell::setCellColor()
{
    if (focus)
        cellRect.setFillColor(focusColor);
    else
        cellRect.setFillColor(aStateColor[state]);
}

void Cell::draw(sf::RenderTarget &target) const
{
    target.draw(cellRect);

    // draw value
    if (state == SOLVED || state == E_MULTIPLEVALUES)
    {
        target.draw(valueText);
    }
    // draw possible values
    else
    {
        for (auto pv : vecPossibleValues)
        {
            target.draw(aDrawPossibleValue[pv - 1].valueText);
        }
    }
}

