#pragma once
#include <SFML/Graphics.hpp>

const int ROWS = 20, COLS = 10;
const int CELL_SIZE = 16;
const int SIZEBAR_SIZE = 150;
const int SCREEN_RESIZE = 4;
const sf::Color BACK_GROUND(36, 36, 84);

//pieces
const sf::Color tPiece(255, 0, 0);
const sf::Color square(247, 196, 0);
const sf::Color line(73, 215, 241);
const sf::Color nPiece(75, 170, 72);
const sf::Color zPiece(234, 64, 191);
const sf::Color jPiece(238, 141, 46);
const sf::Color lPiece(183, 72, 243);

class Piece {
private:
	std::vector<sf::Vector2i> blocks;
	int shape;
	sf::Color color;
public:
	Piece();
	Piece(int _shape);

	bool moveDown(std::vector<std::vector<int>>& board);
	bool moveLeft(std::vector<std::vector<int>>& board);
	bool moveRight(std::vector<std::vector<int>>& board);

	bool isOnFloor(std::vector<std::vector<int>>& board);
	bool reset(int _shape, std::vector<std::vector<int>>& board);
	std::vector<sf::Vector2i> swapBlock(int shape);
	bool rotate(std::vector<std::vector<int>>& board);
	void hardDrop(std::vector<std::vector<int>>& board);

	sf::Color getColor() const;
	std::vector<sf::Vector2i> getBlocks() const;
	int getShape() const;
};