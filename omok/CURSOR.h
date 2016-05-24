#pragma once
#include <Windows.h>


typedef enum { BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE, BLACK_L, BLUE_L, GREEN_L, CYAN_L, RED_L, MAGENTA_L, YELLOW_L, WHITE_L } COLOR;
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

class CURSOR
{
private:
	CURSOR();
public:

	~CURSOR();
	static void setCursor(CURSOR_TYPE c);
	static void setColor(COLOR text, COLOR background);
	static void gotoxy (short x, short y);
};