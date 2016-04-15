#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>

#define MAX_STEP 2

using namespace std;

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

class calc;

struct POS
{
	int x, y;
};

void gotoxy(int x, int y);

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
	void work()
	{

		makeChild(step + 1);
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
	void makeChild(int c);
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

		while (!temp_Q.empty())
		{
			POS temp_pos = temp_Q.front();
			temp_Q.pop();
			for (int i = 1; i < 5; i++)
			{
				if (temp_pos.y - i >= 0)
				{
					if (map->map[temp_pos.x][temp_pos.y - i] == 0)  posQ.push({ temp_pos.x,temp_pos.y - i });
				}
				if (temp_pos.y + i < map->size)
				{
					if (map->map[temp_pos.x][temp_pos.y + i] == 0)  posQ.push({ temp_pos.x,temp_pos.y + i });
				}

				if (temp_pos.x + i < map->size)
				{
					if (map->map[temp_pos.x + i][temp_pos.y] == 0)  posQ.push({ temp_pos.x + i,temp_pos.y });

					if (temp_pos.y - i >= 0)
					{
						if (map->map[temp_pos.x + i][temp_pos.y - i] == 0)  posQ.push({ temp_pos.x + i,temp_pos.y - i });
					}
					if (temp_pos.y + i < map->size)
					{
						if (map->map[temp_pos.x + i][temp_pos.y + i] == 0)  posQ.push({ temp_pos.x + i,temp_pos.y + i });
					}
				}
				if (temp_pos.x - i >= 0)
				{

					if (map->map[temp_pos.x - i][temp_pos.y] == 0)  posQ.push({ temp_pos.x - i,temp_pos.y });
					if (temp_pos.y - i >= 0)
					{
						if (map->map[temp_pos.x - i][temp_pos.y - i] == 0)  posQ.push({ temp_pos.x - i,temp_pos.y - i });
					}
					if (temp_pos.y + i < map->size)
					{
						if (map->map[temp_pos.x - i][temp_pos.y + i] == 0)  posQ.push({ temp_pos.x - i,temp_pos.y + i });
					}
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
			child.front()->child.sort();
			return child.front()->child.front()->pos;
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

queue<calc*> toDo;

void calc::makeChild(int c)
{
	if (c > MAX_STEP) return;
	possiblePosition();
	while (!posQ.empty())
	{
		POS childPos = posQ.front();
		posQ.pop();
		calc* child = new calc(this, *map, childPos, turn*-1, step + 1);
		toDo.push(child);
		this->child.push_back(child);
	}

}


int check(MAP *map)
{
	for (int j = 0; j < map->size - 4; j++)
	{
		for (int k = 0; k < map->size - 4; k++)
		{
			if (map->map[j][k] == map->map[1+j][1+k] && map->map[j][k] == map->map[2+j][2+k] && map->map[j][k] == map->map[3+j][3+k] && map->map[j][k] == map->map[4+j][4+k]) return map->map[j][k];
			if (map->map[4+j][k] == map->map[3+j][1+k] && map->map[4+j][k] == map->map[2+j][2+k] && map->map[4+j][k] == map->map[1+j][3+k] && map->map[4+j][k] == map->map[j][4+k]) return map->map[4+j][k];
			for (int i = 0; i < map->size; i++)
			{
				if (map->map[i+j][k] == map->map[i+j][1+k] && map->map[i+j][k] == map->map[i+j][2+k] && map->map[i+j][k] == map->map[i+j][3+k] && map->map[i+j][k] == map->map[i+j][4+k]) return map->map[i+j][k];
				if (map->map[j][i+k] == map->map[1+j][i+k] && map->map[j][i+k] == map->map[2+j][i+k] && map->map[j][i+k] == map->map[3+j][i+k] && map->map[j][i+k] == map->map[4+j][i+k]) return map->map[j][i+k];

			}
		}
	}
	
	return 0;
}
POS human(MAP map)
{
	return{ 2,2 };
}

MAP map;
int main(void)
{

	POS p;
	map.init_map(15);
	char * setting = new char[100];
	sprintf(setting, "mode con:cols=%d lines=%d", map.size * 4 + 4, map.size * 2 + 4);
	system(setting);
	setcursortype(NOCURSOR);
	initMap(map.size);
	while (1)
	{
		int ch;
		calc *ai = new calc(map, -1, 0);
		gotoxy(0, map.size * 2);
		cout << "연산중...                " << endl;
		ai->work();
		while (!toDo.empty())
		{
			calc *temp = toDo.front();
			toDo.pop();
			temp->work();
		}
		p = ai->nextPOS();
		print(p, 1);
		map.map[p.x][p.y] = 1;
		delete ai;
		ch = check(&map);
		if (ch)
		{
			gotoxy(0, map.size * 2 + 1);
			if (ch == 1)
			{
				cout << "AI win" << endl;
				break;
			}
			else
			{
				cout << "Human win" << endl;
				break;
			}
		}
		do
		{
			p = player();
			if (map.map[p.x][p.y] != 0)continue;
		} while (0);

		map.map[p.x][p.y] = -1;
		print(p, -1);
		ch = check(&map);
		if (ch)
		{
			gotoxy(0, map.size * 2 + 1);
			if (ch == 1)
			{

				cout << "AI win" << endl;
				break;
			}
			else
			{

				cout << "Human win" << endl;
				break;
			}
		}
	}
	system("pause");
	return 0;
}



void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
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
	setcolor(0, 14);
	printf("┌─");
	for (int i = 0; i < size - 2; i++) printf("┬─");
	printf("┐\n");
	setcolor(15, 0);
	printf("   ");
	setcolor(0, 14);
	printf("│");
	for (int i = 0; i < size - 1; i++) printf("　│");
	printf("\n");
	for (int j = 0; j < size - 2; j++)
	{
		setcolor(15, 0);
		printf("%2d ", j + 2);
		setcolor(0, 14);
		printf("├─");
		for (int i = 0; i < size - 2; i++) printf("┼─");

		printf("┤\n");
		setcolor(15, 0);
		printf("   ");
		setcolor(0, 14);
		printf("│");
		for (int i = 0; i < size - 1; i++) printf("　│");
		printf("\n");
	}
	setcolor(15, 0);
	printf("%2d ", size);
	setcolor(0, 14);
	printf("└─");
	for (int i = 0; i < size - 2; i++) printf("┴─");
	printf("┘\n");
	setcolor(15, 0);

}
void print(POS p, int t)
{

	playpos(p.x, p.y);
	if (t == 1)
	{
		setcolor(15, 14);
		printf("●");
	}
	else
	{
		setcolor(0, 14);
		printf("●");
	}

	setcolor(15, 0);
}

POS player(void)
{
	POS p;
	gotoxy(0, map.size * 2);
	printf("놓을곳을입력하세요:  \b\b");
	char input[100];
	scanf("%s", input);
	p.x = (input[0] & ~32) - 'A';
	p.y = input[1] - '1';
	return p;
}

bool PComp(const calc * const & a, const calc * const & b)
{
	return a->point > b->point;
}