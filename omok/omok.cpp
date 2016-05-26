#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <string>
#include <thread>
#include <mutex>
#include <stack>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "gotoxy.h"
#include "timer.h"
#include "MAP.h"
#include "AI.h"
#include "CURSOR.h"
#include "ENTITY.h"
#include "game.h"



#define MAP_SIZE 15
#define SAVED 2
#define MULTIPLE 1

//#define TEST
#define LEARNING

#ifdef TEST
using namespace std;

void playpos(int x, int y);
void initMap(int size);
void print(POS p, int t);
POS player(void);
int check(const MAP * const map)
{
	int j, k;

	for (j = 0; j < map->size - 4; j++) // 대각선
	{
		for (k = 0; k < map->size - 4; k++)
		{
			if (map->map[j][k] && map->map[j][k] == map->map[1 + j][1 + k] && map->map[j][k] == map->map[2 + j][2 + k] && map->map[j][k] == map->map[3 + j][3 + k] && map->map[j][k] == map->map[4 + j][4 + k])
			{
				if (j == map->size - 5 && k == 0)
				{
					return map->map[j][k];
				}
				else if (j == 0 && k == map->size - 5)
				{
					return map->map[j][k];
				}
				if (j == 0 || k == 0)
				{
					if (map->map[j][k] != map->map[j + 5][k + 5]) return map->map[j][k];
				}
				else if (j == map->size - 5 || k == map->size - 5)
				{
					if (map->map[j][k] != map->map[j - 1][k - 1]) return map->map[j][k];
				}
				else
				{
					if ((map->map[j][k] != map->map[j + 5][k + 5]) && (map->map[j][k] != map->map[j - 1][k - 1])) return map->map[j][k];

				}
			}
			if (map->map[4 + j][k] && map->map[4 + j][k] == map->map[3 + j][1 + k] && map->map[4 + j][k] == map->map[2 + j][2 + k] && map->map[4 + j][k] == map->map[1 + j][3 + k] && map->map[4 + j][k] == map->map[j][4 + k])
			{
				if (j == 0 && k == 0)
				{
					return map->map[4 + j][k];
				}
				else if (j == map->size - 5 && k == map->size - 5)
				{
					return map->map[4 + j][k];
				}
				else if (j == 0 || k == map->size - 5)
				{
					if (map->map[4 + j][k] != map->map[j + 5][k - 1]) return map->map[4 + j][k];
				}
				else if (j == map->size - 5 || k == 0)
				{
					if (map->map[4 + j][k] != map->map[j - 1][k + 5]) return map->map[4 + j][k];
				}
				else
				{
					if ((map->map[4 + j][k] != map->map[j + 5][k - 1]) && (map->map[4 + j][k] != map->map[j - 1][k + 5])) return map->map[4 + j][k];

				}
			}
		}
	}





	for (j = 0, k = 0; j < map->size; j++, k = 0) // y축 방향
	{
		if (map->map[j][k] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k] && map->map[j][k] != map->map[j][5 + k]) return map->map[j][k];
		for (k = 1; k < map->size - 5; k++)
		{
			if (map->map[j][k] && map->map[j][k] != map->map[j][k - 1] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k] && map->map[j][k] != map->map[j][5 + k]) return map->map[j][k];
		}
		if (map->map[j][k] && map->map[j][k] != map->map[j][k - 1] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k]) return map->map[j][k];
	}
	for (k = 0, j = 0; k < map->size; k++, j = 0) //x축 방향
	{
		if (map->map[j][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k] && map->map[j][k] != map->map[5 + j][k]) return map->map[j][k];
		for (j = 1; j < map->size - 5; j++)
		{
			if (map->map[j][k] && map->map[j][k] != map->map[j - 1][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k] && map->map[j][k] != map->map[5 + j][k]) return map->map[j][k];
		}
		if (map->map[j][k] && map->map[j][k] != map->map[j - 1][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k]) return map->map[j][k];
	}
	for (k = 0; k < map->size; k++)
		for (j = 0; j < map->size; j++)
			if (map->map[k][j] == 0) return 0;

	return 3;
}
void delTxt(void);
bool rule(MAP * m, POS * p, int t);


MAP map;



int main(void)
{

	POS p;
	PARAMETER pa;
	ostringstream setting;
	TIMER *timer;
	CURSOR::setCursor(CURSOR_TYPE::NOCURSOR);
	map.makeMap(MAP_SIZE);
	map.clearMap();
	setting << "mode con:cols=" << map.size * 4 + 6 << " lines=" << map.size * 2 + 10;
	system(setting.str().c_str());
	initMap(map.size);
	AI * ai = new AI();
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				pa.value[i][j][k] = 2;
			}
		}
	}
	ai->setParameter(pa);
	ai->start();
	p = ai->nextPosition();
	print(p, 1);
	map.map[p.x][p.y] = 1;
	while (1)
	{
		int ch;
		p = player();
		map.map[p.x][p.y] = -1;
		print(p, -1);
		ch = check(&map);
		if (ch)
		{
			delTxt();
			if (ch == 1)
			{

				cout << "AI win" << endl;
				break;
			}
			else if (ch == -1)
			{
				cout << "Human win" << endl;
				break;
			}
			else
			{
				cout << "Draw!" << endl;
				break;
			}
		}
		delTxt();
		cout << "AI is thinking..." << endl;
		//timer->start();
		ai->playTurn(p);
		p = ai->nextPosition();
		//timer->end();
		print(p, 1);
		map.map[p.x][p.y] = 1;
		//timer->print_time();
		//if (timer->time() >= 1) exit(0);

		ch = check(&map);
		if (ch)
		{
			delTxt();
			if (ch == 1)
			{
				cout << "AI win" << endl;
				break;
			}
			else if (ch == -1)
			{
				cout << "Human win" << endl;
				break;
			}
			else
			{
				cout << "Draw!" << endl;
				break;
			}
		}
	}
	delete ai;
	system("pause");
	return 0;
}





void playpos(int x, int y)
{
	gotoxy(4 * x + 3, y * 2 + 1);
}

void initMap(int size)
{
	CURSOR::setColor(WHITE_L, BLACK);
	printf("   A ");
	for (int i = 0; i < size - 1; i++)
	{
		printf("  %c ", i + 'B');
	}
	printf("\n");
	printf(" 1 ");
	CURSOR::setColor(BLACK, YELLOW);
	printf("┌─");
	for (int i = 0; i < size - 2; i++) printf("┬─");
	printf("┐");
	CURSOR::setColor(WHITE_L, BLACK);
	cout << 1 << endl;
	printf("   ");
	CURSOR::setColor(BLACK, YELLOW);
	printf("│");
	for (int i = 0; i < size - 1; i++) printf("　│");
	printf("\n");
	for (int j = 0; j < size - 2; j++)
	{
		CURSOR::setColor(WHITE_L, BLACK);
		printf("%2d ", j + 2);
		CURSOR::setColor(BLACK, YELLOW);
		printf("├─");
		for (int i = 0; i < size - 2; i++) printf("┼─");

		printf("┤");
		CURSOR::setColor(WHITE_L, BLACK);
		cout << j + 2 << endl;
		printf("   ");
		CURSOR::setColor(BLACK, YELLOW);
		printf("│");
		for (int i = 0; i < size - 1; i++) printf("　│");
		printf("\n");
	}
	CURSOR::setColor(WHITE_L, BLACK);
	printf("%2d ", size);
	CURSOR::setColor(BLACK, YELLOW);
	printf("└─");
	for (int i = 0; i < size - 2; i++) printf("┴─");
	printf("┘");
	CURSOR::setColor(WHITE_L, BLACK);
	cout << size << endl;
	CURSOR::setColor(WHITE_L, BLACK);
	printf("   A ");
	for (int i = 0; i < size - 1; i++)
	{
		printf("  %c ", i + 'B');
	}

}
void print(POS p, int t)
{

	playpos(p.x, p.y);
	if (t == 1)
	{
		CURSOR::setColor(BLACK, YELLOW);
		printf("●");
	}
	else
	{
		CURSOR::setColor(WHITE_L, YELLOW);
		printf("●");
	}

	CURSOR::setColor(WHITE_L, BLACK);
}
POS player(void)
{
	delTxt();
	while (1)
	{
		POS p;
		printf("Enter position:");
		string input;
		cin >> input;
		p.x = (input[0] & ~32) - 'A';
		p.y = atoi(input.c_str() + 1) - 1;
		if (rule(&map, &p, -1) || p.x < 0 || p.x >= map.size || p.y < 0 || p.y >= map.size || map.map[p.x][p.y] != 0)
		{
			delTxt();
			cout << "You can not put there.\nRetry." << endl;
			continue;
		}
		return p;
	}
}


bool rule(MAP * m, POS * p, int t)
{
	int count = 0;
	int count_double = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (p->x - i >= 0 && p->x - i < m->size&&p->x + 3 * i >= 0 && p->x + 3 * i < m->size&&p->y - j >= 0 && p->y - j < m->size&&p->y + 3 * j >= 0 && p->y + 3 * j < m->size)
			{
				if (m->map[p->x - i][p->y - j] == 0 && m->map[p->x + i][p->y + j] == t && m->map[p->x + 2 * i][p->y + 2 * j] == t && m->map[p->x + 3 * i][p->y + 3 * j] == 0) count++; // oXxxo
			}
			if (p->x - i >= 0 && p->x - i < m->size&&p->x + 4 * i >= 0 && p->x + 4 * i < m->size&&p->y - j >= 0 && p->y - j < m->size&&p->y + 4 * j >= 0 && p->y + 4 * j < m->size)
			{
				if (m->map[p->x - i][p->y - j] == 0 && m->map[p->x + i][p->y + j] == t && m->map[p->x + 2 * i][p->y + 2 * j] == 0 && m->map[p->x + 3 * i][p->y + 3 * j] == t&&m->map[p->x + 4 * i][p->y + 4 * j] == 0) count++; // oXxoxo
				if (m->map[p->x - i][p->y - j] == 0 && m->map[p->x + i][p->y + j] == 0 && m->map[p->x + 2 * i][p->y + 2 * j] == t && m->map[p->x + 3 * i][p->y + 3 * j] == t&&m->map[p->x + 4 * i][p->y + 4 * j] == 0) count++; // oXoxxo
			}
			if (p->x - 2 * i >= 0 && p->x - 2 * i < m->size&&p->x + 2 * i >= 0 && p->x + 2 * i < m->size&&p->y - 2 * j >= 0 && p->y - 2 * j < m->size&&p->y + 2 * j >= 0 && p->y + 2 * j < m->size)
			{
				if (m->map[p->x - 2 * i][p->y - 2 * j] == 0 && m->map[p->x - i][p->y - j] == t && m->map[p->x + i][p->y + j] == t && m->map[p->x + 2 * i][p->y + 2 * j] == 0) count_double++; // oxXxo
			}
			if (p->x - 2 * i >= 0 && p->x - 2 * i < m->size&&p->x + 3 * i >= 0 && p->x + 3 * i < m->size&&p->y - 2 * j >= 0 && p->y - 2 * j < m->size&&p->y + 3 * j >= 0 && p->y + 3 * j < m->size)
			{
				if (m->map[p->x - 2 * i][p->y - 2 * j] == 0 && m->map[p->x - i][p->y - j] == t && m->map[p->x + i][p->y + j] == 0 && m->map[p->x + 2 * i][p->y + 2 * j] == t && m->map[p->x + 3 * i][p->y + 3 * j] == 0) count++; //oxXoxo
			}

		}
	}
	count += (count_double / 2);
	if (count >= 2)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void delTxt(void)
{
	gotoxy(0, map.size * 2 + 3);
	for (int i = 0; i < map.size * 4 + 3; i++) cout << ' ';
	cout << endl;
	for (int i = 0; i <map.size * 4 + 3; i++) cout << ' ';
	cout << endl;
	for (int i = 0; i <map.size * 4 + 3; i++) cout << ' ';
	gotoxy(0, map.size * 2 + 3);
	return;

}
#else
#ifdef LEARNING
int main()
{
	std::string commonString;
	std::cin >> commonString;
	std::ifstream in(commonString);
	while (in.fail())
	{
		std::cin >> commonString;
		in.open(commonString);
	}
	ENTITY entity[SAVED*MULTIPLE];
	char b[400];
	for (int i = 0; i < SAVED; i++)
	{
		
		in.getline(b, 400);
		entity[i*MULTIPLE].setParameter(b);
		for (int j = 1; j < MULTIPLE; j++)
		{
			entity[i * MULTIPLE + j].makeParameter(entity[i].parameter);
		}
	}
	in.close();
	game g;
	while (1)
	{
		for (int i = 0; i < SAVED*MULTIPLE-1; i++)
		{
			for (int j = i + 1; j <  SAVED*MULTIPLE; j++)
			{
				POS p;
				g.clear();
				int ch = 0;
				AI *ai1 = new AI();
				AI *ai2 = new AI();
				ai1->setParameter(entity[i].parameter);
				ai2->setParameter(entity[j].parameter);
				ai1->start();
				p = ai1->nextPosition();
				g.check(p);
				
				while (1)
				{
					ai2->playTurn(p);
					p = ai2->nextPosition();
			
					ch = g.check(p);
					if (ch) break;
					ai1->playTurn(p);
					p = ai1->nextPosition();
					
					ch = g.check(p);
					if (ch) break;
				}
				if (ch == 1)
				{
					entity[i].point += 2;
					std::cout << i << " win" << std::endl;
				}
				else if (ch == -1)
				{
					entity[j].point += 2;
					std::cout << j << " win" << std::endl;
				}
				else if (ch == 3)
				{
					entity[i].point++;
					entity[j].point++;
				}
				delete ai1;
				delete ai2;
				g.clear();

				ai1 = new AI();
				ai2 = new AI();
				ai1->setParameter(entity[i].parameter);
				ai2->setParameter(entity[j].parameter);
				ai2->start();
				p = ai2->nextPosition();
				g.check(p);
				while (1)
				{
					ai1->playTurn(p);
					p = ai1->nextPosition();
					ch = g.check(p);
					if (ch) break;
					ai2->playTurn(p);
					p = ai2->nextPosition();
					ch = g.check(p);
					if (ch) break;
				}
				if (ch == 1)
				{
					std::cout << j << " win" << std::endl;
					entity[j].point += 2;
				}
				else if (ch == -1)
				{
					std::cout << i << " win" << std::endl;
					entity[i].point += 2;
				}
				else if (ch == 3)
				{
					entity[i].point++;
					entity[j].point++;
				}
				delete ai1;
				delete ai2;

			}
		}
		std::sort(entity, entity + SAVED*MULTIPLE, ENTITY::ENTITYcompare);
		time_t rawtime;
		struct tm * timeinfo;
		std::ostringstream setting;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		setting.str(std::string());
		setting << "omok_" << timeinfo->tm_year + 1900 << '-' << std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << '-' << std::setw(2) << std::setfill('0') << timeinfo->tm_mday << '-' << std::setw(2) << std::setfill('0') << timeinfo->tm_hour << '-' << std::setw(2) << std::setfill('0') << timeinfo->tm_min << '-' << std::setw(2) << std::setfill('0') << timeinfo->tm_sec << ".csv";
		std::ofstream *out=new std::ofstream(setting.str());

		for (int i = 0; i <  SAVED; i++)
		{
			entity[i].saveParameter(out);
		}
		out->close();
		delete out;
		std::cout << std::endl << "Game end" << std::endl;
		for (int i = 0; i < SAVED*MULTIPLE; i++)
		{
			std::cout << i << " : " << entity[i].point << "\t";
		}
		std::cout << std::endl << std::endl;

		for (int i = 1; i <  SAVED; i++)
		{
			entity[i*MULTIPLE].setParameter(entity[i].getParameter);
		}
		for (int i = 0; i < SAVED; i++)
		{
			entity[i*MULTIPLE].setParameter(entity[i].getParameter);
			for (int j = 1; j < MULTIPLE; j++)
			{
				entity[i *  MULTIPLE + j].makeParameter(entity[i].parameter);
			}
		}
	}
	
	return 0;
}
#else
void printNext(POS p);
POS enemyTurn();
POS enemyTurn(std::string input);
int check(POS p);

int main()
{
	AI ai;
	POS p;
	std::string input;
	std::cin >> input;

	if (input.compare("START")==0)
	{
		//std::cout << "start" << std::endl;
		ai.start();
		p = ai.nextPosition();
		check(p);
		printNext(p);
	}
	else
	{
		p = enemyTurn(input);
		check(p);
		ai.playTurn(p);
		p = ai.nextPosition();
		check(p);
		printNext(p);
	}
	while (1)
	{
		p = enemyTurn();
		if (check(p)) exit(0);
		ai.playTurn(p);
		p = ai.nextPosition();
		printNext(p);
		if (check(p))exit(0);
	}

	
}
void printNext(POS p)
{
	std::cout << char(p.x + 'A') << p.y + 1<<std::endl;
}

POS enemyTurn()
{
	std::string input;
	POS p;
	std::cin >> input;
	p.x = (input[0] & ~32) - 'A';
	p.y = atoi(input.c_str() + 1) - 1;
	return p;
}
POS enemyTurn(std::string input)
{
	POS p;
	p.x = (input[0] & ~32) - 'A';
	p.y = atoi(input.c_str() + 1) - 1;
	return p;
}
int check(POS p)
{
	static MAP Map;
	static MAP *map=&Map;
	
	static int turn = -1;
	map->map[p.x][p.y] = turn;
	turn *= -1;
	int j, k;

	for (j = 0; j < map->size - 4; j++) // 대각선
	{
		for (k = 0; k < map->size - 4; k++)
		{
			if (map->map[j][k] && map->map[j][k] == map->map[1 + j][1 + k] && map->map[j][k] == map->map[2 + j][2 + k] && map->map[j][k] == map->map[3 + j][3 + k] && map->map[j][k] == map->map[4 + j][4 + k])
			{
				if (j == map->size - 5 && k == 0)
				{
					return map->map[j][k];
				}
				else if (j == 0 && k == map->size - 5)
				{
					return map->map[j][k];
				}
				if (j == 0 || k == 0)
				{
					if (map->map[j][k] != map->map[j + 5][k + 5]) return map->map[j][k];
				}
				else if (j == map->size - 5 || k == map->size - 5)
				{
					if (map->map[j][k] != map->map[j - 1][k - 1]) return map->map[j][k];
				}
				else
				{
					if ((map->map[j][k] != map->map[j + 5][k + 5]) && (map->map[j][k] != map->map[j - 1][k - 1])) return map->map[j][k];

				}
			}
			if (map->map[4 + j][k] && map->map[4 + j][k] == map->map[3 + j][1 + k] && map->map[4 + j][k] == map->map[2 + j][2 + k] && map->map[4 + j][k] == map->map[1 + j][3 + k] && map->map[4 + j][k] == map->map[j][4 + k])
			{
				if (j == 0 && k == 0)
				{
					return map->map[4 + j][k];
				}
				else if (j == map->size - 5 && k == map->size - 5)
				{
					return map->map[4 + j][k];
				}
				else if (j == 0 || k == map->size - 5)
				{
					if (map->map[4 + j][k] != map->map[j + 5][k - 1]) return map->map[4 + j][k];
				}
				else if (j == map->size - 5 || k == 0)
				{
					if (map->map[4 + j][k] != map->map[j - 1][k + 5]) return map->map[4 + j][k];
				}
				else
				{
					if ((map->map[4 + j][k] != map->map[j + 5][k - 1]) && (map->map[4 + j][k] != map->map[j - 1][k + 5])) return map->map[4 + j][k];

				}
			}
		}
	}





	for (j = 0, k = 0; j < map->size; j++, k = 0) // y축 방향
	{
		if (map->map[j][k] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k] && map->map[j][k] != map->map[j][5 + k]) return map->map[j][k];
		for (k = 1; k < map->size - 5; k++)
		{
			if (map->map[j][k] && map->map[j][k] != map->map[j][k - 1] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k] && map->map[j][k] != map->map[j][5 + k]) return map->map[j][k];
		}
		if (map->map[j][k] && map->map[j][k] != map->map[j][k - 1] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k]) return map->map[j][k];
	}
	for (k = 0, j = 0; k < map->size; k++, j = 0) //x축 방향
	{
		if (map->map[j][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k] && map->map[j][k] != map->map[5 + j][k]) return map->map[j][k];
		for (j = 1; j < map->size - 5; j++)
		{
			if (map->map[j][k] && map->map[j][k] != map->map[j - 1][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k] && map->map[j][k] != map->map[5 + j][k]) return map->map[j][k];
		}
		if (map->map[j][k] && map->map[j][k] != map->map[j - 1][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k]) return map->map[j][k];
	}
	for (k = 0; k < map->size; k++)
		for (j = 0; j < map->size; j++)
			if (map->map[k][j] == 0) return 0;

	return 3;
}
#endif // LEARNING

#endif // TEST 


