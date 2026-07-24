#include "PlayingState.h"
#include "Reversi.h"
#include "MenuState.h"
#include "GameOverState.h"
#include <iostream>

PlayingState::PlayingState(Reversi& game) : game(game) {
    // 初始化投降按钮
    surrenderButton.setRadius(60);
    surrenderButton.setFillColor(sf::Color(200, 50, 50));
    surrenderButton.setOutlineThickness(4);
    surrenderButton.setOutlineColor(sf::Color::White);
    surrenderButton.setPosition(WINDOW_WIDTH - 230, WINDOW_HEIGHT - 170);
    surrenderText.setFont(game.getFont());
    surrenderText.setString(L"投降");
    surrenderText.setCharacterSize(50);
    surrenderText.setStyle(sf::Text::Bold);
    surrenderText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = surrenderText.getLocalBounds();
    surrenderText.setOrigin((textRect.left + textRect.width) / 2.0f,
        (textRect.top + textRect.height) / 2.0f);
    surrenderText.setPosition(WINDOW_WIDTH - 170, WINDOW_HEIGHT - 115);

    currentText.setFont(game.getFont());
    currentText.setCharacterSize(40);
    currentText.setStyle(sf::Text::Bold);
	
	countTextBlack.setFont(game.getFont());
	countTextBlack.setCharacterSize(35);
    countTextBlack.setStyle(sf::Text::Bold);
	countTextBlack.setFillColor(sf::Color::Black);

	countTextWhite.setFont(game.getFont());
	countTextWhite.setCharacterSize(35);
    countTextWhite.setStyle(sf::Text::Bold);
	countTextWhite.setFillColor(sf::Color::White);


    // 初始化悔棋按钮
    undoButton.setRadius(60);
    undoButton.setFillColor(sf::Color(50, 150, 200));
    undoButton.setOutlineThickness(4);
    undoButton.setOutlineColor(sf::Color::White);
    undoButton.setPosition(WINDOW_WIDTH - 230, WINDOW_HEIGHT - 300); // 放在投降按钮上方

    undoText.setFont(game.getFont());
    undoText.setString(L"悔棋");
    undoText.setCharacterSize(50);
    undoText.setStyle(sf::Text::Bold);
    undoText.setFillColor(sf::Color::White);
    sf::FloatRect undoRect = undoText.getLocalBounds();
    undoText.setOrigin((undoRect.left + undoRect.width) / 2.0f,
        (undoRect.top + undoRect.height) / 2.0f);
    undoText.setPosition(WINDOW_WIDTH - 170, WINDOW_HEIGHT - 245);
    
	surrenderHovered = false;
}

void PlayingState::handleEvent(const sf::Event& event, Reversi& game) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(game.getWindow());
            sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(pixelPos);

            // 检查投降按钮
            sf::Vector2f buttonCenter(WINDOW_WIDTH - 170, WINDOW_HEIGHT - 115);
            float radius = 60;
            float dx = mousePos.x - buttonCenter.x;
            float dy = mousePos.y - buttonCenter.y;

            if (dx * dx + dy * dy <= radius * radius) {
                game.getClickSound().play();
                game.setState(new GameOverState(game));
                return;
            }
            // 检查悔棋按钮
            sf::Vector2f undoCenter(WINDOW_WIDTH - 170, WINDOW_HEIGHT - 215);
            float undoRadius = 60;
            dx = mousePos.x - undoCenter.x;
            dy = mousePos.y - undoCenter.y;

            if (dx * dx + dy * dy <= undoRadius * undoRadius && game.canUndo()) {
                game.getClickSound().play();
                // 修改悔棋逻辑：人机模式直接调用 undoPlayerMove()
                if (game.getGameMode() == GameMode::SinglePlayer) {
                    game.undoMove();
                    game.undoMove();
                }
                else {
                    // 双人模式：只撤销一步
                    game.undoMove();
                }
                return;
            }
            // 处理棋盘点击
            if (game.getGameMode() == GameMode::SinglePlayer && game.getCurrentPlayer() == Piece::White) {
                return; // AI回合，不处理玩家点击
            }

            // 计算点击的棋盘位置
            int boardX = (mousePos.x - (WINDOW_WIDTH - WINDOW_HEIGHT) / 2) / CELL_SIZE;
            int boardY = mousePos.y / CELL_SIZE;

            if (boardX >= 0 && boardX < BOARD_SIZE && boardY >= 0 && boardY < BOARD_SIZE) {
                if (game.isValidMove(boardX, boardY)) {
                    game.makeMove(boardX, boardY);
                    game.getChessSound().play();

                    // 检查游戏是否结束
                    if (game.getGameState() == GameState::GameOver) {
                        game.setState(new GameOverState(game));
                    }
                }
            }
        }
    }
}

void PlayingState::update(Reversi& game) {
    // 更新投降按钮悬停状态
    sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f buttonCenter(WINDOW_WIDTH - 170, WINDOW_HEIGHT - 115);
    float radius = 60;
    float distance = sqrt(pow(mousePos.x - buttonCenter.x, 2) + pow(mousePos.y - buttonCenter.y, 2));

    bool wasHovered = surrenderHovered;
    surrenderHovered = (distance <= radius);

    if (surrenderHovered != wasHovered) {
        surrenderButton.setFillColor(surrenderHovered ? sf::Color(220, 70, 70) : sf::Color(200, 50, 50));
    }
    // 更新悔棋按钮悬停状态
    mousePos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f undoCenter(WINDOW_WIDTH - 170, WINDOW_HEIGHT - 215);
    float undoRadius = 60;
   distance = sqrt(pow(mousePos.x - undoCenter.x, 2) +
        pow(mousePos.y - undoCenter.y, 2));

    bool wasUndoHovered = undoHovered;
    undoHovered = (distance <= undoRadius) && game.canUndo();

    if (undoHovered != wasUndoHovered) {
        undoButton.setFillColor(undoHovered ?
            sf::Color(70, 170, 220) :
            sf::Color(50, 150, 200));
    }
    // 更新当前玩家文本
    currentText.setString(game.getCurrentPlayer() == Piece::Black ? L"当前玩家: 黑色" : L"当前玩家: 白色");
    currentText.setFillColor(game.getCurrentPlayer() == Piece::Black ? sf::Color::Black : sf::Color::White);
	sf::FloatRect textRect = currentText.getLocalBounds();
    currentText.setOrigin((game.getCurrentPlayer() == Piece::Black ? textRect.left : textRect.left + textRect.width),
        textRect.top + textRect.height / 2.0f);
    currentText.setPosition((game.getCurrentPlayer() == Piece::Black ? 40 : WINDOW_WIDTH - 40), 150);

    // 更新棋子计数
    int blackCount, whiteCount;
    game.countPieces(blackCount, whiteCount);
    countTextBlack.setString(L"黑色得分: " + std::to_wstring(blackCount));
    textRect = countTextBlack.getLocalBounds();
    countTextBlack.setOrigin(textRect.left,
        textRect.top + textRect.height / 2.0f);
    countTextBlack.setPosition(80, 250);
    countTextWhite.setString(L"白色得分: " + std::to_wstring(whiteCount));
    textRect = countTextWhite.getLocalBounds();
    countTextWhite.setOrigin(textRect.left + textRect.width,
        textRect.top + textRect.height / 2.0f);
    countTextWhite.setPosition(WINDOW_WIDTH - 80, 250);
}

void PlayingState::draw(sf::RenderWindow& window, Reversi& game) {
    // 1. 绘制棋盘背景
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    sf::Color brightenedColor = game.getBoardColor();
    float brightness = game.getBoardBrightness();
    brightenedColor.r = static_cast<sf::Uint8>(std::min(255, std::max(0, static_cast<int>(game.getBoardColor().r * brightness))));
    brightenedColor.g = static_cast<sf::Uint8>(std::min(255, std::max(0, static_cast<int>(game.getBoardColor().g * brightness))));
    brightenedColor.b = static_cast<sf::Uint8>(std::min(255, std::max(0, static_cast<int>(game.getBoardColor().b * brightness))));
    background.setFillColor(brightenedColor);
    window.draw(background);

    // 2. 绘制棋盘网格
    sf::RectangleShape verticalLine(sf::Vector2f(2, WINDOW_HEIGHT));
    verticalLine.setFillColor(sf::Color::Black);
    sf::RectangleShape horizontalLine(sf::Vector2f(WINDOW_HEIGHT, 2));
    horizontalLine.setFillColor(sf::Color::Black);

    for (int i = 0; i <= BOARD_SIZE; ++i) {
        verticalLine.setPosition((WINDOW_WIDTH - WINDOW_HEIGHT) / 2 + i * CELL_SIZE, 0);
        window.draw(verticalLine);
        horizontalLine.setPosition((WINDOW_WIDTH - WINDOW_HEIGHT) / 2, i * CELL_SIZE);
        window.draw(horizontalLine);
    }

    // 3. 绘制棋子
    sf::CircleShape piece(CELL_SIZE / 2 - 4);
    const auto& board = game.getBoard();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != Piece::Empty) {
                piece.setPosition((WINDOW_WIDTH - WINDOW_HEIGHT) / 2 + i * CELL_SIZE + 4, j * CELL_SIZE + 4);
                piece.setFillColor(board[i][j] == Piece::Black ? sf::Color::Black : sf::Color::White);
                window.draw(piece);
            }
        }
    }

    // 4. 绘制投降按钮
    window.draw(surrenderButton);
    window.draw(surrenderText);
   
    window.draw(undoButton);
    window.draw(undoText);
    
    // 5. 绘制当前玩家信息
    window.draw(currentText);

    // 6. 绘制棋子计数
    window.draw(countTextBlack);
    window.draw(countTextWhite);
}