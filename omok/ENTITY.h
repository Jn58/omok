#pragma once
#include <string>
#include <random>
#include <chrono>
#include <fstream>
#include "./PARAMETER.h"
class ENTITY
{
public:
	int point = 0;
	PARAMETER parameter;
	ENTITY();
	~ENTITY();

	void ENTITY::setParameter(std::string s);
	void ENTITY::makeParameter(PARAMETER p);
	void saveParameter(std::ofstream * out);
	std::string* ENTITY::StringSplit(std::string strTarget, std::string strTok);
	PARAMETER ENTITY::getParameter(void);
	static bool ENTITY::ENTITYcompare(ENTITY a, ENTITY b);

};