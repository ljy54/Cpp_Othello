#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <windows.h>
#include <commdlg.h>

#include "AIPlayer.h"
//#include "Reversi.h"
#include "GameOverState.h"
#include "OpenState.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "Constants.h"
#include "Enums.h"
#include "Reversi.h"   
#include "Picture.h"

Reversi::Reversi(sf::RenderWindow& win) :
    window(win), board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE, Piece::Empty)), 
    gameState(GameState::Opening),currentPlayer(Piece::Black), gameMode(GameMode::None), 
    aiPlayer(AIDifficulty::Medium), boardColor(BOARD_COLOR_GREEN), 
    boardBrightness(1.0f), brightnessStep(2) {

    // 加载音效
    if (!chessBuffer.loadFromFile("chess.wav")) {
        std::wcerr << L"下棋音效加载失败!" << std::endl;
    }
    chessSound.setBuffer(chessBuffer);

    if (!clickBuffer.loadFromFile("click.wav")) {
        std::wcerr << L"点击音效加载失败!" << std::endl;
    }
    clickSound.setBuffer(clickBuffer);

    // 加载字体
    if (!font.loadFromFile("C:/Windows/Fonts/simsun.ttc")) {
        std::wcerr << L"无法加载该字体! 将尝试使用其他字体。" << std::endl;
    }

}

// 修改获取难度字符串的函数
std::wstring Reversi::getDifficultyString() const {
    switch (aiPlayer.getDifficulty()) {
    case AIDifficulty::Easy: return L"简单";
    case AIDifficulty::Medium: return L"中等";
    case AIDifficulty::Hard: return L"困难";
    default: return L"未知";
    }
}

// 获取棋盘颜色字符串
std::wstring Reversi::getBoardColorString() const {
    if (boardColor == BOARD_COLOR_GREEN) return L"绿色";
    else if (boardColor == BOARD_COLOR_YELLOW) return L"黄色";
    else if (boardColor == BOARD_COLOR_PURPLE) return L"紫色";
    else return L"未知";
}

std::wstring Reversi::getBrightnessString() const {
    // 将亮度系数转换为百分比字符串（保留整数）
    int percentage = static_cast<int>(boardBrightness * 100);
    return std::to_wstring(percentage) + L"%";
}



// 检查落子位置是否有效
bool Reversi::isValidMove(int x, int y) const {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || board[x][y] != Piece::Empty) {
        return false;
    }

    Piece opponent = (currentPlayer == Piece::Black) ? Piece::White : Piece::Black;
    const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& dir : directions) {
        int dx = dir[0];
        int dy = dir[1];
        int nx = x + dx;
        int ny = y + dy;
        bool foundOpponent = false;

        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
            if (board[nx][ny] == opponent) {
                foundOpponent = true;
                nx += dx;
                ny += dy;
            }
            else if (board[nx][ny] == currentPlayer) {
                if (foundOpponent) {
                    return true;
                }
                break;
            }
            else {
                break;
            }
        }
    }
    return false;
}

// 初始化棋盘布局
void Reversi::initializeBoard() {
    // 清空棋盘
    for (auto& row : board) {
        for (auto& piece : row) {
            piece = Piece::Empty;
        }
    }

    int center = BOARD_SIZE / 2 - 1;
    board[center][center] = Piece::White;
    board[center + 1][center + 1] = Piece::White;
    board[center][center + 1] = Piece::Black;
    board[center + 1][center] = Piece::Black;

    currentPlayer = Piece::Black;
    gameState = GameState::Playing;
}

// 执行落子操作
void Reversi::makeMove(int x, int y) {
    if (gameState != GameState::Playing || !isValidMove(x, y)) {
        return;
    }
    // 创建历史记录
    MoveRecord record;
    record.x = x;
    record.y = y;
    record.player = currentPlayer;

    board[x][y] = currentPlayer;
    Piece opponent = (currentPlayer == Piece::Black) ? Piece::White : Piece::Black;

    const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    // 使用一个总的翻转列表
    std::vector<std::pair<int, int>> allToFlip;

    for (const auto& dir : directions) {
        int dx = dir[0];
        int dy = dir[1];
        int nx = x + dx;
        int ny = y + dy;
        std::vector<std::pair<int, int>> toFlip;

        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
            if (board[nx][ny] == opponent) {
                toFlip.emplace_back(nx, ny);
                nx += dx;
                ny += dy;
            }
            else if (board[nx][ny] == currentPlayer) {
                
                    // 添加到总翻转列表
                    allToFlip.insert(allToFlip.end(), toFlip.begin(), toFlip.end());
                  
                break;
            }
            else {
                toFlip.clear();
                break;
            }
        }
    }


    // 执行翻转
    for (const auto& pos : allToFlip) {
        board[pos.first][pos.second] = currentPlayer;
    }

    // 保存翻转信息到历史记录
    record.flippedPieces = allToFlip;
    moveHistory.push(record);


    // 切换玩家
    currentPlayer = (currentPlayer == Piece::Black) ? Piece::White : Piece::Black;

    // 检查游戏是否结束
    chessSound.play();
    checkGameOver();

    // 如果是单人模式且当前是AI回合，执行AI移动
    if (gameMode == GameMode::SinglePlayer && currentPlayer == Piece::White && gameState == GameState::Playing) {
        makeAIMove();
    }
}
// 通用悔棋方法
void Reversi::undoMove() {
    if (moveHistory.empty()) return;

    MoveRecord lastMove = moveHistory.top();
    moveHistory.pop();

    // 移除落子位置的棋子
    board[lastMove.x][lastMove.y] = Piece::Empty;

    // 恢复被翻转的棋子
    Piece opponent = (lastMove.player == Piece::Black) ? Piece::White : Piece::Black;
    for (const auto& pos : lastMove.flippedPieces) {
        board[pos.first][pos.second] = opponent;
    }

    // 恢复当前玩家
    currentPlayer = lastMove.player;

    // 恢复游戏状态
    gameState = GameState::Playing;
}

// 检查游戏是否结束
void Reversi::checkGameOver() {
    // 检查新玩家是否有合法移动
    bool hasValidMove = false;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == Piece::Empty && isValidMove(i, j)) {
                hasValidMove = true;
                goto endCheck;
            }
        }
    }
endCheck:

    if (!hasValidMove) {
        // 当前玩家无移动，换回原玩家    
        currentPlayer = (currentPlayer == Piece::Black) ? Piece::White : Piece::Black;

        // 检查原玩家是否有合法移动
        bool originalHasMove = false;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == Piece::Empty && isValidMove(i, j)) {
                    originalHasMove = true;
                    goto endOriginalCheck;
                }
            }
        }
    endOriginalCheck:

        if (!originalHasMove) {
            // 双方都无合法移动，游戏结束
            gameState = GameState::GameOver;
            std::cout << "Game Over!" << std::endl;
        }
    }
}

// AI执行移动
void Reversi::makeAIMove() {
    // 使用 AIPlay 类进行决策
    auto move = aiPlayer.makeDecision(board, currentPlayer);

    if (move.first != -1 && move.second != -1) {
        makeMove(move.first, move.second);
    }
    else {
        // 没有有效移动
        checkGameOver();
    }
}

// 计算某个位置可以翻转的棋子数
int Reversi::countFlips(int x, int y) const {
    if (!isValidMove(x, y)) return 0;

    int totalFlips = 0;
    Piece opponent = (currentPlayer == Piece::Black) ? Piece::White : Piece::Black;
    const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& dir : directions) {
        int dx = dir[0];
        int dy = dir[1];
        int nx = x + dx;
        int ny = y + dy;
        int directionFlips = 0;

        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
            if (board[nx][ny] == opponent) {
                directionFlips++;
                nx += dx;
                ny += dy;
            }
            else if (board[nx][ny] == currentPlayer) {
                totalFlips += directionFlips;
                break;
            }
            else {
                break;
            }
        }
    }
    return totalFlips;
}

// 统计黑白棋子数量
void Reversi::countPieces(int& black, int& white) const {
    black = white = 0;
    for (const auto& row : board) {
        for (const auto& piece : row) {
            if (piece == Piece::Black) black++;
            else if (piece == Piece::White) white++;
        }
    }
}


// 加载背景图片
bool Reversi::loadBackgroundImage(const std::string& path) {
    if (path.empty()) {
        // 使用默认背景
        backgroundImagePath.clear();
        if (!backgroundTexture.loadFromFile("background.jpg")) {
            // 如果默认背景加载失败，使用纯色背景
            std::wcerr << L"默认背景加载失败" << std::endl;
            backgroundTexture = sf::Texture(); // 重置纹理
            backgroundSprite = sf::Sprite();   // 重置精灵
        }
        else {
            backgroundSprite.setTexture(backgroundTexture, true);
            // 调整图片大小以适应窗口
            float scaleX = float(WINDOW_WIDTH) / backgroundTexture.getSize().x;
            float scaleY = float(WINDOW_HEIGHT) / backgroundTexture.getSize().y;
            backgroundSprite.setScale(scaleX, scaleY);
        }
        return true;
    }

    if (backgroundTexture.loadFromFile(path)) {
        backgroundImagePath = path;
        backgroundSprite.setTexture(backgroundTexture, true);
        // 调整图片大小以适应窗口
        float scaleX = float(WINDOW_WIDTH) / backgroundTexture.getSize().x;
        float scaleY = float(WINDOW_HEIGHT) / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);
        return true;
    }
    std::wcerr << L"无法加载背景图片: " << path.c_str() << std::endl;
    return false;
}

void Reversi::setState(State* state) {
    if (currentState) delete currentState;
    currentState = state;
}

