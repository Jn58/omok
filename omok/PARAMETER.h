#pragma once
class PARAMETER
{
public:
	
	double value[6][3][2];
	PARAMETER();
	~PARAMETER();
	void PARAMETER::operator=(const PARAMETER &p);
};
