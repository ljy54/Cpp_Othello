#pragma once
#include "State.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Reversi.h"
class Reversi;

class MenuState : public State {
private:
	sf::Font font;
	std::vector<MenuItem> menuItems;
	bool inModeSelection = false;
	bool inSettingsMenu = false;
	bool inRulesMenu = false;
	bool inBackgroundSetting = false;
	Reversi& game;
	AIPlayer& aiPlayer;
	std::vector<std::wstring> rulesLines;
	float rulesScrollOffset = 0.0f;       // 当前滚动偏移
	float rulesLineHeight = 50.0f;        // 每行高度
	sf::RectangleShape backButtonShape;
	sf::Text backButtonText;

public:
	MenuState(Reversi& game);
	void initializeMenu(bool inModeSelection = false, bool inSettingsMenu = false, bool inRulesMenu = false, bool inBackgroundSetting = false);
	void handleEvent(const sf::Event& event, Reversi& game) override;
	void update(Reversi& game) override;
	void draw(sf::RenderWindow& window, Reversi& game) override;
	bool backButtonHovered = false;
	void loadRulesFromFile(const std::wstring& filename);
};
