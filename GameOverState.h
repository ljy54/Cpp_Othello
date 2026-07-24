#pragma once
#include "State.h"
#include "Reversi.h"
#include <SFML/Graphics.hpp>

class GameOverState : public State {
private:
    Reversi& game;
    sf::Font font;
    sf::RectangleShape menuButton;
    sf::Text menuButtonText;
    sf::Text gameOverText;
    sf::Text scoreText;
    sf::Text winnerText;
    bool buttonHovered;

public:
    //explicit GameOverState(Reversi& game);
    GameOverState(Reversi& game);
    void handleEvent(const sf::Event& event, Reversi& game) override;
    void update(Reversi& game) override;
    void draw(sf::RenderWindow& window, Reversi& game) override;
};