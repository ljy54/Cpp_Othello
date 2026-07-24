#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "OpenState.h"
#include "State.h"
#include "Enums.h"
#include "Constants.h"
#include "AIPlayer.h"
#include <stack>
class State;
class OpenState;
class MenuState;
class PlayingState;
class GameOverState;
struct MoveRecord {
    int x, y;
    std::vector<std::pair<int, int>> flippedPieces;
    Piece player;
};
class Reversi {
private:
    //类组合
    std::vector<std::vector<Piece>> board;
    Piece currentPlayer;

    GameState gameState;
    GameMode gameMode;
    AIPlayer aiPlayer;
    sf::Color boardColor;
    float boardBrightness;
    int brightnessStep;

    // 资源
    sf::RenderWindow& window;
    sf::SoundBuffer chessBuffer;
    sf::Sound chessSound;
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::string backgroundImagePath;
    std::stack<MoveRecord> moveHistory; // 添加历史记录栈

public:
    Reversi(sf::RenderWindow& win);

    State* currentState;
    // 游戏逻辑方法
    void initializeBoard();
    bool isValidMove(int x, int y) const;
    void makeMove(int x, int y);
    void checkGameOver();
    void makeAIMove();
    int countFlips(int x, int y) const;
    void countPieces(int& black, int& white) const;
    void undoMove(); // 添加悔棋方法
    bool canUndo() const { return !moveHistory.empty(); } // 检查是否可以悔棋

    // 状态管理
    AIPlayer& getAIPlayer() { return aiPlayer; }

    // 辅助方法
    std::wstring getDifficultyString() const;
    std::wstring getBoardColorString() const;
    std::wstring getBrightnessString() const;
    bool loadBackgroundImage(const std::string& path);

    // 获取器方法
    const std::vector<std::vector<Piece>>& getBoard() const { return board; }
    Piece getCurrentPlayer() const { return currentPlayer; }
    GameState getGameState() const { return gameState; }
    GameMode getGameMode() const { return gameMode; }
    const sf::Font& getFont() const { return font; }
    sf::Sound& getChessSound() { return chessSound; }
    sf::Sound& getClickSound() { return clickSound; }
    const std::string& getBackgroundImagePath() const { return backgroundImagePath; }
    const sf::Texture& getBackgroundTexture() const { return backgroundTexture; }
    const sf::Sprite& getBackgroundSprite() const { return backgroundSprite; }

    sf::RenderWindow& getWindow() { return window; }
    const sf::RenderWindow& getWindow() const { return window; }
    sf::Color getBoardColor() const { return boardColor; }
    float getBoardBrightness() const { return boardBrightness; }
    int getBrightnessStep() const { return brightnessStep; }
    
    // 设置器方法
    void setGameMode(GameMode mode) { gameMode = mode; }
    void setCurrentPlayer(Piece player) { currentPlayer = player; }
    void setState(State* state);
    void setBoardColor(sf::Color color) { boardColor = color; };
    void setBoardBrightness(float brightness) { boardBrightness = brightness; };
    void setBrightnessStep(int step) { brightnessStep = step; };

};