#pragma once
#include <SFML/Graphics.hpp>

// 游戏常量定义
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;
const int BOARD_SIZE = 6;
const int CELL_SIZE = WINDOW_HEIGHT / BOARD_SIZE;
const int BOARD_OFFSET_X = (WINDOW_WIDTH - WINDOW_HEIGHT) / 2;

const sf::Color BOARD_COLOR_GREEN = sf::Color(0, 100, 0);        //绿色
const sf::Color BOARD_COLOR_YELLOW = sf::Color(170, 120, 10);      //黄色    
const sf::Color BOARD_COLOR_PURPLE = sf::Color(170, 70, 140);     //紫色

// 亮度调节参数
const int BRIGHTNESS_STEPS = 5;  // 亮度等级数（1-5级）
const float BRIGHTNESS_MIN = 0.5f;  // 最低亮度（50%）
const float BRIGHTNESS_MAX = 1.5f;  // 最高亮度（150%）

struct MenuItem {
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered;
};


