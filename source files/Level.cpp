#include "Level.h"

Level::Level(Console gameWindow) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	COORD NewSBSize;

	//Retreive size of console
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	columns = SBInfo.srWindow.Right - SBInfo.srWindow.Left;
	rows = SBInfo.srWindow.Bottom - SBInfo.srWindow.Top;

	generateGrid();
}

void Level::generateGrid() {
	grid = new char* [rows];
	for (int i = 0; i < rows; i++) {
		grid[i] = new char[columns];
	}
}

void Level::destroyGrid() {
	for (int i = 0; i < rows; i++) {
		delete[] grid[i];
	}
	delete[] grid;
}

void Level::printGrid() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++)
			std::cout << grid[i][j];
		std::cout << std::endl;
	}
}

void Level::populateGrid() {
	std::cout << "Override this function on each new level";
}

void Level::gameLoop() {
	std::cout << "Override this function on each new level";
}