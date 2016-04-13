#include <iostream>
#include <queue>
#include <stack>
#include <list>

using namespace std;
struct POS
{
	int x, y;
};
class MAP
{
public:
	int ** map;
	int size;
	void init_map(int n)
	{
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
	int left = 0;
	std::queue<POS> posQ;
	bool operator<(const calc & other)
	{
		return point > other.point;
	};
	calc(MAP m, int t, int l)
	{
		map = new MAP(m);
		turn = t;
		left = l;
	}
	calc(calc  *par, MAP m, POS p, int t, int l)
	{
		parent = par;
		map = new MAP(m);
		pos = p;
		turn = t;
		left = l;
		map->map[p.x][p.y] = t;
		evaluatePoint();
		//map->print();
	}
	void work()
	{

		makeChild(left - 1);
	}
	void updatePoint(int add)
	{
		point += add;
		if (parent) parent->updatePoint(add);
	}
	void evaluatePoint(void)
	{
		int pi[5] = { 0, };
		int sum = 0;
		int count = 0;
		for (int i = 0, j = 0; i <5; i++)
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
		for (int i = 0; i < 5; i++)
		{
			sum += (i + 1)*(i + 1)*pi[i];
		}
		updatePoint(sum*turn*(1 + left));
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
			POS temp = { (1 + map->size) / 2,(1 + map->size) };
			posQ.push(temp);
			return;
		}

		MAP temp_map;
		temp_map.init_map(map->size);

		while (!temp_Q.empty())
		{
			POS temp_pos = temp_Q.front();
			temp_Q.pop();
			for (int i = 1; i <= 5; i++)
			{
				if (temp_pos.y - i >= 1)
				{
					if (map->map[temp_pos.x][temp_pos.y - i] == 0)  posQ.push({ temp_pos.x,temp_pos.y - i });
				}
				else if (temp_pos.y + i <= map->size)
				{
					if (map->map[temp_pos.x][temp_pos.y + i] == 0)  posQ.push({ temp_pos.x,temp_pos.y + i });
				}
				if (temp_pos.x + i <= map->size)
				{
					if (map->map[temp_pos.x + i][temp_pos.y] == 0)  posQ.push({ temp_pos.x + i,temp_pos.y });
					if (temp_pos.y - i >= 1)
					{
						if (map->map[temp_pos.x + i][temp_pos.y - i] == 0)  posQ.push({ temp_pos.x + i,temp_pos.y - i });
					}
					else if (temp_pos.y + i <= map->size)
					{
						if (map->map[temp_pos.x + i][temp_pos.y + i] == 0)  posQ.push({ temp_pos.x + i,temp_pos.y + i });
					}
				}
				else if (temp_pos.x - i >= 1)
				{

					if (map->map[temp_pos.x - i][temp_pos.y] == 0)  posQ.push({ temp_pos.x + i,temp_pos.y });
					if (temp_pos.y - i >= 1)
					{
						if (map->map[temp_pos.x - i][temp_pos.y - i] == 0)  posQ.push({ temp_pos.x - i,temp_pos.y - i });
					}
					else if (temp_pos.y + i <= map->size)
					{
						if (map->map[temp_pos.x - i][temp_pos.y + i] == 0)  posQ.push({ temp_pos.x - i,temp_pos.y + i });
					}
				}






			}
		}

	}
	POS nextPOS(void)
	{
		child.sort();
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
		delete map;
	}



};

queue<calc*> toDo;

void calc::makeChild(int c)
{
	if (c == 0) return;
	possiblePosition();
	while (!posQ.empty())
	{
		POS childPos = posQ.front();
		posQ.pop();
		calc* child = new calc(this, *map, childPos, turn*-1, left - 1);
		toDo.push(child);
		this->child.push_back(child);
	}
}

int main(void)
{
	MAP map;
	POS p = { -1,-1 };
	map.init_map(5);
	calc *ai = new calc(map, -1, 5);
	ai->work();
	while (!toDo.empty())
	{
		calc *temp = toDo.front();
		toDo.pop();
		temp->work();
	}
	p = ai->nextPOS();
	cout << p.x << p.y << endl;
	delete ai;
	return 0;
}