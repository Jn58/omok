#pragma once
#include <ctime>
#include <iostream>
#include "gotoxy.h"


class TIMER
{
private:
	clock_t start_clock;
	clock_t end_clock;
	int size;
public:
	TIMER();
	TIMER(int size);
	void start(void);
	void end(void);
	double time(void);
	void print_time();



};