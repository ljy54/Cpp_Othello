#include "GameOverState.h"
#include "MenuState.h"
#include "Reversi.h"
#include "PlayingState.h"
#include <iostream>

GameOverState::GameOverState(Reversi& game) : game(game), buttonHovered(false) {
    // 加载字体
    if (!font.loadFromFile("C:/Windows/Fonts/simsun.ttc")) {
        std::wcerr << L"无法加载字体! 将使用默认字体。" << std::endl;
    }

    // 初始化游戏结束文本
    gameOverText.setFont(font);
    gameOverText.setString(L"游戏结束");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);

    sf::FloatRect gameOverRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(gameOverRect.left + gameOverRect.width / 2.0f,
        gameOverRect.top + gameOverRect.height / 2.0f);
    gameOverText.setPosition(WINDOW_WIDTH / 2, 250);

    // 初始化分数文本
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);

    // 初始化胜利者文本
    winnerText.setFont(font);
    winnerText.setCharacterSize(50);
    winnerText.setFillColor(sf::Color::Yellow);
    winnerText.setStyle(sf::Text::Bold);

    // 初始化返回菜单按钮
    menuButton.setSize(sf::Vector2f(300, 60));
    menuButton.setFillColor(sf::Color(70, 70, 70));
    menuButton.setOutlineThickness(2);
    menuButton.setOutlineColor(sf::Color::White);
    menuButton.setPosition((WINDOW_WIDTH - 300) / 2, 550);

    menuButtonText.setFont(font);
    menuButtonText.setString(L"返回主菜单");
    menuButtonText.setCharacterSize(30);
    menuButtonText.setFillColor(sf::Color::White);

    sf::FloatRect buttonTextRect = menuButtonText.getLocalBounds();
    menuButtonText.setOrigin(buttonTextRect.left + buttonTextRect.width / 2.0f,
        buttonTextRect.top + buttonTextRect.height / 2.0f);
    menuButtonText.setPosition(WINDOW_WIDTH / 2, 580);
}

void GameOverState::handleEvent(const sf::Event& event, Reversi& game) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(game.getWindow());
            sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(pixelPos);

            if (menuButton.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y))) {
                game.getClickSound().play();
                game.setState(new MenuState(game));
            }
        }
    }
}

void GameOverState::update(Reversi& game) {
    // 更新分数和胜利者信息
    int blackCount, whiteCount;
    game.countPieces(blackCount, whiteCount);

    std::wstring scoreStr = L"黑棋: " + std::to_wstring(blackCount) +
        L"  白棋: " + std::to_wstring(whiteCount);
    scoreText.setString(scoreStr);

    sf::FloatRect scoreRect = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreRect.left + scoreRect.width / 2.0f,
        scoreRect.top + scoreRect.height / 2.0f);
    scoreText.setPosition(WINDOW_WIDTH / 2, 350);

    // 确定胜利者
    std::wstring winnerStr;
    if (blackCount + whiteCount < BOARD_SIZE * BOARD_SIZE) {
        if (game.getCurrentPlayer() == Piece::Black) {
            winnerStr = L"黑方投降！白方胜利！";
        }
        else {
            winnerStr = L"白方投降！黑方胜利！";
        }
    }
    else if (blackCount > whiteCount) {
        winnerStr = L"黑棋胜利!";
    }
    else if (whiteCount > blackCount) {
        winnerStr = L"白棋胜利!";
    }
    else {
        winnerStr = L"平局!";
    }
    winnerText.setString(winnerStr);

    sf::FloatRect winnerRect = winnerText.getLocalBounds();
    winnerText.setOrigin(winnerRect.left + winnerRect.width / 2.0f,
        winnerRect.top + winnerRect.height / 2.0f);
    winnerText.setPosition(WINDOW_WIDTH / 2, 450);

    // 更新按钮悬停状态
    sf::Vector2i pixelPos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(pixelPos);
    bool wasHovered = buttonHovered;
    buttonHovered = menuButton.getGlobalBounds().contains(static_cast<float>(mousePos.x),
        static_cast<float>(mousePos.y));

    if (buttonHovered != wasHovered) {
        menuButton.setFillColor(buttonHovered ? sf::Color(100, 100, 100) : sf::Color(70, 70, 70));
        menuButtonText.setFillColor(buttonHovered ? sf::Color::Yellow : sf::Color::White);
    }
}

void GameOverState::draw(sf::RenderWindow& window, Reversi& game) {
    
    PlayingState playingState(game);
    playingState.draw(window, game);

    // 添加半透明覆盖层
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    // 绘制游戏结束信息
    window.draw(gameOverText);
    window.draw(scoreText);
    window.draw(winnerText);

    // 绘制按钮
    window.draw(menuButton);
    window.draw(menuButtonText);
}