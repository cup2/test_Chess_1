#include "Game.h"
#include <iostream>

sf::Vector2f offset(28, 28);

int Game::makeMove(const sf::Vector2f& oldPos, sf::Vector2f& newPos, const int& selectedFigure)
{
    statusCode = 0;
    int test1 = round.checkMove(toCoord8x8(oldPos), toCoord8x8(newPos), isWhiteMove);
    if (test1) {
        if (test1 == 10) {
            drawChangePawnFigure(selectedFigure);
        }
        if (test1 >1 && test1!=10) {
           return drawCastling(test1);
        }
        for (int i = 0; i < 32; i++) {
            if (chessFigure[i].getPosition() == newPos) {
                std::cout << i << std::endl;
                chessFigure[i].setPosition(-100, -100);
            }
        }
        for (int i = 0; i < 32; i++) {
            if (chessFigure[i].getPosition() == oldPos) {
                chessFigure[i].setPosition(newPos);
                break;
            }
        }
        isWhiteMove = !isWhiteMove;
    }
    else {
        statusCode = 3;
        newPos = oldPos;
    }
    return 1;

}

void Game::init() {
    sf::SoundBuffer buffer;
    buffer.loadFromFile("../image/vac.wav");
    sound.setBuffer(buffer);
	t1.loadFromFile("../image/figures.png");
	t2.loadFromFile("../image/board.png");
	board.setTexture(t2);
	for (int i=0; i < 32; i++) {
		chessFigure[i].setTexture(t1);
	}
    sf::Font font;
    font.loadFromFile("../image/arial.ttf");
    alert.set(2*size, 0.06*size, font);
    loadPosition();
    menuTexture1.loadFromFile("../image/111.png");
    menuTexture2.loadFromFile("../image/222.png");
    menuTexture3.loadFromFile("../image/333.png");
    menu1.setTexture(menuTexture1);
    menu2.setTexture(menuTexture2);
    menu3.setTexture(menuTexture3);
}

Game::Game(){
	isRun = true;
}

int Game::execute(){
    sf::RenderWindow window(sf::VideoMode(504, 504), "v1", sf::Style::Close | sf::Style::Titlebar);
    sound.play();
    bool isMove = false;
    float dx = 0, dy = 0; // old 
    sf::Vector2f oldPos, newPos;
    std::string str;
    int selectedFigure = 0;

    bool isMenu = true;
    int menuNum = 0;
    menu1.setPosition(100, 30);
    menu2.setPosition(100, 90);
    menu3.setPosition(100, 150);
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
                if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) {  menuNum = 1; }
                if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(window))) {  menuNum = 3; }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))                {
                    if (menuNum == 1) isMenu = false; 
                    if (menuNum == 3) { window.close(); isMenu = false; }
                }
            }
            else {
                /////drag and drop///////
                if (e.type == sf::Event::MouseButtonPressed) {
                    if (e.key.code == sf::Mouse::Left) {
                        for (int i = 0; i < 32; i++)
                            if (chessFigure[i].getGlobalBounds().contains(pos.x, pos.y))
                            {
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
                    if (e.key.code == sf::Mouse::Left) 
                    {
                        if ((isWhiteMove && selectedFigure > 15) || (!isWhiteMove && selectedFigure < 16)) {

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
                if (statusCode == 3) {
                    alert.drawMessage(3, window);
                }
                else if (statusCode == 4) {
                    alert.drawMessage(3, window);
                }
                else if (statusCode == 4) {
                    alert.drawMessage(3, window);
                }
                else if (isWhiteMove) {
                    alert.drawMessage(1, window);
                }
                else {
                    alert.drawMessage(2, window);
                }
                for (int i = 0; i < 32; i++) chessFigure[i].move(offset);
                for (int i = 0; i < 32; i++) window.draw(chessFigure[i]);
                window.draw(chessFigure[selectedFigure]);
                for (int i = 0; i < 32; i++) chessFigure[i].move(-offset);
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


int Game::drawChangePawnFigure(const int& selectedFigure) {
    int y = isWhiteMove ? 1 : 0;
    chessFigure[selectedFigure].setTextureRect(sf::IntRect(3 * size, size * y, size, size));
    return 1;
};

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