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
#include "gotoxy.h"
#include "timer.h"


#define MAX_STEP 2
#define THREAD int(thread::hardware_concurrency())+1
#define MAP_SIZE 15

using namespace std;

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

class calc;
mutex mut;



struct POS
{
	int x, y;
};


void setcursortype(CURSOR_TYPE c);

void playpos(int x, int y);

void setcolor(int color, int bgcolor);

void initMap(int size);
void print(POS p, int t);
POS player(void);
bool PComp(const calc * const & a, const calc * const & b);


class MAP
{
public:
	int ** map;
	int size = 0;
	void init_map(int n)
	{
		this->~MAP();
		size = n;
		map = new int*[size];
		for (int i = 0; i < size; i++)
		{
			map[i] = new int[size];
		}
		
	}
	void del_map(void)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				map[i][j] = 0;
	}
	MAP()
	{

	}
	MAP(const MAP &parent)
	{
		init_map(parent.size);
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				map[i][j] = parent.map[i][j];

	}
	MAP(MAP const * const parent)
	{
		init_map(parent->size);
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				map[i][j] = parent->map[i][j];

	}
	void print()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (map[j][i]) printf("%c", (map[j][i] == 1) ? 'o' : 'x');
				else printf("+");
			}
			printf("\n");

		}

		printf("\n\n");

	}
	~MAP()
	{
		if (size != 0)
		{
			for (int i = 0; i < size; i++)
			{
				delete map[i];
			}
			delete map;
		}

	}
};
bool rule(MAP * m, POS * p, int t);
class calc
{
public:
	calc* parent = NULL;
	list<calc*> child;
	MAP * map;
	int turn;
	POS pos;
	double point = 0;
	int step = 0;
	std::queue<POS> posQ;
	bool operator<(const calc & other)
	{
		return point < other.point;
	};
	bool compare_pointer(const calc* const & first, const calc * const & second)
	{
		return first->point > second->point;
	}
	calc(MAP m, int t, int s)
	{
		map = new MAP(m);
		turn = t;
		step = s;
	}
	calc(calc  *par, MAP m, POS p, int t, int s)
	{
		parent = par;
		map = new MAP(m);
		pos = p;
		turn = t;
		step = s;
		map->map[p.x][p.y] = t;
		evaluatePoint();
		//map->print();
	}
	calc(const MAP * const m,  int  t, const int s)
	{
		map = new MAP(m);
		turn = t;
		step = s;
	}
	calc(calc  *par, const MAP * const m, POS p, int t, int s)
	{
		parent = par;
		map = new MAP(m);
		pos = p;
		turn = t;
		step = s;
		map->map[p.x][p.y] = t;
		evaluatePoint();
		//map->print();
	}
	void work(mutex * m)
	{

		makeChild(step + 1,m);
		delete map;
	}
	void updatePoint(double add)
	{
		point += add;
		if (parent) parent->updatePoint(add);
	}
	void evaluatePoint(void)
	{
		int pi[5] = { 0, };
		double sum = 0;
		int count = 0;
		for (int i = 0, j = 0; i < 5; i++)
		{
			if (pos.x - 4 + i < 0) continue;
			if (pos.x + i >= map->size) break;
			for (j = 0, count = 0; j < 5; j++)
			{
				if (map->map[pos.x - 4 + i + j][pos.y] == turn*-1) break;
				if (map->map[pos.x - 4 + i + j][pos.y] == turn) count++;
			}
			if (j == 5)
				pi[count - 1]++;
		}
		for (int i = 0, j = 0; i < 5; i++)
		{
			if (pos.y - 4 + i < 0) continue;
			if (pos.y + i >= map->size) break;
			for (j = 0, count = 0; j < 5; j++)
			{
				if (map->map[pos.x][pos.y - 4 + i + j] == turn*-1) break;
				if (map->map[pos.x][pos.y - 4 + i + j] == turn) count++;
			}
			if (j == 5)
				pi[count - 1]++;
		}
		for (int i = 0, j = 0; i < 5; i++)
		{
			if (pos.x - 4 + i < 0 || pos.y - 4 + i < 0) continue;
			if (pos.x + i >= map->size || pos.y + i >= map->size) break;
			for (j = 0, count = 0; j < 5; j++)
			{
				if (map->map[pos.x - 4 + i + j][pos.y - 4 + i + j] == turn*-1) break;
				if (map->map[pos.x - 4 + i + j][pos.y - 4 + i + j] == turn) count++;
			}
			if (j == 5)
				pi[count - 1]++;
		}
		for (int i = 0, j = 0; i < 5; i++)
		{
			if (pos.x - 4 + i < 0 || pos.y + 4 - i >= map->size) continue;
			if (pos.x + i >= map->size || pos.y - i < 0) break;
			for (j = 0, count = 0; j < 5; j++)
			{
				if (map->map[pos.x - 4 + i + j][pos.y + 4 - i - j] == turn*-1) break;
				if (map->map[pos.x - 4 + i + j][pos.y + 4 - i - j] == turn) count++;
			}
			if (j == 5)
				pi[count - 1]++;
		}
		for (int i = 0; i < 5; i++)
		{
			sum += pow(1000, i)*pi[i];
		}
		updatePoint(sum*turn / (pow(step, 4)));
	}
	void makeChild(int c,mutex * m);
	void possiblePosition(void)
	{
		queue<POS> temp_Q;
		for (int i = 0; i < map->size; i++) {
			for (int j = 0; j < map->size; j++)
			{
				if (map->map[i][j] != 0)
				{
					POS temp = { i,j };
					temp_Q.push(temp);
				}
			}
		}

		if (temp_Q.empty())
		{
			POS temp = { map->size / 2, map->size / 2 };
			posQ.push(temp);
			return;
		}
		MAP temp_map;
		temp_map.init_map(map->size);
		temp_map.del_map();
		
		while (!temp_Q.empty())
		{
			POS temp_pos = temp_Q.front();
			temp_Q.pop();
			for (int i = 1; i < 5; i++)
			{
				if (temp_pos.y - i >= 0)
				{
					if (map->map[temp_pos.x][temp_pos.y - i] == 0)  temp_map.map[temp_pos.x][temp_pos.y - i] = 1;
				}
				if (temp_pos.y + i < map->size)
				{
					if (map->map[temp_pos.x][temp_pos.y + i] == 0)  temp_map.map[temp_pos.x][temp_pos.y + i] = 1;
				}

				if (temp_pos.x + i < map->size)
				{
					if (map->map[temp_pos.x + i][temp_pos.y] == 0) temp_map.map[temp_pos.x + i][temp_pos.y] = 1; 

					if (temp_pos.y - i >= 0)
					{
						if (map->map[temp_pos.x + i][temp_pos.y - i] == 0)  temp_map.map[temp_pos.x + i][temp_pos.y - i] = 1; 
					}
					if (temp_pos.y + i < map->size)
					{
						if (map->map[temp_pos.x + i][temp_pos.y + i] == 0)  temp_map.map[temp_pos.x + i][temp_pos.y + i] = 1;
					}
				}
				if (temp_pos.x - i >= 0)
				{

					if (map->map[temp_pos.x - i][temp_pos.y] == 0)  temp_map.map[temp_pos.x - i][temp_pos.y] = 1;
					if (temp_pos.y - i >= 0)
					{
						if (map->map[temp_pos.x - i][temp_pos.y - i] == 0)  temp_map.map[temp_pos.x - i][temp_pos.y-i] = 1;
					}
					if (temp_pos.y + i < map->size)
					{
						if (map->map[temp_pos.x - i][temp_pos.y + i] == 0)  temp_map.map[temp_pos.x - i][temp_pos.y+i] = 1;
					}
				}
			}
		}
		for (int i = 0; i < map->size; i++) {
			for (int j = 0; j < map->size; j++)
			{
				if (temp_map.map[i][j] == 1)
				{
					POS temp = { i,j };
					posQ.push(temp);
				}
			}
		}
	}
	POS nextPOS(void)
	{
		child.sort(PComp);
		if (turn == -1)
		{

			return child.front()->pos;
		}
		else
		{

			return child.back()->pos;
		}

	}
	~calc()
	{
		while (!child.empty())
		{
			calc* temp = child.front();
			child.pop_front();
			delete temp;
		}

	}

};

stack<calc*> toDo;

void calc::makeChild(int c,mutex * m)
{
	if (c > MAX_STEP) return;
	possiblePosition();
	while (!posQ.empty())
	{
		POS childPos = posQ.front();
		posQ.pop();
		if (rule(map, &childPos, turn)) continue;
		calc* child = new calc(this, map, childPos, turn*-1, step + 1);
		m->lock();
		toDo.push(child);
		m->unlock();
		this->child.push_back(child);
	}

}


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

void threadWork(stack<calc*>* toDo,mutex * m);

void delTxt(void);


MAP map;



int main(void)
{

	POS p;
	ostringstream setting;
	TIMER *timer;

	thread ** devideWork = new thread*[THREAD];
	map.init_map(MAP_SIZE);
	map.del_map();
	setting << "mode con:cols=" << map.size * 4 + 16 << " lines=" << map.size * 2 + 4;
	
	system(setting.str().c_str());
	setcursortype(NOCURSOR);
	initMap(map.size);
	timer = new TIMER(map.size);
	while (1)
	{
		int ch;
		calc *ai = new calc(&map, -1, 0);

		delTxt();
		cout << "AI is thinking..." << endl;
		timer->start();
		ai->work(&mut);
		
		for (int i = 0; i < THREAD; i++)
		{
			devideWork[i] = new thread(&threadWork, &toDo, &mut);

		}
		for (int i = 0; i < THREAD; i++)
		{
			devideWork[i]->join();
		}
		for (int i = 0; i < THREAD; i++)
		{
			delete devideWork[i];
		}
		
		timer->end();
		p = ai->nextPOS();
		print(p, 1);

		map.map[p.x][p.y] = 1;
		delete ai;
		timer->print_time();
		if (timer->time() >= 1) exit(0);
		
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
	}
	system("pause");
	return 0;
}




void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;
	switch (c)
	{
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
void playpos(int x, int y)
{
	gotoxy(4 * x + 3, y * 2 + 1);
}
void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}
void initMap(int size)
{
	setcolor(15, 0);
	printf("   A ");
	for (int i = 0; i < size - 1; i++)
	{
		printf("  %c ", i + 'B');
	}
	printf("\n");
	printf(" 1 ");
	setcolor(0, 6);
	printf("┌─");
	for (int i = 0; i < size - 2; i++) printf("┬─");
	printf("┐");
	setcolor(15, 0);
	cout << 1 << endl;
	printf("   ");
	setcolor(0, 6);
	printf("│");
	for (int i = 0; i < size - 1; i++) printf("　│");
	printf("\n");
	for (int j = 0; j < size - 2; j++)
	{
		setcolor(15, 0);
		printf("%2d ", j + 2);
		setcolor(0, 6);
		printf("├─");
		for (int i = 0; i < size - 2; i++) printf("┼─");

		printf("┤");
		setcolor(15, 0);
		cout << j + 2<<endl;
		printf("   ");
		setcolor(0, 6);
		printf("│");
		for (int i = 0; i < size - 1; i++) printf("　│");
		printf("\n");
	}
	setcolor(15, 0);
	printf("%2d ", size);
	setcolor(0, 6);
	printf("└─");
	for (int i = 0; i < size - 2; i++) printf("┴─");
	printf("┘");
	setcolor(15, 0);
	cout << size << endl;
	setcolor(15, 0);
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
		setcolor(0, 6);
		printf("●");
	}
	else
	{
		setcolor(15, 6);
		printf("●");
	}

	setcolor(15, 0);
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
		if (rule(&map, &p, -1) || p.x < 0 || p.x >= map.size || p.y < 0 || p.y >= map.size|| map.map[p.x][p.y] != 0)
		{
			delTxt();
			cout << "You can not put there.\nRetry." << endl;
			continue;
		}
		return p;
	}

}

bool PComp(const calc * const & a, const calc * const & b)
{
	return a->point > b->point;
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
	count += count_double / 2;
	if (count >= 2) return true;
	return false;

}
void threadWork(stack<calc*>* toDo, mutex * m)
{
	while (1)
	{
		m->lock();
		if (toDo->empty())break;
		calc *temp = toDo->top();
		toDo->pop();
		m->unlock();
		if (temp) temp->work(m);
	}
	m->unlock();
}

void delTxt(void)
{
	gotoxy(0, map.size * 2+1);
	for (int i = 0; i < map.size * 4 + 3; i++) cout << ' ';
	cout << endl;
	for (int i = 0; i <map.size * 4 + 3; i++) cout << ' ';
	cout << endl;
	for (int i = 0; i <map.size * 4 + 3; i++) cout << ' ';
	gotoxy(0, map.size * 2+1);
	return;

}

