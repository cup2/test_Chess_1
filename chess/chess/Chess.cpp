#include "Chess.h"
#include <iostream>
#include <vector>
int Chess::changePawn(int typefigure, const sf::Vector2f& pos) {
	int type = typefigure == 0 ? ROOK  : typefigure == 1 ? KNIGHT : typefigure == 2 ?  BISHOP : QUIN;
	int color = mathboard[(int)pos.y][(int)pos.x] & maskColorFigure;
	if (waitNewPawn) {
		mathboard[(int)pos.y][(int)pos.x] = type | color;
		waitNewPawn = false;
		return 0;
	}
	return 1;
}

bool Chess::checkPawnIsAttack(const sf::Vector2f& coordKing, const int& color){
	int k = ((color == WHITE) ? -1 : 1);
	int x = coordKing.x + k;
	int y = coordKing.y + k;
	if (((mathboard[y][x] & maskTypeFigure) == PAWN) && ((mathboard[y][x] & maskColorFigure) != color)) {
		return true;
	}
	 x = coordKing.x - k;
	 y = coordKing.y + k;
	if (((mathboard[y][x] & maskTypeFigure) == PAWN) && ((mathboard[y][x] & maskColorFigure) != color)) {
		return true;
	}
	return false;
}


int Chess::checkMove(const sf::Vector2f& oldPos, const sf::Vector2f& newPos, bool isWhite) {
	COLOR_CHESS curentColor = (isWhite) ? WHITE : BLACK;
	if (checkCheckMate(curentColor)) {
		return PAT;
	};
	int type = (mathboard[(int)oldPos.y][(int)oldPos.x] & maskTypeFigure);
	int result = 0;
	switch (type) {
	case PAWN:
		result = checkPawn(oldPos, newPos, curentColor);
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		if (result && (newPos.y == 0 || newPos.y == 7)) {
			waitNewPawn = true;
			return CHANGE_PAWN;
		} 
		break;
	case KNIGHT:
		result = checkKnight(oldPos, newPos, curentColor);
		result = result && setNewPosition(oldPos, newPos, curentColor, type);
		break;
	case ROOK:
		result = checkRook(oldPos, newPos, curentColor);
		if (result == 2) {
			return DOCASTLING;
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
	COLOR_CHESS aponentColor = (!isWhite) ? WHITE : BLACK; 
	isCheck = checkCheck(aponentColor);
	if (isCheck && checkCheckMate(aponentColor)) {
		return CHECKMATE;
	};
	return result;
};

void Chess::drawMatrix() {
	for (int i = 0; i < 8; i++)
	{
		std::cout << std::endl;
		for (int j = 0; j < 8; j++)
		{
			std::cout << mathboard[i][j] << " ";
		}
	}
}

int Chess::haveMove(Vec move, const sf::Vector2f& coord, const int color,const bool reapeted) {
	int old = mathboard[(int)coord.y][(int)coord.x];
	mathboard[(int)coord.y][(int)coord.x] = 0;
	int check = false;
	Vec newCoord = { coord.x + move.x, coord.y + move.y };

	for (int i = 0; i < 8; i++) {
		if (newCoord.x < 8 && newCoord.x>0 && newCoord.y > 0 && newCoord.y < 8) {
			if (((mathboard[newCoord.y][newCoord.x] & maskColorFigure) != color) || (mathboard[newCoord.y][newCoord.x] == 0)) {
				int ol = mathboard[newCoord.y][newCoord.x];
				mathboard[newCoord.y][newCoord.x] = 4;
				check = checkCheck(color);
				mathboard[newCoord.y][newCoord.x] = ol;
				if (!check) {
					mathboard[(int)coord.y][(int)coord.x] = old;
					return 1;
				}
			}
			if (!reapeted) {
				break;
			}
			else if ((mathboard[newCoord.y][newCoord.x] && maskTypeFigure) != 0) {
				break;
			}
			newCoord.x += move.x;
			newCoord.y += move.y;
		}
	}
	mathboard[(int)coord.y][(int)coord.x] = old;
	return 0;
}

int Chess::checkAvalebleMove(const COLOR_CHESS& color) {
	for (float y = 0; y < 8; y++) {
		for (float x = 0; x < 8; x++) {
			int figuraType = mathboard[(int)y][(int)x] & 7;
			if (figuraType == 0) {
				continue;			
			}

			if (((figuraType == PAWN) && ((mathboard[(int)y][(int)x] & color) == color))) {
				int k = ((color == WHITE) ? 1 : -1);
				if (checkPawn({ x,y }, { x + k,y - k }, color)) {
					if (haveMove({ k,-k }, { x, y }, color, false))	return 1;
				}
				if (checkPawn({ x,y }, { x - k,y - k }, color)) {
					if (haveMove({ -k,-k }, { x, y }, color, false))	return 1;
				}
				if (checkPawn({ x,y }, { x, y -k }, color)) {
					if (haveMove({ 0,-k }, { x, y }, color, false))	return 1;
				}				
			}	

			if ((((figuraType == ROOK) || (figuraType == QUIN)) && ((mathboard[(int)y][(int)x] & color) == color)) ) {
				for (auto it = moveRookQuin.begin(); it < moveBishopQuin.end(); ++it) {
					if (haveMove(*it, { x, y }, color, true)){
						return 1;
					}
				}
			}
			if (((figuraType == BISHOP) ||(figuraType == QUIN)) && ((mathboard[(int)y][(int)x] & color) == color)) {
				for (auto it = moveBishopQuin.begin(); it < moveBishopQuin.end(); ++it) {
					if (haveMove(*it, { x, y }, color, true)) {
						return 1;
					}
				}
			}
			if ((figuraType == KNIGHT ) && ((mathboard[(int)y][(int)x] & color) == color)) {
				
				for (auto it = moveKnight.begin(); it < moveKnight.end(); ++it) {
					if (haveMove(*it, { x, y }, color, false)) { 
						return 1; 
					}
				}
			}
		}
	}
	return 0;
}


int Chess::checkCheckMate(const COLOR_CHESS& color) {
	int result = 1;
	sf::Vector2f coordKing;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (mathboard[i][j] == (KING | color)) {
				coordKing.y = i;
				coordKing.x = j;
				break;
			}
		}
	}	
	if (result) {
		int mat = false;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				Vec newCoord = { coordKing.x + j, coordKing.y  + i };
				if (newCoord.x >= 0 && newCoord.x < 8 && newCoord.y >= 0 && newCoord.y < 8) {
					sf::Vector2f coo = { (float)newCoord.x,(float)newCoord.y };
					int test = checkCheck(color, coo);
					if (test==0) {
						return 0;
					}
				}

			}
		}
	}

	int test = checkAvalebleMove(color);
	if (test == 1) {
		return 0;
	}
	return 1;
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
	for (auto it = moveKnight.begin(); it < moveKnight.end(); ++it) {
		int x = coordKing.x + it->x;
		int y = coordKing.y + it->y;
		if (x >= 0 && x < 8 && y >= 0 && y < 8) {
			if (((mathboard[y][x] & maskTypeFigure) == KNIGHT) && ((mathboard[y][x] & maskColorFigure) != color)) {
				return 1;
			}
		}
	}
	return 0;
}

int Chess::checkIsAtackKing(const int& kingColor, const  Vec& attack, const Vec& needAtack, const sf::Vector2f& coord) {
	bool kolonialnostVector = (((attack.x * needAtack.y) == ((attack.y * needAtack.x))))? true : false;
	if (kolonialnostVector) {
		return checkIsFigureAfter(coord, attack);
	}
	else {
		return 0;
	}
}

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
	else {
		if ((mathboard[(int)coordKing.y][(int)coordKing.x] != 0 ) && ((mathboard[(int)coordKing.y][(int)coordKing.x] & maskColorFigure) == color)) {
			return 1;
		}	
	}

	for (float y = 0; y < 8; y++) {
		for (float x = 0; x < 8; x++) {
			int figuraType = mathboard[(int)y][(int)x] & 7;
			if (figuraType == 0) {
				continue;
			}
			Vec attac = { x-coordKing.x,y-coordKing.y};
			if (((figuraType == ROOK) || (figuraType == QUIN)) && ((mathboard[(int)y][(int)x] & color) == 0)) {
				for (auto it = moveRookQuin.begin(); it < moveRookQuin.end(); ++it) {
					isCheck = isCheck || checkIsAtackKing(color, attac,*it, { x,y });
				}
			}
			if (((figuraType == BISHOP)|| (figuraType == QUIN)) && ((mathboard[(int)y][(int)x] & color) == 0)) {
				for (auto it = moveBishopQuin.begin(); it < moveBishopQuin.end(); ++it) {
					isCheck = isCheck || checkIsAtackKing(color, attac, *it, { x,y });
				}
			}
			if (figuraType == KNIGHT && ((mathboard[(int)y][(int)x] & color) == 0)) {
				isCheck = isCheck || checkKnightIsAttack(coordKing, color);
			}
			if (figuraType == PAWN && ((mathboard[(int)y][(int)x] & color) == 0)) {
				isCheck = isCheck || checkPawnIsAttack(coordKing, color);
				
			}
		}
	}
	return isCheck;	
}
	

int Chess::checkIsAvalable(const sf::Vector2f& newCoord, const int& color) {
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
		typeCastling = 2;
		return 1;

	}
	if (type == 1 && (color == BLACK)) {
		mathboard[0][4] = 0;
		mathboard[0][6] = KING | color;
		mathboard[0][5] = ROOK | color;
		isBeRokB = true;
		typeCastling = 3;
		return 2;


	}
	if (type == 0 && (color == BLACK)) {
		mathboard[0][4] = 0;
		mathboard[0][2] = KING | color;
		mathboard[0][3] = ROOK | color;
		isBeRokB = true;
		typeCastling =  4;
		return 2;

	}
	if (type == 0 && (color == WHITE)) {
		isBeRokW = true;
		mathboard[7][4] = 0;
		mathboard[7][2] = KING | color;
		mathboard[7][3] = ROOK | color;
		isBeRokW = true;
		typeCastling = 5;
		return 2;
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