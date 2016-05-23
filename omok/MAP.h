#pragma once
class MAP
{
private:
	
public:
	int ** map;
	int size;
	void makeMap(int n);
	void clearMap(void);
	int getSize();
	void setSize(int size);
	MAP();
	//MAP( MAP  &parent);
	MAP(MAP  * const parent);
	void print();
	~MAP();
};

