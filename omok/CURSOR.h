#pragma once
#include <Windows.h>

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
typedef enum { BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE, BLACK_L, BLUE_L, GREEN_L, CYAN_L, RED_L, MAGENTA_L, YELLOW_L, WHITE_L } COLOR;

class CURSOR
{
private:
	CURSOR();
public:
	~CURSOR();
	static void setCursor(CURSOR_TYPE c);
	static void setColor(COLOR text, COLOR background);
	template <T> static void gotoxy (T x, T y);
};