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

	calc::calc();
	calc::calc(int step);
	calc::calc(MAP *  map, int  turn, int step, POS position);
	calc::calc(MAP *  map,  int  turn,  int step, POS position, calc  *parent );

	calc::calc(MAP  *map, POS position);
	calc::~calc();
	bool calc::operator<(const calc & other);
	static bool calc::compare(const calc * const & a, const calc * const & b);
	bool calc::rule(MAP * m, POS * p, int t);
	void calc::updatePoint(double add);
	void calc::evaluatePoint(void);
	void calc::possiblePosition(std::queue<POS> * posQ);
	void calc::makeChild(std::mutex * toDoMutex, std::stack<calc *> * toDo);
	void calc::work(std::mutex * toDoMutex, std::stack<calc *> * toDo);
	calc * calc::enemyPlay(POS position);
	void calc::resetStep(int step);
	void calc::place(void);
	


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
	void AI::start(void);
	calc * AI::availablePlay(POS position);
	void AI::playTurn(POS position);
	POS AI::nextPosition(void);
	void AI::calculate(void);
	static void AI::threadWork(std::stack<calc *> * toDo, std::mutex * toDoMutex);
	void AI::reduceChild(int num);

};


