#pragma once
// 游戏状态枚举
enum class GameState {
    Opening,
    Menu,       // 菜单界面
    Playing,    // 游戏进行中
    GameOver    // 游戏结束
};

// 游戏模式枚举
enum class GameMode {
    None,
    SinglePlayer,  // 单人模式
    TwoPlayer,     // 双人模式
};

// AI难度枚举
enum class AIDifficulty {
    Easy,
    Medium,
    Hard
};

// 棋子类型枚举
enum class Piece {
    Empty,
    Black,
    White
};