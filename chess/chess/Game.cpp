#include "Game.h"
#include <iostream>

int Game::drawChangePawnFigureChoose(const int& selectedFigure) {
	int y = isWhiteMove ? 1 : 0;
	chessFigure[selectedFigure].move(offset);
	sf::Vector2f pos = chessFigure[selectedFigure].getPosition();
	chessFigureChoose[0].setScale(0.5, 0.5);
	chessFigureChoose[1].setScale(0.5, 0.5);
	chessFigureChoose[2].setScale(0.5, 0.5);
	chessFigureChoose[3].setScale(0.5, 0.5);
	chessFigureChoose[0].setPosition(pos.x - size / 4, pos.y + size / 4 );
	chessFigureChoose[1].setPosition(pos.x + size / 4, pos.y + size/ 4);
	chessFigureChoose[2].setPosition(pos.x + size / 4, pos.y - size/ 4);
	chessFigureChoose[3].setPosition(pos.x - size / 4, pos.y - size/ 4);
	chessFigure[selectedFigure].setScale(0.0, 0.0);
	for (int i = 0; i < 4; i++) chessFigureChoose[i].move(14., 14.);   
	return 1;
}

int Game::makeMove(const sf::Vector2f& oldPos, sf::Vector2f& newPos, const int& selectedFigure) {
	statusCode = 0;
	int status = round->checkMove(toCoord8x8(oldPos), toCoord8x8(newPos), isWhiteMove);
	if (status) {
		if (status == DOCASTLING) {
		   return drawCastling(round->getTypeCatling());
		}

	   for (int i = 0; i < 32; i++) {
			if (chessFigure[i].getPosition() == newPos) {
				chessFigure[i].setPosition(-100, -100);
				break;
			}
		}
		for (int i = 0; i < 32; i++) {
			if (chessFigure[i].getPosition() == oldPos) {
				chessFigure[i].setPosition(newPos);
				break;
			}            
		}
		if (status == CHANGE_PAWN) {
			chessFigure[selectedFigure].setPosition(newPos);
			drawChangePawnFigureChoose(selectedFigure);
			return 1;
		}
		isWhiteMove = !isWhiteMove;
		if (status == CHECKMATE) {
			statusCode = 4;
		}
	}
	else if (status == PAT) {

		statusCode = 5;
		newPos = oldPos;
	}
	else if(status==0){
		statusCode = 3;
		newPos = oldPos;
	}
	return 1;
}

void Game::init() {
	buffer.loadFromFile("../image/vac.wav");
	sound.setBuffer(buffer);
	t1.loadFromFile("../image/figures.png");
	t2.loadFromFile("../image/board.png");

	for (int i = 0; i < 4; i++){
	   chessFigureChoose[i].setTexture(t1);
	}
	for (int i=0; i < 32; i++) {
		chessFigure[i].setTexture(t1);
	}
	board.setTexture(t2);
	sf::Font font;
	font.loadFromFile("../image/arial.ttf");
	alert.set(2*size, 0.06*size, font);
	menuTexture1.loadFromFile("../image/111.png");
	menuTexture3.loadFromFile("../image/333.png");
	menu1.setTexture(menuTexture1);
	menu2.setTexture(menuTexture2);
	menu3.setTexture(menuTexture3);
	loadPosition();
}

Game::Game():window(sf::VideoMode(504, 504), "v1", sf::Style::Close | sf::Style::Titlebar), offset(28, 28) {
	isRun = true;
}

int Game::execute(){  
	bool isMove = false;
	float dx = 0, dy = 0; // old 
	sf::Vector2f oldPos, newPos;
	int selectedFigure = 0;
	bool isMenu = true;
	int menuNum = 0;
	menu1.setPosition(150, 100);
	menu3.setPosition(150, 204);
	sound.play();

	while (window.isOpen()) {
		sf::Vector2i pos = sf::Mouse::getPosition(window) - sf::Vector2i(offset);
		sf::Event e;
		menu1.setColor(sf::Color::White);
		menu2.setColor(sf::Color::White);
		menu3.setColor(sf::Color::White);
		menuNum = 0;

		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) { 
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { 
				isMenu = true;
			}

			if (isMenu) {
				if (sf::IntRect(150, 100, 300, 50).contains(sf::Mouse::getPosition(window))) {  menuNum = 1; }
				if (sf::IntRect(150, 204, 300, 50).contains(sf::Mouse::getPosition(window))) {  menuNum = 3; }
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))                {
					if (menuNum == 1) {
						dx = 0, dy = 0;
						selectedFigure = 0;
						isWhiteMove = true;
						loadPosition();
						delete round;
						round = new Chess();
						statusCode = 0;
						isMenu = false;
						isRun = true;
					}
					if (menuNum == 3) { window.close(); isMenu = false; }
				}
			}
			if(!isMenu && isRun) {
				if (e.type == sf::Event::MouseButtonPressed) {
					if (round->getIsWhaiteNewFigure() &&  e.key.code == sf::Mouse::Left) {
						for (int i = 0; i < 4; i++) chessFigureChoose[i].move(-28, -28.);
						if (e.key.code == sf::Mouse::Left) {
							for (int i = 0; i < 4; i++) {
								if (chessFigureChoose[i].getGlobalBounds().contains(pos.x, pos.y)) {
									newPos = sf::Vector2f(size * int(pos.x / size), size * int(pos.y / size));
									if (round->changePawn(i, toCoord8x8(newPos))==CHECKMATE) {
										statusCode = 4;
									}
									for (int z = 0; z < 4; z++) {
										chessFigureChoose[z].setPosition(-100, -100);
									}
									int color = isWhiteMove ? 1 : 0;
									for (int j = 0; j < 32; j++) {
										chessFigure[j].setScale(1.0, 1.0);
										if (chessFigure[j].getGlobalBounds().contains(pos.x, pos.y)) {
											chessFigure[j].setTextureRect(sf::IntRect(size * i, size * color, size, size));
											chessFigure[j].setScale(1.0, 1.0);
											isWhiteMove = !isWhiteMove;
										}
									}
								}
							}

						}
						for (int i = 0; i < 4; i++) chessFigureChoose[i].move(28, 28);

					}
				}

				if (e.type == sf::Event::MouseButtonPressed) {                          

					if (!round->getIsWhaiteNewFigure() && e.key.code == sf::Mouse::Left) {
						for (int i = 0; i < 32; i++)
							if (chessFigure[i].getGlobalBounds().contains(pos.x, pos.y))                            {
								if ((isWhiteMove && i > 15) || (!isWhiteMove && i < 16)) {
									isMove = true;
									selectedFigure = i;
									dx = pos.x - chessFigure[i].getPosition().x;
									dy = pos.y - chessFigure[i].getPosition().y;
									oldPos = chessFigure[i].getPosition();
								}
							}
					}
				}
				if (e.type == sf::Event::MouseButtonReleased) {
					if (e.key.code == sf::Mouse::Left) {
						if ((!round->getIsWhaiteNewFigure() && isWhiteMove && selectedFigure > 15) || (!isWhiteMove && selectedFigure < 16)) {
							isMove = false;
							sf::Vector2f p = chessFigure[selectedFigure].getPosition() + sf::Vector2f(size / 2, size / 2);
							newPos = sf::Vector2f(size * int(p.x / size), size * int(p.y / size));
	
							if (newPos != oldPos) {
								if (makeMove(oldPos, newPos, selectedFigure)) {
									chessFigure[selectedFigure].setPosition(newPos);
								}
							}
							else {
								chessFigure[selectedFigure].setPosition(oldPos);
							}
						}
					}
				}

				if (isMove) {
					chessFigure[selectedFigure].setPosition(pos.x - dx, pos.y - dy);
				}							   				 
				window.clear();
				window.draw(board);
				for (int i = 0; i < 32;  i++) chessFigure[i].move(offset);
				for (int i = 0; i < 32; i++) window.draw(chessFigure[i]);
				window.draw(chessFigure[selectedFigure]);
				if (round->getIsWhaiteNewFigure()) {
					window.draw(chessFigureChoose[0]);
					window.draw(chessFigureChoose[1]);
					window.draw(chessFigureChoose[2]);
					window.draw(chessFigureChoose[3]);
				}
				for (int i = 0; i < 32; i++) chessFigure[i].move(-offset);

				if (statusCode == 4) {
					alert.drawMessage(4, window);
					isRun = false;
				}
				else if (round->getIsCheck()) {
					alert.drawMessage(6, window);
				}
				else if (statusCode == 3) {
					alert.drawMessage(3, window);
				}
				else if (statusCode == 5) {
					alert.drawMessage(5, window);
					isRun = false;
				}
				else if (isWhiteMove) {
					alert.drawMessage(1, window);
				}
				else if (!isWhiteMove) {
					alert.drawMessage(2, window);
				}


				window.display();
			}
		}
		if (isMenu) {
			window.clear();
			window.draw(board);
			window.draw(menu1);
			window.draw(menu2);
			window.draw(menu3);
			window.display();
		}
	}
	return 0;
}

void Game::exit(){
	isRun = false;
}

void Game::loadPosition(){
	int k = 0;
	chessFigureChoose[0].setTextureRect(sf::IntRect(size * 0, size * 0, size, size));
	chessFigureChoose[1].setTextureRect(sf::IntRect(size * 1, size * 0, size, size));
	chessFigureChoose[2].setTextureRect(sf::IntRect(size * 2, size * 0, size, size));
	chessFigureChoose[3].setTextureRect(sf::IntRect(size * 3, size * 0, size, size));
	chessFigureChoose[0].setColor(sf::Color::Green);
	chessFigureChoose[1].setColor(sf::Color::Green);
	chessFigureChoose[2].setColor(sf::Color::Green);
	chessFigureChoose[3].setColor(sf::Color::Green);
	for (int z = 0; z < 4; z++) {
		chessFigureChoose[z].setPosition(-100, -100);
	}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++){
			int n = initBoard[i][j];
			if (!n) continue;
			int x = abs(n) - 1;
			int y = n > 0 ? 1 : 0;
			chessFigure[k].setTextureRect(sf::IntRect(size * x, size * y, size, size));
			chessFigure[k].setPosition(size * j, size * i);
			k++;
		}
}

sf::Vector2f Game::toCoord8x8(sf::Vector2f pos) {
	return sf::Vector2f(pos.x / size, pos.y / size);
}




int Game::drawCastling(const int& type) {
	if (type == 2) {
		chessFigure[28].setPosition(6 * size, 7 * size);
		chessFigure[31].setPosition(5 * size, 7 * size);
	}
	if (type == 3) {
		chessFigure[4].setPosition(6 * size, 0 * size);
		chessFigure[7].setPosition(5 * size, 0 * size);
	}
	if (type == 4) {
		chessFigure[4].setPosition(2 * size, 0 * size);
		chessFigure[0].setPosition(3 * size, 0 * size);
	}
	if (type == 5) {
		chessFigure[28].setPosition(2 * size, 7 * size);
		chessFigure[24].setPosition(3 * size, 7 * size);
	}
	isWhiteMove = !isWhiteMove;
	return 0;
}

// для подключение в будущем Stockfish
std::string Game::toChessNote(sf::Vector2f p) {
	std::string s = "";
	s += char(p.x / size + 49);
	s += char(p.y / size + 49);
	std::cout << s;
	return s;
}

sf::Vector2f Game::toCoord(char a, char b) {
	int x = int(a) + 49;
	int y = int(b) + 49;
	return sf::Vector2f(x * size, y * size);
}