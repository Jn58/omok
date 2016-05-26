#include "game.h"



game::game()
{
	map = new MAP();
}


game::~game()
{
	delete map;
}

int game::check(POS p)
{
	turn *= -1;
	map->map[p.x][p.y] = turn;
	
	int j, k;

	for (j = 0; j < map->size - 4; j++) // 대각선
	{
		for (k = 0; k < map->size - 4; k++)
		{
			if (map->map[j][k] && map->map[j][k] == map->map[1 + j][1 + k] && map->map[j][k] == map->map[2 + j][2 + k] && map->map[j][k] == map->map[3 + j][3 + k] && map->map[j][k] == map->map[4 + j][4 + k])
			{
				if (j == map->size - 5 && k == 0)
				{
					return map->map[j][k];
				}
				else if (j == 0 && k == map->size - 5)
				{
					return map->map[j][k];
				}
				if (j == 0 || k == 0)
				{
					if (map->map[j][k] != map->map[j + 5][k + 5]) return map->map[j][k];
				}
				else if (j == map->size - 5 || k == map->size - 5)
				{
					if (map->map[j][k] != map->map[j - 1][k - 1]) return map->map[j][k];
				}
				else
				{
					if ((map->map[j][k] != map->map[j + 5][k + 5]) && (map->map[j][k] != map->map[j - 1][k - 1])) return map->map[j][k];

				}
			}
			if (map->map[4 + j][k] && map->map[4 + j][k] == map->map[3 + j][1 + k] && map->map[4 + j][k] == map->map[2 + j][2 + k] && map->map[4 + j][k] == map->map[1 + j][3 + k] && map->map[4 + j][k] == map->map[j][4 + k])
			{
				if (j == 0 && k == 0)
				{
					return map->map[4 + j][k];
				}
				else if (j == map->size - 5 && k == map->size - 5)
				{
					return map->map[4 + j][k];
				}
				else if (j == 0 || k == map->size - 5)
				{
					if (map->map[4 + j][k] != map->map[j + 5][k - 1]) return map->map[4 + j][k];
				}
				else if (j == map->size - 5 || k == 0)
				{
					if (map->map[4 + j][k] != map->map[j - 1][k + 5]) return map->map[4 + j][k];
				}
				else
				{
					if ((map->map[4 + j][k] != map->map[j + 5][k - 1]) && (map->map[4 + j][k] != map->map[j - 1][k + 5])) return map->map[4 + j][k];

				}
			}
		}
	}





	for (j = 0, k = 0; j < map->size; j++, k = 0) // y축 방향
	{
		if (map->map[j][k] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k] && map->map[j][k] != map->map[j][5 + k]) return map->map[j][k];
		for (k = 1; k < map->size - 5; k++)
		{
			if (map->map[j][k] && map->map[j][k] != map->map[j][k - 1] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k] && map->map[j][k] != map->map[j][5 + k]) return map->map[j][k];
		}
		if (map->map[j][k] && map->map[j][k] != map->map[j][k - 1] && map->map[j][k] == map->map[j][1 + k] && map->map[j][k] == map->map[j][2 + k] && map->map[j][k] == map->map[j][3 + k] && map->map[j][k] == map->map[j][4 + k]) return map->map[j][k];
	}
	for (k = 0, j = 0; k < map->size; k++, j = 0) //x축 방향
	{
		if (map->map[j][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k] && map->map[j][k] != map->map[5 + j][k]) return map->map[j][k];
		for (j = 1; j < map->size - 5; j++)
		{
			if (map->map[j][k] && map->map[j][k] != map->map[j - 1][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k] && map->map[j][k] != map->map[5 + j][k]) return map->map[j][k];
		}
		if (map->map[j][k] && map->map[j][k] != map->map[j - 1][k] && map->map[j][k] == map->map[1 + j][k] && map->map[j][k] == map->map[2 + j][k] && map->map[j][k] == map->map[3 + j][k] && map->map[j][k] == map->map[4 + j][k]) return map->map[j][k];
	}
	for (k = 0; k < map->size; k++)
		for (j = 0; j < map->size; j++)
			if (map->map[k][j] == 0) return 0;

	return 3;
}

void game::clear(void)
{
	delete map;
	map = new MAP();
	turn = -1;
}


