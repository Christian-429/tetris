#include "Engine.h"

Engine::Engine() : cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1)), board(ROWS, std::vector<int>(COLS)) {
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	int screenWidth =  ((COLS * CELL_SIZE)+ SIZEBAR_SIZE) * SCREEN_RESIZE;
	int screenHeight = (ROWS * CELL_SIZE) * SCREEN_RESIZE;
	sf::VideoMode vm(screenWidth, screenHeight);
	window.create(vm, "game", sf::Style::Default);
	window.clear(sf::Color::Black);
	window.display();
	window.setFramerateLimit(60);

	view.reset(sf::FloatRect(0, 0, (COLS * CELL_SIZE) + SIZEBAR_SIZE, ROWS * CELL_SIZE));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	window.setView(view);
	initData();

	if (!music.openFromFile("sound/8bit.wav")) {
		std::cout << "couldn't read\n";
	}
	music.setLoop(true);
	music.play();
	music.setVolume(35);
}

void Engine::run() {
	while (window.isOpen()) {
		handleInput();
		update();
		draw();
	}
}

void Engine::handleInput() {
	while (window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code) {
			case sf::Keyboard::Escape:
				window.close();
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				piece.moveLeft(board);
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				piece.moveRight(board);
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				piece.moveDown(board);
				break;
			case sf::Keyboard::Z:
			case sf::Keyboard::E:
				piece.rotate(board);
				break;
			case sf::Keyboard::X:
			case sf::Keyboard::Q:
				piece.hardDrop(board);
				break;
			}
		}
	}
}
void Engine::draw() {
	window.clear(sf::Color::Black);
	if (gameRunning) {
		drawBoard();
		drawPiece();
		std::string point = std::to_string(score);
		scoreBoard.setString(point);
		window.draw(title);
		window.draw(scoreBoard);
	}
	else {
		window.draw(title);
	}
	window.display();
}
void Engine::lockPiece() {
	for (auto& block : piece.getBlocks()) {
		board[block.y][block.x] = 1;
	}
}
void Engine::clearLines() {
	for (int row = ROWS - 1; row >= 0; row--) {
		bool full = true;
		for (int col = 0; col < COLS; col++) {
			if (board[row][col] == 0) { full = false; break; }
		}
		if (full) {
			board.erase(board.begin() + row);
			board.insert(board.begin(), std::vector<int>(COLS, 0));
			row++;
			score += 10;
		}
	}
}
void Engine::update() {
	if (moveDelay == 60) {
		piece.moveDown(board);
		moveDelay = 0;


	}
	moveDelay++;

	if (piece.isOnFloor(board)) {
		lockPiece();
		clearLines();
		gameRunning = piece.reset(rand() % 7, board);
		if (!gameRunning) {
			title.setString("GAME OVER >:(");
			title.setPosition((COLS * CELL_SIZE) / 2, (ROWS * CELL_SIZE) / 2);
		}
	}
}
void Engine::initData() {
	if (!font.loadFromFile("font/PixelFont.ttf")) { return; }
	title.setFont(font);
	title.setString("Tetris :D");
	title.setCharacterSize(30);
	title.setPosition((COLS * CELL_SIZE) + 10, 0);
	scoreBoard.setFont(font);
	scoreBoard.setPosition((COLS * CELL_SIZE)+ 120,0);
}
void Engine::drawBoard() {
	for (int rows = 0; rows < ROWS; rows++) {
		for (int cols = 0; cols < COLS; cols++) {
			if (board[rows][cols]) {
				cell.setFillColor(piece.getColor());
			}
			else {
				cell.setFillColor(BACK_GROUND);
			}
			cell.setPosition(cols * CELL_SIZE, rows * CELL_SIZE);
			window.draw(cell);
		}
	}
}
void Engine::drawPiece() {
	std::vector<sf::Vector2i> pieceBlocks = piece.getBlocks();
	for (auto& block : pieceBlocks) {
		cell.setFillColor(piece.getColor());
		cell.setPosition(block.x * CELL_SIZE, block.y * CELL_SIZE);
		window.draw(cell);
	}
}