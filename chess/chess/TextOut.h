#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class TextOut {
public:
	TextOut() {};
	void set(float X, float Y, sf::Font Font) {
		sf::Text Text(stringBadMoveCheck, font, 20);
		x = X; y = Y;
		font = Font;
		text = Text;	
	}
	TextOut(float X, float Y, sf::Font Font, float TimeStart);
	void drawMessage(int code, sf::RenderWindow& window);
private:	
	void update(sf::RenderWindow& window, const std::string message);
	float x, y;
	sf::Font font;
	sf::Text text;
	std::string stringWhire = "White move";
	std::string stringBlack = "Black move";
	std::string stringBadMoveFigure = "Invalid move";
	std::string stringBadMoveCheck = "Invalid move, you have check";
	std::string stringCheck = "Check";
	std::string stringCheckMate = "CheckMate, press ESC and start new game";
	std::string stringPat= "Draw";

};
