#include "TextOut.h"

TextOut::TextOut(float X, float Y, sf::Font Font, float TimeStart) {
	sf::Text Text(stringBadMoveCheck, font, 20);
	x = X; y = Y;
	font = Font;
	text = Text;
}


void TextOut::drawMessage(int code, sf::RenderWindow& window) {
	switch (code) {
	case 1:
		update(window, stringWhire);
		break;
	case 2:
		update(window, stringBlack);
		break;
	case 3:
		update(window, stringBadMoveFigure);
		break;
	case 4:
		update(window, stringCheck);
		break;
	case 5:
		update(window, stringCheckMate);
		break;
	default:
		update(window, stringWhire);

		break;
	}
}
void TextOut::update(sf::RenderWindow& window, const std::string message){
	text.setString(message);
	text.setPosition(x, y);
	window.draw(text);	
}