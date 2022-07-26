#include "Level.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class Level1 : public Level {
public:
	Level1(Console gameWindow) : Level(gameWindow) {}
	
	//Floor vars
	const int floorLevel = 5;
	int floor = rows - floorLevel;

	//Obstacle vars
	int obstacleX = columns - 1;
	const int maxObstacleHeight = 3;
	int maxObstacleY = floor - maxObstacleHeight;	//Max Y-Coordinate of obstacle

	//Player vars
	int maxCharHeight = 3;
	int charY = floor - maxCharHeight;
	int charX = 6;

	//Game vars
	int score = 0;
	int tick = 40;
	bool falling = true;
	int count = 0;

	void populateGrid() override { //Map generation
		srand(time(NULL));

		//Generate floor
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (i < floor)
					grid[i][j] = ' ';
				else
					grid[i][j] = '.';
			}
		}
	}
	
	void gameLoop() override {
		PlaySound("gameLoop.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
		printGrid();
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)(columns / 2) - 4, 0 });
		std::cout << "Score: " << score;

		charY -= 5;
		while (1) {
			if (charX == obstacleX && charY == maxObstacleY)
				return;
			if (falling) {
				if(count %2 == 0)
					charY += 1;
				count++;
				if(count == 10)
					falling = false;
			}
			else {
				count = 0;
				handleInput();
			}
			//Set Player
			grid[charY][charX] = 'O';
			grid[charY + 1][charX] = '|';
			grid[charY + 2][charX] = 'A';

			//Set obstacle
			grid[maxObstacleY][obstacleX] = 'V';
			grid[maxObstacleY + 1][obstacleX] = 'V';
			grid[maxObstacleY + 2][obstacleX] = 'V';

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, (SHORT)maxObstacleY-5 });	//Reset cursor position

			//Draw player & obstacle
			for (int i = maxObstacleY-5; i < floor; i++) {
				for (int j = 0; j < columns; j++)
					std::cout << grid[i][j];
				std::cout << std::endl;
			}

			//Every tick, move obstacle to the left
			Sleep(tick);
			writeToView(0, 2, ' ', floorLevel + 1); //Clear view above floor level
			grid[maxObstacleY][obstacleX] = ' ';
			grid[maxObstacleY + 1][obstacleX] = ' ';
			grid[maxObstacleY + 2][obstacleX] = ' ';

			grid[charY][charX] = ' ';
			grid[charY + 1][charX] = ' ';
			grid[charY + 2][charX] = ' ';

			obstacleX--;

			if (obstacleX < 0) {
				//Update score
				score++;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)(columns / 2) - 4, 0});
				std::cout << "Score: " << score;
				//Increase difficulity
				if (tick > 10)
					tick -= 5;
				else if (tick > 5)
					tick -= 1;
				else
					tick = 1;

				//Wrap around
				obstacleX = columns - (rand() % 50 + 1);
			}
		}
	}

	void handleInput() {
		if (GetKeyState(VK_SPACE) & 0x8000) { //If space is pressed
			falling = true;
			//Draw character
			charY -= 5;
		}
	}

	void writeToView(int x, int y, char c, int floor) {
		HANDLE                     hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD                      count;
		DWORD                      cellCount;
		COORD                      homeCoords = { x, y };

		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) return;

		/* Get the number of cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
		cellCount = (csbi.dwSize.X) * (csbi.dwSize.Y - floor - 2);

		/* Fill the entire buffer with spaces */
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR)c,
			cellCount,
			homeCoords,
			&count
		)) return;

		/* Fill the entire buffer with the current colors and attributes */
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) return;

		/* Move the cursor home */
		SetConsoleCursorPosition(hStdOut, homeCoords);
	}

	void mainMenu() {
		PlaySound(NULL, 0, 0);
		PlaySound("splash.wav", NULL, SND_FILENAME | SND_ASYNC);
		system("cls");
		//writeToView(0, 0, '-', 0);
		//writeToView(2, 1, '.', 0);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, (SHORT)(rows / 2) });
		std::cout.width((columns/2 + 10)); //11 being half of glitch-hop
		std::cout.fill('-');
		std::cout << "Gl 1 tch  -  H 0 P";

		std::cout.width(columns / 2 - 9); //11 being half of glitch-hop
		std::cout.fill('-');
		std::cout << "";

		Sleep(2000);
	}
};

int main() {
	Console gameWindow;		//Create new window
	while (1) {
		Console gameWindow;		//Recreate new window (due to bug with scrollbar removal)
		Level1 lvl(gameWindow);	//Create new level
		lvl.mainMenu();		//Initialize menu
		lvl.populateGrid();		//Generate level
		lvl.gameLoop();		//Print level
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)(lvl.columns / 2) - 17, 2 });
		std::cout << "Game Over! Restarting in 5 seconds..";
		PlaySound("lose.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(5000);
	}
	return 0;
}