#pragma once
#include <vector>
#include <random>
#include <chrono>
#include "Enums.h"
#include "Constants.h"

class AIPlayer {
private:
    std::mt19937 rng;
    AIDifficulty difficulty;

    // 评估当前局面
    int evaluatePosition(const std::vector<std::vector<Piece>>& board, Piece aiPiece) const;

    // 计算移动分数
    int calculateMoveScore(const std::vector<std::vector<Piece>>& board,
        int x, int y, Piece aiPiece) const;

    // 获取位置权重
    std::vector<std::vector<int>> getPositionWeights() const;

public:
    AIPlayer(AIDifficulty diff);
    void setDifficulty(AIDifficulty diff);
    AIDifficulty getDifficulty() const;

    // AI 做决策
    std::pair<int, int> makeDecision(const std::vector<std::vector<Piece>>& board,
        Piece currentPlayer);

    // 随机移动
    std::pair<int, int> makeRandomMove(const std::vector<std::pair<int, int>>& validMoves);

    // 最佳移动
    std::pair<int, int> makeBestMove(const std::vector<std::vector<Piece>>& board,
        const std::vector<std::pair<int, int>>& validMoves,
        Piece aiPiece);
};