#pragma once
#include <SFML/Graphics.hpp>
//#include "Reversi.h"

class Reversi;
class State {
public:
    virtual ~State() = default;
    virtual void handleEvent(const sf::Event& event, Reversi& game) = 0;
    virtual void update(Reversi& game) = 0;
    virtual void draw(sf::RenderWindow& window, Reversi& game) = 0;
};