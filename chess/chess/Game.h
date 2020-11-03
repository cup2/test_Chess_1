#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Chess.h"

#include "TextOut.h"


extern sf::Vector2f offset;

class Game 
{
public:
	Game();
	int execute();
	void init();
	void exit();
private:
	sf::RenderWindow window;

	sf::Sprite menu1, menu2, menu3;
	sf::Texture menuTexture1, menuTexture2, menuTexture3;
	sf::Sound sound;
	sf::Texture t1;
	sf::Texture t2;
	sf::Texture t3;
	sf::Sprite board;

	sf::Sprite chessFigure[32];
	sf::Sprite chessFigureChoose[4];
	sf::SoundBuffer buffer;

	bool isRun = true;
	int statusCode = 0;
	Chess *round = new Chess();
	TextOut alert;
	bool isWhiteMove = true;
	const int initBoard[8][8] = {
	 -1,-2,-3,-4,-5,-3,-2,-1,
	 -6,-6,-6,-6,-6,-6,-6,-6,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  6, 6, 6, 6, 6, 6, 6, 6,
	  1, 2, 3, 4, 5, 3, 2, 1
	};


	sf::Vector2f offset;

	const int size = 56;
	void loadPosition();
	int drawChangePawnFigureChoose(const int& selectedFigure);
	int makeMove(const sf::Vector2f& oldPos, sf::Vector2f& newPos, const int& selectedFigure);
	int drawCastling(const int& type);
	sf::Vector2f toCoord8x8(sf::Vector2f pos);
	std::string toChessNote(sf::Vector2f p);
	sf::Vector2f toCoord(char a, char b);

};

