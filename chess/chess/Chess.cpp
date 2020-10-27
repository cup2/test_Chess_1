#include "Chess.h"
#include <iostream>
#include <vector>

int Chess::checkMove(const sf::Vector2f& oldPos, const sf::Vector2f& newPos, bool isWhite) {
	COLOR_CHESS curentColor = (isWhite) ? WHITE : BLACK;
	int type = (mathboard[(int)oldPos.y][(int)oldPos.x] & maskTypeFigure);
	int result = 0;
	switch (type) {
	case PAWN:
		result = checkPawn(oldPos, newPos, curentColor);
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		if (result && (newPos.y == 0 || newPos.y == 7)) {
			mathboard[(int)newPos.y][(int)newPos.x] = QUIN | curentColor;
			drawMatrix();
			return 10;
		}
		break;
	case KNIGHT:
		result = checkKnight(oldPos, newPos, curentColor);
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		break;
	case ROOK:
		result = checkRook(oldPos, newPos, curentColor);
		if (result != 1 && result != 0) {
			return result;
		}
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		break;
	case BISHOP:
		result = checkBishop(oldPos, newPos, curentColor);
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		break;
	case QUIN:
		result = checkQuin(oldPos, newPos, curentColor);
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		break;
	case KING:
		result = checkKing(oldPos, newPos, curentColor);
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		break;
	default:
		break;
	}
	//int checkAponent;
	//COLOR_CHESS aponentColor = (!isWhite) ? WHITE : BLACK; //
	//checkAponent = checkCheck(aponentColor);
	return result;
};

void Chess::drawMatrix() {
	std::cout << std::endl << "MotherBoard:" << std::endl;
	for (int i = 0; i < 8; i++)
	{
		std::cout << std::endl;
		for (int j = 0; j < 8; j++)
		{
			std::cout << mathboard[i][j] << " ";
		}
	}
}
int Chess::checkCheckMate(const int& color) {
	int result = checkCheck(color);
	sf::Vector2f coordKing;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (mathboard[i][j] == (KING | color)) {
				coordKing.y = i;
				coordKing.x = j;
			}
		}
	}
	if (result) {
		int mat = false;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; i++) {
				if (i >= 0 && i < 8 && j >= 0 && j < 8) {
					sf::Vector2f coo = { (float)j,(float)i };
					checkCheck(color, coo);
				}

			}
		}
	}
}


int Chess::setNewPosition(const sf::Vector2f& oldPos, const sf::Vector2f& newPos,const int& color, const int& type) {
	mathboard[(int)oldPos.y][(int)oldPos.x] = 0;
	mathboard[(int)newPos.y][(int)newPos.x] = type | color;
	if (checkCheck(color)) {
		mathboard[(int)oldPos.y][(int)oldPos.x] = type | color;;
		mathboard[(int)newPos.y][(int)newPos.x] = 0;
		return 0;
	}
	return 1;
};



bool Chess::checkKnightIsAttack(const sf::Vector2f& coordKing, const int& color) {
	std::vector<Vec> posible = { {2,1} ,{2,-1},{1,2},{1,-2},{-1,2}, {-1,-2},{-2,1},{-2,-1} };
	for (auto it = posible.begin(); it < posible.end(); ++it) {
		int x = coordKing.x + it->x;
		int y = coordKing.y + it->y;
		if (x >= 0 && x < 8 && y >= 0 && y < 8) {
			if (((mathboard[y][x ]& maskTypeFigure)==KNIGHT) &&((mathboard[y][x] & maskColorFigure) != color)) {
				return 1;
			}		
		}
	}
	return 0;
}



bool Chess::checkIsAtackKing(const int& kingColor, const  Vec& attack, const Vec& needAtack, const sf::Vector2f& coord) {
	bool kolonialnostVector = (((attack.x * needAtack.y) == ((attack.y * needAtack.x))))? true : false;
	if (kolonialnostVector) {
		return checkIsFigureAfter(coord, attack);
	}
	else {
		return false;
	}
}



int checkPawnIsAtackK(sf::Vector2f coordKing, int color) {
	//int k = ((color == WHITE) ? 1 : -1);
	//if ((oldCoord.x == newCoord.x - k) && (oldCoord.y == (newCoord.y + k))) {
	//	if (((mathboard[(int)newCoord.y][(int)newCoord.x] & aponentcolor))) {
	//		return 1;
	//	}
	//	else {
	//		return 0;
	//	}
	//}
	//else {
	//	return 0;
	//}
};



int Chess::checkCheck(const int& color, sf::Vector2f coordKing) {
	int isCheck = false;
	if (coordKing.x == -1) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (mathboard[i][j] == (KING | color)) {
					coordKing.y = i;
					coordKing.x = j;
				}
			}
		}
	}
	for (float y = 0; y < 8; y++) {
		for (float x = 0; x < 8; x++) {
			int figuraType = mathboard[(int)y][(int)x] & 7;
			if (figuraType == 0) {
				continue;
			}
			Vec attac = { x-coordKing.x,y-coordKing.y};
			if ((figuraType == ROOK) && ((mathboard[(int)y][(int)x] & color) == 0)) {
				isCheck = isCheck || checkIsAtackKing(color, attac , { 0, 1 },{ x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, { 0,-1}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, { 1, 0}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {-1, 0}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, { 0,-1 }, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, { 0,-1 }, { x,y });
			}
			if (figuraType == BISHOP && ((mathboard[(int)y][(int)x] & color) == 0)) {
				isCheck = isCheck || checkIsAtackKing(color, attac, { 1,1 }, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, { 1,-1}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, { -1,1 }, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, { -1,-1 }, { x,y });
			}
			if (figuraType == QUIN && ((mathboard[(int)y][(int)x] & color) == 0)) {
				isCheck = isCheck || checkIsAtackKing(color, attac, { 0, 1 }, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {0,-1}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {1,0}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {-1,0}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {1,1}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {1,-1}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {-1,1}, { x,y });
				isCheck = isCheck || checkIsAtackKing(color, attac, {-1,-1}, { x,y });
			}
			if (figuraType == KNIGHT && ((mathboard[(int)y][(int)x] & color) == 0)) {
				isCheck = isCheck || checkKnightIsAttack(coordKing, color);
			}
		}
	}
	return isCheck;	
}
	

int Chess::checkIsAvalable(const sf::Vector2f& newCoord, const int& color) {
	// 1 empty // 2 enemy // 0 own
	COLOR_CHESS aponentcolor = (color & WHITE) ? BLACK : WHITE;
	if ((mathboard[(int)newCoord.y][(int)newCoord.x] & (BLACK | WHITE)) == color) {
		return 0;
	}
	else if ((mathboard[(int)newCoord.y][(int)newCoord.x] & (BLACK | WHITE)) == aponentcolor) {
		return 1;
	}
	else {
		return 2;
	}
}

int Chess::checkIsFigureAfter(const sf::Vector2f& oldCoord, const Vec& move) {
	int x = (move.x == 0) ? 0 : (move.x > 0 ? move.x - 1 : move.x + 1);
	int y = (move.y == 0) ? 0 : (move.y > 0 ? move.y - 1 : move.y + 1);

	Vec newMove = { x,y };
	if (newMove.x == 0 && newMove.y == 0) {
		return 1;
	}
	if ((mathboard[(int)oldCoord.y - newMove.y][(int)oldCoord.x - newMove.x] & 7) != 0) {
		return 0;
	}
	else {
		return  checkIsFigureAfter(oldCoord, newMove);
	}
}




int Chess::checkPawn(const sf::Vector2f& oldCoord,const sf::Vector2f& newCoord, const COLOR_CHESS& color) {
	int k = ((color == WHITE) ? 1: -1);
	COLOR_CHESS aponentcolor = (color & WHITE) ? BLACK : WHITE;
	if ((oldCoord.y == 1 || oldCoord.y ==6) && (oldCoord.x == newCoord.x) && (oldCoord.y == (newCoord.y + k*2)) && mathboard[(int)newCoord.y][(int)newCoord.x] == 0) {
		return 1;
	}
	if ((oldCoord.x == newCoord.x) && (oldCoord.y == (newCoord.y + k)) && mathboard[(int)newCoord.y][(int)newCoord.x] == 0) {
		return 1;
	}
	else if ((oldCoord.x == newCoord.x + k) && (oldCoord.y == (newCoord.y + k))){
		if (((mathboard[(int)newCoord.y][(int)newCoord.x] & aponentcolor))) {
			return 1;
		} 
		else {
			return 0;
		}
	}
	else if ((oldCoord.x == newCoord.x - k) && (oldCoord.y == (newCoord.y + k))) {
		if (((mathboard[(int)newCoord.y][(int)newCoord.x] & aponentcolor))) {
			return 1;

		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int Chess::checkKnight(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color) {
	COLOR_CHESS aponentcolor = (color & WHITE) ? BLACK : WHITE;
	std::vector<Vec> posible = { {2,1} ,{2,-1},{1,2},{1,-2},{-1,2}, {-1,-2},{-2,1},{-2,-1} };
	Vec curent = { oldCoord.x - newCoord.x, oldCoord.y - newCoord.y };
	int checkNew = checkIsAvalable(newCoord, color);
	if (checkNew ==0) {
		return 0;
	}

	for (auto it = posible.begin(); it < posible.end(); ++it) {
		if (it->x == curent.x && it->y == curent.y) {
			return 1;
		}
	}
	return 0;
}

int Chess::doCastling(const int& type, const COLOR_CHESS& color) {
	if (type == 1 && (color == WHITE)) {
		mathboard[7][4] = 0;
		mathboard[7][6] = KING | color;
		mathboard[7][5] = ROOK | color;
		isBeRokW = true;
		drawMatrix();
		return 2;
	}
	if (type == 1 && (color == BLACK)) {
		mathboard[0][4] = 0;
		mathboard[0][6] = KING | color;
		mathboard[0][5] = ROOK | color;
		isBeRokB = true;
		drawMatrix();
		return 3;

	}
	if (type == 0 && (color == BLACK)) {
		mathboard[0][4] = 0;
		mathboard[0][2] = KING | color;
		mathboard[0][3] = ROOK | color;
		isBeRokB = true;
		drawMatrix();
		return 4;
	}
	if (type == 0 && (color == WHITE)) {
		isBeRokW = true;
		mathboard[7][4] = 0;
		mathboard[7][2] = KING | color;
		mathboard[7][3] = ROOK | color;
		isBeRokW = true;
		return 5;
	}


}

int Chess::checkRook(sf::Vector2f oldCoord, sf::Vector2f newCoord, COLOR_CHESS color) {
	Vec curent = { oldCoord.x - newCoord.x, oldCoord.y - newCoord.y };
	int checkNew = checkIsAvalable(newCoord, color);
	bool rok = false;
	if (checkNew == 0) {
		if ((mathboard[(int)newCoord.y][(int)newCoord.x] & maskTypeFigure) == KING) {
			if (color == WHITE) {
				if (isBeRokW) {
					return 0;
				}
				else {
					rok = true;
				}
			}	
			if (color == BLACK) {
				if (isBeRokB) {
					return 0;
				}
				else {
					rok = true;
				}
			}
		}
		else {
			return 0;
		}
	}	
	
	if (rok) {
		switch ((int)oldCoord.x)
		{
		case 0:
			if (checkIsFigureAfter(oldCoord, curent)) {
				mathboard[(int)oldCoord.y][(int)oldCoord.x] = 0;
				return doCastling(0, color);				 
			}
			break;
		case 7:
			if (checkIsFigureAfter(oldCoord, curent)) {
				mathboard[(int)oldCoord.y][(int)oldCoord.x] = 0;
				return doCastling(1, color);
			}
			break;
		default:
			break;
		}				
	}

	if ((curent.x == 0) || (curent.y == 0)) {
		if (checkIsFigureAfter(oldCoord,curent)) {
			isBeRokW = true;
			return 1;
		}
	}
	return 0;
}

int Chess::checkBishop(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color) {
	Vec curent = { oldCoord.x - newCoord.x, oldCoord.y - newCoord.y };
	int checkNew = checkIsAvalable(newCoord, color);
	if (checkNew == 0) {
		return 0;
	}
	if ((abs(curent.x) == abs(curent.y) )) {
			return 1;
	}	
}

int Chess::checkQuin(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color) {
	Vec curent = { oldCoord.x - newCoord.x, oldCoord.y - newCoord.y };
	int checkNew = checkIsAvalable(newCoord, color);
	if (checkNew == 0) {
		return 0;
	}
	if ((abs(curent.x) == abs(curent.y))) {
		if (checkIsFigureAfter(oldCoord, curent)) {
			return 1;
		}
	}

	if ((curent.x == 0) || (curent.y == 0)) {
		if (checkIsFigureAfter(oldCoord, curent)) {
			return 1;
		}
	}
	return 0;
}

int Chess::checkKing(const sf::Vector2f& oldCoord, const sf::Vector2f& newCoord, const int& color) {
	Vec curent = { oldCoord.x - newCoord.x, oldCoord.y - newCoord.y };
	int checkNew = checkIsAvalable(newCoord, color);
	if (checkNew == 0) {
		return 0;
	}
	if ((abs(curent.x) < 2 && abs(curent.y) < 2)) {
		return 1;
	}
}