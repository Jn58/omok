#pragma once
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <mutex>
#include <thread>
#include "MAP.h"
struct POS
{
	int x,y;
};
class calc
{
public:
	calc* parent;
	std::list<calc*> child;
	MAP * map;
	int turn;
	POS position;
	double point=0;
	int step;
	std::mutex mutex;

	calc();
	calc(MAP *  map, int  turn, int step, POS position);
	calc(MAP *  map,  int  turn,  int step, POS position, calc  *parent );

	calc(MAP  *map, POS position);
	~calc();
	bool operator<(const calc & other);
	static bool compare(const calc * const & a, const calc * const & b);
	bool rule(MAP * m, POS * p, int t);
	void updatePoint(double add);
	void evaluatePoint(void);
	void possiblePosition(std::queue<POS> * posQ);
	void makeChild(std::mutex * toDoMutex, std::stack<calc *> * toDo);
	void work(std::mutex * toDoMutex, std::stack<calc *> * toDo);
	calc * enemyPlay(POS position);
	void resetStep(int step);
	void place(void);


};

class AI
{
public:
	calc * turn = NULL;
	std::stack<calc *> * toDo;
	std::mutex * toDoMutex;
	std::thread ** thread;
	AI();
	~AI();
	void start(void);
	calc * availablePlay(POS position);
	void playTurn(POS position);
	POS nextPosition(void);
	void calculate(void);
	static void threadWork(std::stack<calc *> * toDo, std::mutex * toDoMutex);

};


