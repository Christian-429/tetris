#include "Piece.h"

Piece::Piece() {
    int randNum = rand() % 7;
    blocks.clear();
    blocks = swapBlock(randNum);
    shape = randNum;
}
Piece::Piece(int _shape) {
	shape = _shape;

	blocks.clear();
    blocks = swapBlock(_shape);
}

std::vector<sf::Vector2i> Piece::getBlocks() const {
	return blocks;
}

bool Piece::moveDown(std::vector<std::vector<int>>& board) {
    for (auto& block : blocks) {
        int newY = block.y + 1;
        if (newY >= ROWS || board[newY][block.x] != 0) {
            return false;
        }
    }
    for (auto& block : blocks) {
        block.y += 1;
    }
    return true;
}
bool Piece::moveLeft(std::vector<std::vector<int>>& board) {
    for (auto& block : blocks) {
        int newX = block.x - 1;
        if (newX < 0 || board[block.y][newX] != 0) {
            return false;
        }
    }
    for (auto& block : blocks) {
        block.x--;
    }
    return true;
}
bool Piece::moveRight(std::vector<std::vector<int>>& board) {
    for (auto& block : blocks) {
        int newX = block.x + 1;
        if (newX >= COLS || board[block.y][newX] != 0) {
            return false;
        }
    }
    for (auto& block : blocks) {
        block.x++;
    }
    return true;
}

bool Piece::isOnFloor(std::vector<std::vector<int>>& board) {
    for (auto& block : blocks) {
        int newY = block.y + 1;
        if (newY >= ROWS || board[newY][block.x] != 0) {
            return true;
        }
    }
    return false;
}

bool Piece::reset(int _shape, std::vector<std::vector<int>>& board) {
    shape = _shape;
    blocks.clear();
    blocks = swapBlock(_shape);

    for (auto& block : blocks) {
        if (block.y >= 0 && board[block.y][block.x] != 0)
            return false; 
    }
    return true;

}

std::vector<sf::Vector2i> Piece::swapBlock(int shape) {
    std::vector<sf::Vector2i> blocks;
    switch (shape) {
    case 0: // I
        blocks = { {4,0}, {3,0}, {5,0}, {6,0} };
        color = line;
        break;
    case 1: // square
        blocks = { {4,0}, {3,0}, {3,1}, {4,1} };
        color = square;
        break;
    case 2: // T
        blocks = { {4,1}, {3,1}, {5,1} , {4,0} };
        color = tPiece;
        break;
    case 3: // N
        blocks = { {4,0}, {5,0}, {4,1}, {3,1} };
        color = nPiece;
        break;
    case 4: // Z
        blocks = { {4,0}, {3,0}, {4,1}, {5,1} };
        color = zPiece;
        break;
    case 5: // L
        blocks = { {5,0}, {4,1}, {4,0}, {6,0} };
        color = lPiece;
        break;
    case 6: // J
        blocks = { {5,1}, {4,1}, {4,0}, {6,1} };
        color = jPiece;
        break;
    }
    return blocks;

}
bool Piece::rotate(std::vector<std::vector<int>>& board) {
    if (shape == 1) { return true; }
    sf::Vector2i piviot = blocks[0];
    std::vector<sf::Vector2i> rotated;

    for (auto& block : blocks) {
        int relX = block.x - piviot.x;
        int relY = block.y - piviot.y;

        sf::Vector2i newBlock(piviot.x - relY, piviot.y + relX);
        rotated.push_back(newBlock);
    }
    for (auto& block : rotated) {
        if (block.x < 0 || block.x >= COLS || block.y < 0 || block.y >= ROWS)
            return false;
        if (block.y >= 0 && board[block.y][block.x] != 0)
            return false;
    }

    blocks = rotated;
    return true;
}
void Piece::hardDrop(std::vector<std::vector<int>>& board) {
    
    while (true) {

        for (auto& block : blocks) {
            int newY = block.y + 1;
            if (newY >= ROWS || board[newY][block.x] != 0) {
                return;
            }
        }
        for (auto& block : blocks) {
            block.y += 1;
        }
    }
}
sf::Color Piece::getColor() const {
    return color;
}

int Piece::getShape() const {
    return shape;
}