#include "ENTITY.h"



ENTITY::ENTITY()
{
}


ENTITY::~ENTITY()
{
}

void ENTITY::setParameter(std::string s)
{
	std::string * splitString = StringSplit(s, ",");
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				parameter.value[i][j][k]= std::stod( splitString[i * 6 + j * 2 + k]);
			}
		}
	}
}
std::string* ENTITY::StringSplit(std::string strTarget, std::string strTok)
{
	int nCutPos;
	int nIndex = 0;
	std::string* strResult = new std::string[50];

	while ((nCutPos = strTarget.find_first_of(strTok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			strResult[nIndex++] = strTarget.substr(0, nCutPos);
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		strResult[nIndex++] = strTarget.substr(0, nCutPos);
	}

	return strResult;
}
PARAMETER ENTITY::getParameter(void)
{
	return parameter;
}