#include "PARAMETER.h"



PARAMETER::PARAMETER()
{
	
}


PARAMETER::~PARAMETER()
{
}

void PARAMETER::operator=(const PARAMETER & p)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				value[i][j][k] = p.value[i][j][k];
			}
		}
	}
}
