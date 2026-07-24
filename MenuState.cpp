#include "MenuState.h"
#include "Reversi.h"
#include "PlayingState.h"
#include "Picture.h"
#include <iostream>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>

MenuState::MenuState(Reversi& game) : game(game), aiPlayer(game.getAIPlayer()) {
    // 尝试加载字体文件
    if (!font.loadFromFile("C:/Windows/Fonts/simsun.ttc")) {
        std::wcerr << L"无法加载该字体! 将尝试使用其他字体。" << std::endl;
    }
    initializeMenu();
    game.loadBackgroundImage("");
}

void MenuState::initializeMenu(bool inModeSel, bool inSettings, bool inRules, bool inBackground) {
    inModeSelection = inModeSel;
    inSettingsMenu = inSettings;
    inRulesMenu = inRules;
    inBackgroundSetting = inBackground;

    menuItems.clear();
    if (inModeSelection) {
        // 模式选择菜单
        std::vector<std::wstring> itemTexts = { L"人机对局", L"双人对局", L"返回" };

        for (size_t i = 0; i < itemTexts.size(); ++i) {
            MenuItem item;
            item.shape.setSize(sf::Vector2f(300, 60));
            item.shape.setFillColor(sf::Color(70, 70, 70));
            item.shape.setOutlineThickness(2);
            item.shape.setOutlineColor(sf::Color::White);

            float xPos = (WINDOW_WIDTH - item.shape.getSize().x) / 2;
            float yPos = 300 + i * 100;
            item.shape.setPosition(xPos, yPos);

            item.text.setFont(font);
            item.text.setString(itemTexts[i]);
            item.text.setCharacterSize(30);
            item.text.setFillColor(sf::Color::White);

            sf::FloatRect textRect = item.text.getLocalBounds();
            item.text.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            item.text.setPosition(xPos + item.shape.getSize().x / 2,
                yPos + item.shape.getSize().y / 2);

            item.isHovered = false;
            menuItems.push_back(item);
        }
    }
    else if (inSettingsMenu) {
        // 设置菜单
        std::vector<std::wstring> itemTexts = {
            L"困难度: " + game.getDifficultyString(),
            L"棋盘颜色: " + game.getBoardColorString(),
            L"棋盘亮度: " + game.getBrightnessString(),         // 显示当前亮度
            L"背景设置",
            L"返回"
        };

        for (size_t i = 0; i < itemTexts.size(); ++i) {
            MenuItem item;
            item.shape.setSize(sf::Vector2f(350, 60));
            item.shape.setFillColor(sf::Color(70, 70, 70));
            item.shape.setOutlineThickness(2);
            item.shape.setOutlineColor(sf::Color::White);

            float xPos = (WINDOW_WIDTH - item.shape.getSize().x) / 2;
            float yPos = 300 + i * 100;
            item.shape.setPosition(xPos, yPos);

            item.text.setFont(font);
            item.text.setString(itemTexts[i]);
            item.text.setCharacterSize(30);
            item.text.setFillColor(sf::Color::White);

            sf::FloatRect textRect = item.text.getLocalBounds();
            item.text.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            item.text.setPosition(xPos + item.shape.getSize().x / 2,
                yPos + item.shape.getSize().y / 2);

            item.isHovered = false;
            menuItems.push_back(item);
        }
    }
    else if (inRulesMenu) {
        loadRulesFromFile(L"rules.txt");
        rulesScrollOffset = 0.0f;

        // 返回按钮
        backButtonShape.setSize(sf::Vector2f(180, 50));
        backButtonShape.setFillColor(sf::Color(70, 70, 70));
        backButtonShape.setOutlineThickness(2);
        backButtonShape.setOutlineColor(sf::Color::White);
        float bx = (WINDOW_WIDTH - backButtonShape.getSize().x) / 2;
        float by = WINDOW_HEIGHT - 100;
        backButtonShape.setPosition(bx, by);

        backButtonText.setFont(font);
        backButtonText.setString(L"返回");
        backButtonText.setCharacterSize(28);
        backButtonText.setFillColor(sf::Color::White);
        sf::FloatRect trect = backButtonText.getLocalBounds();
        backButtonText.setOrigin(trect.left + trect.width / 2.0f, trect.top + trect.height / 2.0f);
        backButtonText.setPosition(bx + backButtonShape.getSize().x / 2, by + backButtonShape.getSize().y / 2);
    }
    else if (inBackgroundSetting) {
        // 背景选择菜单
        std::vector<std::wstring> itemTexts = { L"选择图片", L"默认背景", L"返回" };
        // ... 创建菜单项 ...
        for (size_t i = 0; i < itemTexts.size(); ++i) {
            MenuItem item;
            item.shape.setSize(sf::Vector2f(300, 60));
            item.shape.setFillColor(sf::Color(70, 70, 70));
            item.shape.setOutlineThickness(2);
            item.shape.setOutlineColor(sf::Color::White);

            float xPos = (WINDOW_WIDTH - item.shape.getSize().x) / 2;
            float yPos = 300 + i * 100;
            item.shape.setPosition(xPos, yPos);

            item.text.setFont(font);
            item.text.setString(itemTexts[i]);
            item.text.setCharacterSize(30);
            item.text.setFillColor(sf::Color::White);

            sf::FloatRect textRect = item.text.getLocalBounds();
            item.text.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            item.text.setPosition(xPos + item.shape.getSize().x / 2,
                yPos + item.shape.getSize().y / 2);

            item.isHovered = false;
            menuItems.push_back(item);
        }
    }
    else {
        // 主菜单
        std::vector<std::wstring> itemTexts = { L"开始游戏", L"设置", L"游戏规则", L"退出" };

        for (size_t i = 0; i < itemTexts.size(); ++i) {
            MenuItem item;
            item.shape.setSize(sf::Vector2f(300, 60));
            item.shape.setFillColor(sf::Color(70, 70, 70));
            item.shape.setOutlineThickness(2);
            item.shape.setOutlineColor(sf::Color::White);

            // 计算位置居中
            float xPos = (WINDOW_WIDTH - item.shape.getSize().x) / 2;
            float yPos = 300 + i * 100;
            item.shape.setPosition(xPos, yPos);

            // 设置菜单项文本（使用宽字符）
            item.text.setFont(font);
            item.text.setString(itemTexts[i]);
            item.text.setCharacterSize(30);
            item.text.setFillColor(sf::Color::White);

            // 文本居中
            sf::FloatRect textRect = item.text.getLocalBounds();
            item.text.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            item.text.setPosition(xPos + item.shape.getSize().x / 2,
                yPos + item.shape.getSize().y / 2);

            item.isHovered = false;
            menuItems.push_back(item);
        }
    }
}

void MenuState::handleEvent(const sf::Event& event, Reversi& game) {
    if (inRulesMenu) {
        // 鼠标滚轮滚动
        if (event.type == sf::Event::MouseWheelScrolled) {
            float maxOffset = std::max(0.f, rulesLines.size() * rulesLineHeight - (WINDOW_HEIGHT - 220));
            rulesScrollOffset -= event.mouseWheelScroll.delta * 40.0f;
            if (rulesScrollOffset < 0) rulesScrollOffset = 0;
            if (rulesScrollOffset > maxOffset) rulesScrollOffset = maxOffset;
            return;
        }
        // 返回按钮点击
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(game.getWindow());
            sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(pixelPos);
            if (backButtonShape.getGlobalBounds().contains(mousePos)) {
                game.getClickSound().play();
                inRulesMenu = false;
                initializeMenu();
            }
        }
        return; // 规则界面不处理其他菜单项
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(game.getWindow());
            sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(pixelPos);

            for (size_t i = 0; i < menuItems.size(); ++i) {
                if (menuItems[i].shape.getGlobalBounds().contains(
                    static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {

                    game.getClickSound().play();

                    if (inModeSelection) {
                        // 处理模式选择点击
                        switch (i) {
                        case 0: // 单人模式
                            game.setGameMode(GameMode::SinglePlayer);
                            game.initializeBoard();
                            game.setState(new PlayingState(game));
                            return;
                        case 1: // 双人模式
                            game.setGameMode(GameMode::TwoPlayer);
                            game.initializeBoard();
                            game.setState(new PlayingState(game));
                            return;
                        case 2: // 返回
                            initializeMenu(false, false, false, false);
                            break;
                        }
                    }
                    else if (inSettingsMenu) {
                        // 处理设置菜单点击
                        switch (i) {
                        case 0: {
                            // 获取当前难度
                            AIDifficulty currentDiff = aiPlayer.getDifficulty();
                            // 循环切换难度
                            switch (currentDiff) {
                            case AIDifficulty::Easy:
                                aiPlayer.setDifficulty(AIDifficulty::Medium);
                                break;
                            case AIDifficulty::Medium:
                                aiPlayer.setDifficulty(AIDifficulty::Hard);
                                break;
                            case AIDifficulty::Hard:
                                aiPlayer.setDifficulty(AIDifficulty::Easy);
                                break;
                            }
                            initializeMenu(false, true, false, false); // 刷新菜单显示
                            break;
                        }
                        case 1: // 循环切换棋盘颜色
                            if (game.getBoardColor() == BOARD_COLOR_GREEN) {
                                game.setBoardColor(BOARD_COLOR_YELLOW);
                            }
                            else if (game.getBoardColor() == BOARD_COLOR_YELLOW) {
                                game.setBoardColor(BOARD_COLOR_PURPLE);
                            }
                            else {
                                game.setBoardColor(BOARD_COLOR_GREEN);
                            }
                            initializeMenu(false, true, false, false); // 刷新菜单显示
                            break;
                        case 2: //亮度设置
                            game.setBrightnessStep((game.getBrightnessStep() + 1) % BRIGHTNESS_STEPS);
                            game.setBoardBrightness(BRIGHTNESS_MIN +
                                (BRIGHTNESS_MAX - BRIGHTNESS_MIN) * game.getBrightnessStep() / (BRIGHTNESS_STEPS - 1));
                            initializeMenu(false, true, false, false);  // 刷新菜单显示
                            break;
                        case 3: // 背景设置
                            inBackgroundSetting = true;
                            inSettingsMenu = false;
                            initializeMenu(false, false, false, true);
                            break;
                        case 4: // 返回
                            inSettingsMenu = false;
                            initializeMenu();
                            break;
                        }
                    }
                    else if (inBackgroundSetting) {
                        // 处理背景设置点击
                        switch (i) {
                        case 0: // 选择图片
                        {
                            std::string path = openFileDialog();
                            if (!path.empty()) {
                                game.loadBackgroundImage(path);
                                // 可以在这里保存路径到配置文件
                            }
                            break;
                        }
                        case 1: // 默认背景
                            game.loadBackgroundImage("");
                            break;
                        case 2: // 返回
                            inBackgroundSetting = false;
                            inSettingsMenu = true;
                            initializeMenu();
                            break;
                        }
                    }
                    else {
                        // 处理主菜单点击
                        switch (i) {
                        case 0: // 开始游戏
                            initializeMenu(true, false, false, false);
                            break;
                        case 1:  // 设置
                            initializeMenu(false, true, false, false);
                            break;
                        case 2: // 游戏规则 
                            initializeMenu(false, false, true, false);
                            break;
                        case 3: // 退出
                            game.getWindow().close();
                            break;
                        }
                    }
                }
            }
        }
    }
}

void MenuState::update(Reversi& game) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(pixelPos);

    if (inRulesMenu) {

        bool wasHovered = backButtonHovered;
        backButtonHovered = backButtonShape.getGlobalBounds().contains(mousePos);

        if (backButtonHovered != wasHovered) {
            backButtonShape.setFillColor(backButtonHovered ? sf::Color(100, 100, 100) : sf::Color(70, 70, 70));
            backButtonText.setFillColor(backButtonHovered ? sf::Color::Yellow : sf::Color::White);
        }
        return; // 规则界面不处理其他菜单项
    }

    for (auto& item : menuItems) {
        bool wasHovered = item.isHovered;
        item.isHovered = item.shape.getGlobalBounds().contains(
            static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (item.isHovered != wasHovered) {
            item.shape.setFillColor(item.isHovered ? sf::Color(100, 100, 100) : sf::Color(70, 70, 70));
            item.text.setFillColor(item.isHovered ? sf::Color::Yellow : sf::Color::White);
        }
    }
}

void MenuState::draw(sf::RenderWindow& window, Reversi& game) {
    // 绘制背景
    if (!game.getBackgroundImagePath().empty() || game.getBackgroundTexture().getSize().x > 0) {
        window.draw(game.getBackgroundSprite());
    }
    else {
        sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        background.setFillColor(BOARD_COLOR_GREEN);
        window.draw(background);
    }

    if (inRulesMenu) {
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150)); // 120为透明度，可根据需要调整
        window.draw(overlay);
    }

    // 绘制标题
    std::wstring titleText;
    if (inModeSelection) titleText = L"模式选择";
    else if (inSettingsMenu) titleText = L"设置";
    else if (inRulesMenu) titleText = L"游戏规则";
    else if (inBackgroundSetting) titleText = L"背景设置";
    else titleText = L"黑白棋";

    sf::Text title1(titleText, font, 60);
    sf::Text title2(titleText, font, 60);
    title1.setStyle(sf::Text::Bold);
    title2.setStyle(sf::Text::Bold);
    title1.setFillColor(sf::Color::Black);
    title2.setFillColor(sf::Color::White);
    sf::FloatRect titleRect = title1.getLocalBounds();
    title1.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title1.setPosition(WINDOW_WIDTH / 2.0f - 2, 100);
    titleRect = title2.getLocalBounds();
    title2.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    title2.setPosition(WINDOW_WIDTH / 2.0f + 2, 100);
    window.draw(title1);
    window.draw(title2);

    if (inRulesMenu) {

        float startY = 160 - rulesScrollOffset;
        for (size_t i = 0; i < rulesLines.size(); ++i) {
            sf::Text line(rulesLines[i], font, 35);
            line.setStyle(sf::Text::Bold);
            line.setFillColor(sf::Color::White);
            line.setPosition(150, startY + i * rulesLineHeight);
            // 只绘制在可视区域的行
            if (line.getPosition().y > 120 && line.getPosition().y < WINDOW_HEIGHT - 120)
                window.draw(line);
        }

        // 返回按钮
        window.draw(backButtonShape);
        window.draw(backButtonText);
        return;
    }
    else {
        // 绘制菜单项
        for (const auto& item : menuItems) {
            window.draw(item.shape);
            window.draw(item.text);
        }
    }
}

void MenuState::loadRulesFromFile(const std::wstring& filename) {
    rulesLines.clear();
    std::wifstream fin(filename.c_str());
    // 兼容 Windows 下 UTF-8 文件
    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));
    if (!fin) {
        // 读取失败，使用默认规则
        rulesLines.push_back(L"未找到规则文件，显示默认规则。");
        return;
    }
    std::wstring line;
    while (std::getline(fin, line)) {
        rulesLines.push_back(line);
    }
}
