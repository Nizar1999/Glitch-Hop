#pragma once

#include "Console.h"
#include <iostream>

class Level {
public:
	int rows;
	int columns;
	char** grid;

	Level(Console);
	
	void generateGrid();
	void destroyGrid();
	void printGrid();
	virtual void populateGrid();
	virtual void gameLoop();
};