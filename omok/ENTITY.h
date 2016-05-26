#pragma once
#include <string>
#include "PARAMETER.h"
class ENTITY
{
public:
	int point = 0;
	PARAMETER parameter;
	ENTITY();
	~ENTITY();

	void ENTITY::setParameter(std::string s);
	std::string* ENTITY::StringSplit(std::string strTarget, std::string strTok);
	PARAMETER ENTITY::getParameter(void);

};