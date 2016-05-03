#include "timer.h"


TIMER::TIMER()
{
	start_clock = 0;
	end_clock = 0;

}
TIMER::TIMER(int size)
{
	TIMER::size = size;
}
void TIMER::start(void)
{
	start_clock = clock();
}
void TIMER::end(void)
{
	end_clock = clock();
}
double TIMER::time(void)
{
	return (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
}
void TIMER::print_time()
{
	gotoxy(size * 4 + 3, 0);
	std::cout << "     ";
	gotoxy(size * 4 + 3, 0);
	std::cout << this->time();
}