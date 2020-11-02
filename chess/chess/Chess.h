#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


struct Vec {
	int x;
	int y;
};

enum TYPE_FIGURE {
	KING = 1, // wk9, 17
	QUIN = 2, // wq10, 18
	BISHOP = 3,//11 , 19
	KNIGHT = 4,//12, 20
	ROOK = 5, // 13, 21
	PAWN = 6//14 , 22
};

enum STATUS_CODE {
	OK = 1,
	CHECK_CURENT,
	CHECK_ENEMY,//11 , 19
	CHANGE_PAWN,//12, 20
	DOCASTLING, // 13, 21
	CHECKMATE,
	PAT
};

enum COLOR_CHESS {
	WHITE = 8,
	BLACK = 16
};

enum STATE_FIGURE {
	CHECK = 32,
	CASTLING = 64
};

class Chess
{
public:
	Chess() {};
	int getTypeCatling() {
		return typeCastling;
	}
	bool getIsWhaiteNewFigure() {
		return waitNewPawn;
	}
	bool getIsCheck() {
		return isCheck;
	}
	int changePawn(int typefigure, const sf::Vector2f& pos);
	int checkMove(const sf::Vector2f& oldPos,const sf::Vector2f& newPos, bool isWhite);
	int mathboard[8][8] =
	{
	 21, 20, 19, 18, 17, 19, 20, 21,
	 22,22, 22,22,22,22,22,22,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 14, 14, 14, 14, 14, 14, 14, 14,
	 13,12,11,10,9,11,12,13,

	};
	void drawMatrix();
private:
	std::vector<Vec> moveBishopQuin = { {1,1}, {1,-1},{-1,-1},{-1,1} };
	std::vector<Vec> moveRookQuin = { {0,1}, {0,-1},{1,0},{-1,0} };
	std::vector<Vec> moveKnight = { {1,2}, {1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
	int typeCastling = 0;

	bool isCheck = false;
	bool isBeRokB = false;
	bool isBeRokW = false;
	bool waitNewPawn = false;
	const int maskTypeFigure = KING | QUIN | PAWN | ROOK | KNIGHT | BISHOP;
	const int maskColorFigure = WHITE | BLACK;


	bool checkPawnIsAttack(const sf::Vector2f& coordKing, const int& color);
	int checkAvalebleMove(const COLOR_CHESS& color);
	int checkCheckMate(const COLOR_CHESS& color);
	bool checkKnightIsAttack(const sf::Vector2f& coordKing, const int& color);
	int checkCheck(const int& color,  sf::Vector2f coordKing = { -1,-1 });
	int checkIsAtackKing(const int& kingColor, const Vec& attack, const Vec& needAtack, const sf::Vector2f& coord);
	int doCastling(const int& type, const COLOR_CHESS&); // 0 short // 1 long
	int setNewPosition(const sf::Vector2f& oldPos, const sf::Vector2f& newPos, const int& color, const int& type);

	int checkIsAvalable(const sf::Vector2f& newCoord, const int& color);
	int checkIsFigureAfter(const sf::Vector2f& oldCoord, const Vec& move);
	int haveMove( Vec move, const sf::Vector2f& coord, const int color,const bool reapeted);
	int checkKing(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color);
	int checkQuin(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color);
	int checkBishop(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color);
	int checkKnight(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color);
	int checkRook(const sf::Vector2f oldCoord, const sf::Vector2f newCoord, const COLOR_CHESS color);
	int checkPawn(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const COLOR_CHESS& color);
};

