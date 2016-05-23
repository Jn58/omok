#pragma once
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <mutex>
#include "MAP.h"
struct POS
{
	int x,y;
};
class calc
{
public:
	calc* parent = NULL;
	std::list<calc*> child;
	MAP * map = NULL;
	int turn = 0;
	POS position = { 0,0 };
	double point = 0;
	int step = 0;
	std::mutex mutex;

	calc();
	calc(MAP * const map, const int  turn, const int step, calc  *parent, POS position);
	~calc();
	bool operator<(const calc & other);
	static bool compare(const calc * const & a, const calc * const & b);
	bool rule(MAP * m, POS * p, int t);
	void updatePoint(double add);
	void evaluatePoint(void);
	void possiblePosition(POS * posQ);
	void makeChild(std::mutex * toDoMutex, std::stack<calc *> * toDo);
	void work(std::mutex * toDoMutex, std::stack<calc *> * toDo);


};

class AI
{
public:
	calc * turn = NULL;
	std::stack<calc *> * toDo;
	std::mutex * toDoMutex;
	AI();
	~AI();
	void start(void);
	calc * availablePlay(POS position);
	void playTurn(POS position);
	POS nextPosition(void);

};


