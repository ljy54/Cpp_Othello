#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class Reversi;

class PlayingState : public State {
private:
    sf::CircleShape surrenderButton;
    sf::Text surrenderText;
    sf::Text currentText;
    sf::Text countTextBlack;
    sf::Text countTextWhite;
    bool surrenderHovered = false;
    Reversi& game;
    sf::CircleShape undoButton; // 添加悔棋按钮
    sf::Text undoText;          // 添加悔棋文本
    bool undoHovered = false;   // 添加悔棋按钮悬停状态
public:
    PlayingState(Reversi& game);
    void handleEvent(const sf::Event& event, Reversi& game) override;
    void update(Reversi& game) override;
    void draw(sf::RenderWindow& window, Reversi& game) override;
};