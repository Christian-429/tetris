#pragma once
#include "Piece.h"
#include <SFML/Audio.hpp>
#include <iostream>

class Engine {
public:
	Engine();
	void run();
private:
	sf::RenderWindow window;
	sf::View view;
	sf::RectangleShape cell;
	sf::Event event;
	sf::Font font;
	sf::Text title;
	sf::Text scoreBoard;
	sf::Music music;

	Piece piece;
	std::vector<std::vector<int>> board;
	int moveDelay = 0;
	int score = 0;
	bool gameRunning = true;

	void handleInput();
	void draw();
	void lockPiece();
	void clearLines();
	void update();
	void initData();
	void drawBoard();
	void drawPiece();
};