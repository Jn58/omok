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
void ENTITY::makeParameter(PARAMETER p)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0.0, 2.0);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				double distributionValue = distribution(generator);
				if (distributionValue >= 0)
				{
					distributionValue += 1;
				}
				else
				{
					distributionValue *= -1;
					distributionValue += 1;
					distributionValue = 1 / distributionValue;
				}
				parameter.value[i][j][k] = p.value[i][j][k]* distributionValue;
			}
		}
	}
}
void ENTITY::saveParameter(std::ofstream *out)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				if (!(i == 0 && j == 0 && k == 0))
				{
					*out << ",";
				}
				*out << parameter.value[i][j][k];
			}
		}
	}
	*out << std::endl;
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

bool ENTITY::ENTITYcompare(ENTITY a, ENTITY b)
{
	return a.point > b.point;
}
