#include "Console.h"

Console::Console() {
	consoleSize.cx = 640;
	consoleSize.cy = 480;
	initConsole();
}

Console::Console(int width, int height) {
	consoleSize.cx = width;
	consoleSize.cy = height;
	initConsole();
}

bool Console::initConsole() {
	consoleWindow = GetConsoleWindow();		//Get handle to console
	SIZE monitorSize;
	if (setFont())							//Set font
		if (getMonitorSize(&monitorSize))		//Set Monitor Size
			if (setPosAndSize(monitorSize))	//Center and change size
				if(showConsoleCursor(false))	//Hide cursor
					if(removeScrollBar())	//Hide scrollbar
						return 1;			//success
	return 0;
}

bool Console::getMonitorSize(SIZE* monitorSize) {
	HMONITOR monitor = MonitorFromWindow(consoleWindow, MONITOR_DEFAULTTONEAREST); //Get handle to monitor
	MONITORINFO info; //Declare object to hold monitor info
	info.cbSize = sizeof(MONITORINFO); //Initialize cbsize
	if (GetMonitorInfo(monitor, &info)) { //Retreive monitor info and store it in info object
		monitorSize->cx = info.rcMonitor.right - info.rcMonitor.left; //Set the monitor width
		monitorSize->cy = info.rcMonitor.bottom - info.rcMonitor.top; //Set the monitor height
		return 1; //success
	}
	return 0;
}

bool Console::setPosAndSize(SIZE monitorSize) {
	consolePos.cx = monitorSize.cx / 2 - consoleSize.cx / 2;
	consolePos.cy = monitorSize.cy / 2 - consoleSize.cy / 2;

	DWORD style = GetWindowLong(consoleWindow, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(consoleWindow, GWL_STYLE, style);
	return SetWindowPos(consoleWindow, 0, consolePos.cx, consolePos.cy, consoleSize.cx, consoleSize.cy, SWP_SHOWWINDOW | SWP_FRAMECHANGED); //Change window position and size
}

bool Console::setFont() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	return SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

bool Console::removeScrollBar() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	COORD NewSBSize;

	//Retreive size of console
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	columns = SBInfo.srWindow.Right - SBInfo.srWindow.Left + 1;
	rows = SBInfo.srWindow.Bottom - SBInfo.srWindow.Top + 1;

	NewSBSize.X = columns;
	NewSBSize.Y = rows;

	return SetConsoleScreenBufferSize(hOut, NewSBSize); //Set buffer to size of console
}

bool Console::showConsoleCursor(bool flag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = flag; // set the cursor visibility
	return SetConsoleCursorInfo(out, &cursorInfo);
}