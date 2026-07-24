#pragma once

#include "State.h"
#include "Reversi.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Reversi;

class OpenState : public State {
private:
    Reversi& game;
    sf::Texture openingBgTexture;
    sf::Sprite openingBgSprite;
    sf::Font font;
    std::vector<std::wstring> openingTexts;
    sf::Clock animationClock;
    float scrollPosition;
    float normalScrollSpeed;
    float scrollSpeed;
    float fastScrollSpeed;

public:
    OpenState(Reversi& game);
    void handleEvent(const sf::Event& event, Reversi& game) override;
    void update(Reversi& game) override;
    void draw(sf::RenderWindow& window, Reversi& game) override;
};