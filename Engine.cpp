#include "Engine.h"

Engine::Engine() : cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1)), board(ROWS, std::vector<int>(COLS)) {
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	int screenWidth = ((COLS * CELL_SIZE) + SIZEBAR_SIZE) * SCREEN_RESIZE;
	int screenHeight = (ROWS * CELL_SIZE) * SCREEN_RESIZE;
	sf::VideoMode vm(screenWidth, screenHeight);
	window.create(vm, "game", sf::Style::Default);
	window.clear(sf::Color::Black);
	window.display();
	window.setFramerateLimit(60);
	nextPiece = Piece(rand() % (7) + 1);
	piece.reset(rand() % 7, board);
	view.reset(sf::FloatRect(0, 0, (COLS * CELL_SIZE) + SIZEBAR_SIZE, ROWS * CELL_SIZE));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	window.setView(view);
	initData();

	if (!music.openFromFile("sound/8bit.wav")) {
		std::cout << "couldn't read\n";
	}
	music.setLoop(true);
	music.play();
	music.setVolume(25);

	if (!s1.loadFromFile("sound/pieceDrop.wav") || !s2.loadFromFile("sound/lineClear.wav") || !s3.loadFromFile("sound/gameOver.wav")) {
		std::cout << "error\n";
	}
	drop.setBuffer(s1);
	lineClear.setBuffer(s2);
	ending.setBuffer(s3);
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
				drop.play();
				piece.hardDrop(board);
				break;
			case sf::Keyboard::M:
				music.stop();
				break;
			}
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
		gameRunning = piece.reset(nextPiece.getShape(), board);
		nextPiece = Piece(rand() % (7) + 1);
		if (!gameRunning) {
			music.stop();
			ending.play();
			title.setString("GAME OVER >:(");
			title.setPosition((COLS * CELL_SIZE) / 2, (ROWS * CELL_SIZE) / 2);
			scoreBoard.setString("Final Score: " + std::to_string(score));
			scoreBoard.setPosition((COLS * CELL_SIZE) / 2, ((ROWS * CELL_SIZE) / 2) + 30);
		}
	}
}

void Engine::draw() {
	window.clear(sf::Color::Black);
	if (gameRunning) {
		drawBoard();
		drawPiece();
		drawSidebar();
	}
	else {
		window.draw(title);
		window.draw(scoreBoard);
	}
	window.display();
}

void Engine::lockPiece() {
	for (auto& block : piece.getBlocks()) {
		board[block.y][block.x] = piece.getShape();
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
			lineClear.play();
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
	scoreBoard.setCharacterSize(20);
	scoreBoard.setPosition((COLS * CELL_SIZE) + 120, 0);
}

void Engine::drawBoard() {
	for (int rows = 0; rows < ROWS; rows++) {
		for (int cols = 0; cols < COLS; cols++) {
			cell.setFillColor(colorBlock(rows,cols));
			//cell.setFillColor(board[rows][cols] ? piece.getColor() : BACK_GROUND);
			cell.setPosition(cols * CELL_SIZE, rows * CELL_SIZE);
			window.draw(cell);
		}
	}
}

void Engine::drawPiece() {
	for (auto& block : piece.getBlocks()) {
		cell.setFillColor(piece.getColor());
		cell.setPosition(block.x * CELL_SIZE, block.y * CELL_SIZE);
		window.draw(cell);
	}
}

void Engine::drawSidebar() {
	// score
	scoreBoard.setString(std::to_string(score));
	window.draw(title);
	window.draw(scoreBoard);

	// next piece
	for (auto& block : nextPiece.getBlocks()) {
		float x = (COLS * CELL_SIZE) + 20 + (block.x - 3) * CELL_SIZE;
		float y = 100 + block.y * CELL_SIZE;
		cell.setFillColor(nextPiece.getColor());
		cell.setPosition(x, y);
		window.draw(cell);
	}
	sf::RectangleShape box(sf::Vector2f(100, 100));
	box.setOutlineColor(sf::Color::White);
	box.setOutlineThickness(2);
	box.setFillColor(sf::Color::Transparent);
	box.setPosition((COLS * CELL_SIZE) + 10, 60);
	window.draw(box);
}
sf::Color colorBlock(int rows, int cols) {
	//cell.setFillColor(board[rows][cols] ? piece.getColor() : BACK_GROUND);
	if (board[rows][cols] == 1) { return line; }
	else if (board[rows][cols] == 2) { return square; }
	else if (board[rows][cols] == 3) { return tPiece; }
	else if (board[rows][cols] == 4) { return nPiece; }
	else if (board[rows][cols] == 5) { return zPiece; }
	else if (board[rows][cols] == 6) { return lPiece; }
	else if (board[rows][cols] == 7) { return jPiece; }
	else {
		return BACK_GROUND;
	}
}