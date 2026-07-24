#include <SFML/Graphics.hpp>
#include "Reversi.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "PlayingState.h"
int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), L"黑白棋", sf::Style::Close | sf::Style::Resize);
    Reversi game(window);
    game.setState(new OpenState(game));

    sf::Music music;
    if (!music.openFromFile("backguandmusic.mp3"))
    {
        return -1;
    }
    music.setLoop(true);
    music.play();

    // 创建视图并保持原始宽高比
    sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view);

    while (window.isOpen()) {
        sf::Event event;
        // 处理窗口事件的主循环
        while (window.pollEvent(event)) {
            // 检查是否收到窗口关闭事件（用户点击了窗口的关闭按钮）
            if (event.type == sf::Event::Closed) {
                music.stop();       // 停止背景音乐播放
                window.close();     // 关闭游戏窗口
                
            }

            // 检查是否收到键盘按键按下事件
            if (event.type == sf::Event::KeyPressed) {
                // 根据按下的不同按键执行相应操作
                switch (event.key.code)
                {
                case sf::Keyboard::Add:     // 如果按下的是加号键（+）
                    // 增大音量（当前音量+10，但不超过最大值100）
                    music.setVolume(std::min(music.getVolume() + 10.0f, 100.0f));
                    break;
                case sf::Keyboard::Subtract: // 如果按下的是减号键（-）
                    // 减小音量（当前音量-10，但不低于最小值0）
                    music.setVolume(std::max(music.getVolume() - 10.0f, 0.0f));
                    break;
                }
            }

            if (game.currentState)
                game.currentState->handleEvent(event, game);

            // 将当前事件传递给游戏主逻辑进行更新处理
            // 这个方法会根据游戏当前状态（菜单、游戏中等）调用相应的状态处理函数
            
        }

        if (game.currentState)
            game.currentState->update(game);
        window.clear();
        if (game.currentState)
            game.currentState->draw(window, game);
        window.display();
    }
    if (game.currentState) delete game.currentState;
    return 0;
}