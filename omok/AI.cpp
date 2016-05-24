#include "AI.h"


#define MAX_STEP 2
#define THREAD int((std::thread::hardware_concurrency()*3)/2)
#define SINGLE


AI::AI()
{
	toDoMutex = new std::mutex();
	toDo = new std::stack<calc *>;
	std::thread ** thread = new std::thread*[THREAD];

}

AI::~AI()
{
	delete thread;
	delete turn;
	delete toDoMutex;
	delete toDo;
}

void AI::start(void)
{
	turn = new calc();
	this->calculate();
}

calc * AI::availablePlay(POS position)
{
	calc * nextTurn = NULL;
	if (turn != NULL)
		while (!turn->child.empty())
		{
			calc * pop = turn->child.front();
			turn->child.pop_front();
			if (pop->position.x == position.x && pop->position.y == position.y)
			{
				nextTurn = pop;
			}
			else
			{
				delete pop;
			}
		}
	return nextTurn;
}

void AI::playTurn(POS position)
{
	calc * nextTurn = availablePlay(position);

	if (nextTurn!=NULL)
	{
		delete turn;
		turn = nextTurn;
	}
	else
	{
		if (turn == NULL)
		{
			turn = new calc();
		}		
		turn = turn->enemyPlay(position);
		
		
	}
	turn->resetStep(0);
	this->calculate();
}

POS AI::nextPosition(void)
{
	calc * nextTurn;
	int temp;
	turn->child.sort(calc::compare);
	temp = 0;
	nextTurn = turn->child.front();
	turn->child.pop_front();
	delete turn;
	turn = nextTurn;
	return nextTurn->position;
}
#ifndef SINGLE
void AI::calculate()
{
	AI::turn->makeChild(toDoMutex, toDo);
	for (int i = 0; i < THREAD; i++)
	{
		thread[i] = new std::thread(&threadWork, toDo, toDoMutex);

	}
	for (int i = 0; i < THREAD; i++)
	{
		thread[i]->join();
	}
	for (int i = 0; i < THREAD; i++)
	{
		delete thread[i];
	}
}
#else
void AI::calculate(void)
{
	AI::turn->makeChild(toDoMutex, toDo);
	while (1)
	{
		toDoMutex->lock();
		if (toDo->empty())break;
		calc *temp = toDo->top();
		toDo->pop();
		toDoMutex->unlock();
		temp->work(toDoMutex, toDo);
	}
	toDoMutex->unlock();
}
#endif // !SINGLE




void AI::threadWork(std::stack<calc *> * toDo, std::mutex * toDoMutex)
{
	while (1)
	{
		toDoMutex->lock();
		if (toDo->empty())break;
		calc *temp = toDo->top();
		toDo->pop();
		toDoMutex->unlock();
		temp->work(toDoMutex, toDo);
	}
	toDoMutex->unlock();
}


calc::calc()
{
	this->parent = NULL;
	this->map = new MAP();
	this->turn = -1;
	this->point = 0;
	this->step = 0;
}
calc::calc(MAP *  map, int  turn, int step, POS position)
{
	this->map = new MAP(map);
	this->turn = turn;
	this->step = step;
	this->parent = NULL;
	this->position.x = position.x;
	this->position.y = position.y;
	this->map->map[position.x][position.y] = turn;
}
calc::calc(MAP *  map,  int  turn,  int step,  POS position ,calc  *parent )
{
	this->map = new MAP(map);
	this->turn = turn;
	this->step = step;
	this->parent = parent;
	this->position.x = position.x;
	this->position.y = position.y;
	this->map->map[position.x][position.y] = turn;
}

calc::calc(MAP * map, POS position)
{
	this->map = new MAP(map);
	this->turn = turn;
	this->step = step;
	this->parent = NULL;
	this->position.x = position.x;
	this->position.y = position.y;
	this->map->map[position.x][position.y] = -1;
}

calc::~calc()
{
	{
		while (!child.empty())
		{
			calc* temp = child.front();
			child.pop_front();
			delete temp;
		}

	}
	delete map;
}

bool calc::operator<(const calc & other)
{
	return this->point > other.point;
}

bool calc::compare(const calc * const & a, const calc * const & b)
{
	return a->point > b->point;
}

bool calc::rule(MAP * m, POS * p, int t)
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

void calc::updatePoint(double add)
{
	{
		this->mutex.lock();
		point += add;
		this->mutex.unlock();
		if (parent) parent->updatePoint(add);
	}
}

void calc::evaluatePoint(void)
{
	int pi[5] = { 0, };
	double sum = 0;
	int count = 0;
	for (int i = 0,j; i < 5; i++)
	{
		if (this->position.x - 4 + i < 0) continue;
		if (this->position.x + i >= map->size) break;
		for ( j = 0, count = 0; j < 5; j++)
		{
			if (map->map[this->position.x - 4 + i + j][this->position.y] == turn*-1) break;
			if (map->map[this->position.x - 4 + i + j][this->position.y] == turn) count++;
		}
		if (j == 5)
			pi[count - 1]++;
	}
	for (int i = 0, j = 0; i < 5; i++)
	{
		if (this->position.y - 4 + i < 0) continue;
		if (this->position.y + i >= map->size) break;
		for (j = 0, count = 0; j < 5; j++)
		{
			if (map->map[this->position.x][this->position.y - 4 + i + j] == turn*-1) break;
			if (map->map[this->position.x][this->position.y - 4 + i + j] == turn) count++;
		}
		if (j == 5)
			pi[count - 1]++;
	}
	for (int i = 0, j = 0; i < 5; i++)
	{
		if (this->position.x - 4 + i < 0 || this->position.y - 4 + i < 0) continue;
		if (this->position.x + i >= map->size || this->position.y + i >= map->size) break;
		for (j = 0, count = 0; j < 5; j++)
		{
			if (map->map[this->position.x - 4 + i + j][this->position.y - 4 + i + j] == turn*-1) break;
			if (map->map[this->position.x - 4 + i + j][calc::position.y - 4 + i + j] == turn) count++;
		}
		if (j == 5)
			pi[count - 1]++;
	}
	for (int i = 0, j = 0; i < 5; i++)
	{
		if (calc::position.x - 4 + i < 0 || calc::position.y + 4 - i >= map->size) continue;
		if (calc::position.x + i >= map->size || calc::position.y - i < 0) break;
		for (j = 0, count = 0; j < 5; j++)
		{
			if (map->map[calc::position.x - 4 + i + j][calc::position.y + 4 - i - j] == turn*-1) break;
			if (map->map[calc::position.x - 4 + i + j][calc::position.y + 4 - i - j] == turn) count++;
		}
		if (j == 5)
			pi[count - 1]++;
	}
	for (int i = 0; i < 5; i++)
	{
		sum += double(pow(1000, i)*pi[i]);
	}
	updatePoint(sum*turn / (pow(step, 4)));
}

void calc::possiblePosition(std::queue<POS> * posQ)
{
	std::queue<POS> temp_Q;
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
		posQ->push(temp);
		return;
	}
	MAP temp_map;
	temp_map.makeMap(map->size);
	temp_map.clearMap();

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
					if (map->map[temp_pos.x - i][temp_pos.y - i] == 0)  temp_map.map[temp_pos.x - i][temp_pos.y - i] = 1;
				}
				if (temp_pos.y + i < map->size)
				{
					if (map->map[temp_pos.x - i][temp_pos.y + i] == 0)  temp_map.map[temp_pos.x - i][temp_pos.y + i] = 1;
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
				posQ->push(temp);
			}
		}
	}
}

void calc::makeChild(std::mutex * toDoMutex, std::stack<calc *> * toDo)
{
	if (step == MAX_STEP) return;
	std::queue<POS> *posQ = new std::queue<POS>;
	possiblePosition(posQ);
	while (!posQ->empty())
	{
		POS childPos = posQ->front();
		posQ->pop();
		if (rule(map, &childPos, turn*-1)) continue;
		calc* child = new calc(map, turn*-1, step + 1, childPos,this );
		toDoMutex->lock();
		toDo->push(child);
		toDoMutex->unlock();
		this->child.push_back(child);
	}
	delete posQ;
}

void calc::work(std::mutex * toDoMutex, std::stack<calc *> * toDo)
{

	if (child.empty())
	{
		calc::evaluatePoint();
		calc::makeChild(toDoMutex, toDo);
	}
	else
	{
		toDoMutex->lock();
		for (std::list<calc *>::iterator i = child.begin(); i != child.end(); i++)
		{
			toDo->push((*i));
		}
		toDoMutex->unlock();
	}

}



calc * calc::enemyPlay(POS position)
{
	calc * nextTurn = new calc(map, position);
	delete this;
	return nextTurn;
}

void calc::resetStep(int step)
{
	calc::step = step++;
	if (!child.empty())
	{
		for (std::list<calc *>::iterator i = child.begin(); i != child.end(); i++)
		{
			(*i)->resetStep(step);
		}
	}
}