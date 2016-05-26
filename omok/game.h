#pragma once
#include "MAP.h"
#include <iostream>

class game
{
	
public:
	int turn = -1;
	MAP *map=NULL;
	game();
	~game();
	int check(POS p);
	void clear(void);
};

