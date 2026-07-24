#include "OpenState.h"
#include "Reversi.h"
#include "MenuState.h"
#include <string>
#include <iostream>

OpenState::OpenState(Reversi& game) : game(game), scrollPosition(0), normalScrollSpeed(50.0f), scrollSpeed(50.0f), fastScrollSpeed(200.0f) {

    if (!openingBgTexture.loadFromFile("open.png")) {
        std::wcerr << L"无法打开背景图片！" << std::endl;
    }
    openingBgSprite.setTexture(openingBgTexture);

    float scaleX = float(WINDOW_WIDTH) / openingBgTexture.getSize().x;
    float scaleY = float(WINDOW_HEIGHT) / openingBgTexture.getSize().y;
    openingBgSprite.setScale(scaleX, scaleY);

    // 尝试加载字体文件
    if (!font.loadFromFile("C:/Windows/Fonts/simsun.ttc")) {
        std::wcerr << L"无法加载该字体! 将尝试使用其他字体。" << std::endl;
    }

    // 初始化开场动画文本
    openingTexts = {
        L"《黑白弈经》",
        L" ",
        L"夫弈者 阴阳之道也。",
        L"楸枰列阵 玄素争锋",
        L"始如星罗 终似云合",
        L"黑子如墨龙潜渊 白子若雪鹤翔空",
        L"一着落定 乾坤倒旋",
        L" ",
        L"初 四子镇中 八荒暗伏 ",
        L"黑先白应 各逞机锋",
        L"或围魏以救赵 或暗渡而陈仓 ",
        L"子落处 敌势溃散如秋叶",
        L"局变时 我形骤聚似春潮",
        L"翻子之声 泠泠若涧泉击石",
        L"收官之际 寂寂如老僧参禅",
        L" ",
        L"至若胜负分明 满盘皆化太极图",
        L"黑中有白 白中藏黑",
        L"恰似世事无常 盈亏相生",
        L"故曰：弈棋非争得失",
        L"实乃观心照影 得天地圆融之趣也",
        L" ",
        L"                     —— 录自《弈林野叟手谈笔记》",
    };
}

// 处理OpenState（开场状态）的事件
void OpenState::handleEvent(const sf::Event& event, Reversi& game) {
    // 检查事件类型是否为鼠标按键按下
    if (event.type == sf::Event::MouseButtonPressed) {
        // 检查按下的鼠标按键是否为左键
        if (event.mouseButton.button == sf::Mouse::Left) {
            // 播放点击音效
            game.getClickSound().play();
            // 将游戏状态切换到菜单状态
            game.setState(new MenuState(game)); 
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        scrollSpeed = fastScrollSpeed;
    }
    else {
        scrollSpeed = normalScrollSpeed;
    }
}

void OpenState::update(Reversi& game) {
    float deltaTime = animationClock.restart().asSeconds();
    scrollPosition += scrollSpeed * deltaTime;

    float totalHeight = 0;
    for (const auto& text : openingTexts) {
        sf::Text tempText(text, font, 40);
        totalHeight += tempText.getLocalBounds().height + 100;
    }

    if (scrollPosition > totalHeight + WINDOW_HEIGHT) {
        scrollPosition = 0;
    }
}

// 添加绘制开场动画的方法
void OpenState::draw(sf::RenderWindow& window, Reversi& game) {
    // 绘制背景图片  
    window.draw(openingBgSprite);

    //添加半透明遮罩使文字更清晰
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // 半透明黑色
    window.draw(overlay);

    // 更新滚动位置
    float deltaTime = animationClock.restart().asSeconds();
    scrollPosition += scrollSpeed * deltaTime;

    // 计算总文本高度
    float totalHeight = 0;
    for (const auto& text : openingTexts) {
        sf::Text tempText(text, font, 40);
        totalHeight += tempText.getLocalBounds().height + 100;
    }

    // 重置滚动位置当所有文本都滚出屏幕
    if (scrollPosition > totalHeight + WINDOW_HEIGHT) {
        scrollPosition = 0;
    }

    // 绘制所有文本
    float yPos = WINDOW_HEIGHT - scrollPosition;
    for (const auto& text : openingTexts) {
        sf::Text textElement(text, font, 40);
        textElement.setFillColor(sf::Color::White);
        textElement.setStyle(sf::Text::Bold);

        sf::FloatRect textRect = textElement.getLocalBounds();
        textElement.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        textElement.setPosition(WINDOW_WIDTH / 2, yPos);

        // 只绘制在屏幕内的文本
        if (yPos + textRect.height / 2 > 0 && yPos - textRect.height / 2 < WINDOW_HEIGHT) {
            window.draw(textElement);
        }

        yPos += textRect.height + 70;
    }

	std::wstring callText1 = L"按空格或下键加速";
	std::wstring callText2 = L"点击任意位置跳过";

    // 绘制提示文字
    sf::Text hintText1(callText1, font, 30);
    hintText1.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hintRect = hintText1.getLocalBounds();
    hintText1.setOrigin(hintRect.left + hintRect.width / 2.0f,
        hintRect.top + hintRect.height / 2.0f);
    hintText1.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100);
    window.draw(hintText1);

    sf::Text hintText2(callText2, font, 30);
    hintText2.setFillColor(sf::Color(150, 150, 150));
    hintRect = hintText2.getLocalBounds();
    hintText2.setOrigin(hintRect.left + hintRect.width / 2.0f,
        hintRect.top + hintRect.height / 2.0f);
    hintText2.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
    window.draw(hintText2);
}
