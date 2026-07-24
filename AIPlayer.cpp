#include "AIPlayer.h"
#include "Constants.h"
#include <algorithm>

AIPlayer::AIPlayer(AIDifficulty diff = AIDifficulty::Medium) : difficulty(diff) {
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void AIPlayer::setDifficulty(AIDifficulty diff) {
    difficulty = diff;
}

AIDifficulty AIPlayer::getDifficulty() const {
    return difficulty;
}

int AIPlayer::evaluatePosition(const std::vector<std::vector<Piece>>& board, Piece aiPiece) const {
    int score = 0;
    Piece opponent = (aiPiece == Piece::Black) ? Piece::White : Piece::Black;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == aiPiece) {
                score++;
            }
            else if (board[i][j] == opponent) {
                score--;
            }
        }
    }
    return score;
}

std::pair<int, int> AIPlayer::makeDecision(const std::vector<std::vector<Piece>>& board,
    Piece currentPlayer) {
    // 获取有效移动
    std::vector<std::pair<int, int>> validMoves;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == Piece::Empty) {
                // 检查是否为有效移动
                bool isValid = false;
                Piece opponent = (currentPlayer == Piece::Black) ? Piece::White : Piece::Black;
                const int directions[8][2] = {
                    {-1, -1}, {-1, 0}, {-1, 1},
                    {0, -1},           {0, 1},
                    {1, -1},  {1, 0},  {1, 1}
                };

                for (const auto& dir : directions) {
                    int dx = dir[0];
                    int dy = dir[1];
                    int nx = i + dx;
                    int ny = j + dy;
                    bool foundOpponent = false;

                    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                        if (board[nx][ny] == opponent) {
                            foundOpponent = true;
                            nx += dx;
                            ny += dy;
                        }
                        else if (board[nx][ny] == currentPlayer) {
                            if (foundOpponent) {
                                isValid = true;
                                break;
                            }
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    if (isValid) break;
                }

                if (isValid) {
                    validMoves.emplace_back(i, j);
                }
            }
        }
    }

    if (validMoves.empty()) {
        return { -1, -1 }; // 表示没有有效移动
    }

    // 根据难度选择移动
    switch (difficulty) {
    case AIDifficulty::Easy:
        return makeRandomMove(validMoves);
    case AIDifficulty::Medium: {
        std::uniform_int_distribution<int> dist(0, 99);
        if (dist(rng) < 70) { // 70%概率选择最佳移动
            return makeBestMove(board, validMoves, currentPlayer);
        }
        else {
            return makeRandomMove(validMoves);
        }
    }
    case AIDifficulty::Hard:
        return makeBestMove(board, validMoves, currentPlayer);
    default:
        return makeRandomMove(validMoves);
    }
}

std::pair<int, int> AIPlayer::makeRandomMove(const std::vector<std::pair<int, int>>& validMoves) {
    std::uniform_int_distribution<int> dist(0, validMoves.size() - 1);
    int choice = dist(rng);
    return validMoves[choice];
}

std::pair<int, int> AIPlayer::makeBestMove(const std::vector<std::vector<Piece>>& board,
    const std::vector<std::pair<int, int>>& validMoves,
    Piece aiPiece) {
    std::vector<std::vector<int>> positionWeights = getPositionWeights();

    int maxScore = -1000;
    std::vector<std::pair<int, int>> bestMoves;

    for (const auto& move : validMoves) {
        int score = calculateMoveScore(board, move.first, move.second, aiPiece);

        if (score > maxScore) {
            maxScore = score;
            bestMoves.clear();
            bestMoves.push_back(move);
        }
        else if (score == maxScore) {
            bestMoves.push_back(move);
        }
    }

    if (!bestMoves.empty()) {
        // 在最佳移动中随机选择一个
        return makeRandomMove(bestMoves);
    }

    return { -1, -1 }; // 没找到移动
}

std::vector<std::vector<int>> AIPlayer::getPositionWeights() const {
    std::vector<std::vector<int>> weights(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));

    // 角点权重高
    weights[0][0] = 99;
    weights[0][BOARD_SIZE - 1] = 99;
    weights[BOARD_SIZE - 1][0] = 99;
    weights[BOARD_SIZE - 1][BOARD_SIZE - 1] = 99;

    // 边缘位置权重较低
    for (int i = 1; i < BOARD_SIZE - 1; ++i) {
        weights[0][i] = -8;
        weights[BOARD_SIZE - 1][i] = -8;
        weights[i][0] = -8;
        weights[i][BOARD_SIZE - 1] = -8;
    }

    return weights;
}

int AIPlayer::calculateMoveScore(const std::vector<std::vector<Piece>>& board,
    int x, int y, Piece aiPiece) const {
    auto positionWeights = getPositionWeights();
    Piece opponent = (aiPiece == Piece::Black) ? Piece::White : Piece::Black;

    // 1. 计算翻转分数
    int flipScore = 0;
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
            else if (board[nx][ny] == aiPiece) {
                flipScore += directionFlips;
                break;
            }
            else {
                break;
            }
        }
    }

    // 2. 位置权重
    int positionScore = positionWeights[x][y];

    // 3. 计算对手的移动性
    int opponentMobility = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == Piece::Empty) {
                // 检查是否为有效移动
                bool isValid = false;
                for (const auto& dir : directions) {
                    int dx = dir[0];
                    int dy = dir[1];
                    int nx = i + dx;
                    int ny = j + dy;
                    bool foundOpponent = false;

                    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                        if (board[nx][ny] == aiPiece) {
                            foundOpponent = true;
                            nx += dx;
                            ny += dy;
                        }
                        else if (board[nx][ny] == opponent) {
                            if (foundOpponent) {
                                isValid = true;
                                break;
                            }
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    if (isValid) break;
                }

                if (isValid) {
                    opponentMobility++;
                }
            }
        }
    }

    // 4. 综合评分: 翻转分数 + 位置权重 - 对手移动性
    int totalScore = flipScore * 2 + positionScore * 3 - opponentMobility;

    // 5. 如果是角点，额外加分
    if ((x == 0 || x == BOARD_SIZE - 1) && (y == 0 || y == BOARD_SIZE - 1)) {
        totalScore += 50;
    }

    return totalScore;
}