#pragma once

#include <atlbase.h>
#include <atlwin.h>
#include <Winuser.h>
#pragma comment(lib, "winmm.lib") //Playsound library

//Initialize console position and size
class Console {
private:
	HWND consoleWindow;
	SIZE consoleSize; //256x192, 320x240, 640x480, 800x600, 960x720, 1024x768, 1280x960, 1440 x1080, 1600x1200, 1920x1440
	SIZE consolePos;

	bool initConsole();
	bool getMonitorSize(SIZE*);		//Retreive size of current monitor
	bool setPosAndSize(SIZE);		//Set console to center
	bool setFont();				//Set font to consolas-16
	bool removeScrollBar();			//Remove the scrollbar
	bool showConsoleCursor(bool);		//Toggle cursor visibility

public:
	int columns, rows;
	Console();
	Console(int, int);
};