#include "MAP.h"
#include <iostream>

void MAP::makeMap(int size=15)
{
	this->~MAP();
	MAP::size = size;
	map = new int*[size];
	for (int i = 0; i < size; i++)
	{
		map[i] = new int[size];
	}

}
void MAP::clearMap(void)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			map[i][j] = 0;
}
int MAP::getSize()
{
	return size;
}
void MAP::setSize(int size = 15)
{
	MAP::size = size;
}
void MAP::print()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (map[j][i]) std::cout << ("%c", (map[j][i] == 1) ? 'o' : 'x');
			else std::cout<<"+";
		}
		std::cout << std::endl;

	}

	printf("\n\n");

}
MAP::MAP()
{
	makeMap();
	clearMap();
}
/*MAP::MAP( MAP &parent)
{
	makeMap(parent.getSize());
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			map[i][j] = parent.map[i][j];
}*/
MAP::MAP(MAP  * const  parent)
{
	makeMap(parent->getSize());
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			map[i][j] = parent->map[i][j];
}
MAP::~MAP()
{
	if (size != 0)
	{
		for (int i = 0; i < size; i++)
		{
			delete map[i];
		}
		delete map;
		size = 0;
	}
}