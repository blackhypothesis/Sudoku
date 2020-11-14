#pragma once

#include <string>
#include <array>
#include <vector>

#include <sstream>
#include <SFML/Graphics.hpp>

#include "State.h"

class Cell
{
public:
    Cell(sf::Font&);

private:
    // cell representation
    // ----------------------------------------------------------------------------
    int index;
    sf::Vector2i position;    // this is the x, y position, calculated from the index
    int value;
    CellState state;

    // Cluster Member
    /* 0-> horizontal, 1-> vertical, 2-> region
     * region cluster are 3 x 3 clusters in the sudoku
     *   +-----+
     *   |0|1|2|
     *   +-+-+-+
     *   |3|4|5|
     *   +-+-+-+
     *   |6|7|8|
     *   +-+-+-+
     */
    std::array<unsigned int, 3> clusterMember;

    std::vector<int> vecPossibleValues;
    std::vector<int> vecPossibleValuesApproved;

    // user interaction: mouse, keyboard
    // ----------------------------------------------------------------------------

    bool focus;

    // graphic
    // ----------------------------------------------------------------------------
    const sf::Font fontConsolas;
    std::array<sf::Color, 12> aStateColor;
    sf::Color cellColor;
    sf::Color focusColor;
    sf::Color valueColor;
    sf::Color possibleValueColor;
    sf::Color possibleValueApprovedColor;

    sf::Vector2f cellSize;
    sf::Vector2f cellDrawPosition;
    sf::RectangleShape cellRect;

    sf::Text valueText;
    sf::Vector2f valueTextOffset;

    struct DrawPossibleValue
    {
        sf::Vector2f drawPosition;
        sf::Text valueText;
        sf::Color valueColor;
    };

    std::array<DrawPossibleValue, 9> aDrawPossibleValue;

public:
    // cell representation
    // ----------------------------------------------------------------------------
    void setIndex(int);
    int getIndex() const;
    void calculateXY();
    void setValue(int);
    int getValue() const;
    bool setState(CellState);
    CellState getState() const;
    std::array<unsigned int, 3> getClusterNumbers();

    void setPossibleValuesApproved(std::vector<int>);
    std::vector<int> getPossibleValuesApproved() const;

    void setPossibleValues(std::vector<int>);
    std::vector<int> getPossibleValues() const;
    void removePossibleValues(std::vector<int>);
    bool containsPossibleValues(std::vector<int>);

    // user interaction: mouse, keyboard
    // ----------------------------------------------------------------------------
    bool mouseAction(sf::Vector2i, bool, bool);
    bool getFocus() const;

    // graphic
    // ----------------------------------------------------------------------------
    void setDPossibleValuesrawPosition(sf::Vector2f);
    void setCellColor();
    void draw(sf::RenderTarget&) const;
};
