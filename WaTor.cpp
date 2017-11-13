#include "stdafx.h"
#include "gameObject.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

const int height = 28;
const int width = 64;
const int fishBreed = 3;
const int sharkBreed = 10;
const int starve = 3;

void Draw(Tile world[width][height]);
void Init(Tile world[width][height], int numFish, int numSharks);
void Sim(Tile world[width][height], int day, int &numFish,int &numSharks);
void MoveFish(Tile world[width][height], int x, int y, int day, int &numFish);
void MoveShark(Tile world[width][height], int x, int y, int day, int &numShark);
void MoveSharkEat(Tile world[width][height], int x, int y, int day, int &numShark, int &numFish);

bool IsEmpty(Tile world[width][height], int x, int y);
bool HasFish(Tile world[width][height], int x, int y);
int CheckSides(int dest);
int CheckTop(int dest);


int main()
{
	srand(time(NULL)); //seed the ran
	int numFish = 850;
	int numSharks = 85;
	int day = 0;

	Tile world[width][height];
	Init(world, numFish, numSharks);
	while (true)
	{
		Draw(world);
		cout << "Day: " << day << " Fish: " << numFish << "Sharks: " << numSharks;
		day++;
		Sim(world, day, numFish, numSharks);
		this_thread::sleep_for(chrono::milliseconds(200));
	}
	
	
	return 0;
}

void Draw(Tile world[width][height])
{
	system("CLS");
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			cout << world[x][y].symbol;
		}
		cout << endl;
	}
	cout << endl;
	
}

void Init(Tile world[width][height], int numFish, int numSharks)
{
	int day = 0;
	while (numFish > 0)
	{
		int x = rand() % width;
		int y = rand() % height;
		if (world[x][y].type = 'w')
		{
			world[x][y] = Tile('f', rand() % fishBreed, day);
			numFish--;
		}
	}
	while (numSharks > 0)
	{
		int x = rand() % width;
		int y = rand() % height;
		if (world[x][y].type = 'w')
		{
			world[x][y] = Tile('s', rand() % sharkBreed, day);
			numSharks--;
		}
	}
}

void Sim(Tile world[width][height], int day, int &numFish, int &numSharks)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (world[x][y].turnUpdated < day)
			{
				if (world[x][y].type == 'w')
				{
					world[x][y].turnUpdated = day;
				}
				else if (world[x][y].type == 'f')
				{
					MoveFish(world, x, y, day, numFish);
				}
				else
				{
					MoveSharkEat(world, x, y, day, numSharks, numFish);
				}
			}
		}
	}

}

void MoveFish(Tile world[width][height], int x, int y, int day, int &numFish)
{
	int x2 = x;
	int y2 = y;
	bool north = IsEmpty(world, x, y - 1);
	bool south = IsEmpty(world, x, y + 1);
	bool east = IsEmpty(world, x + 1, y);
	bool west = IsEmpty(world, x - 1, y);
	int options = 0;
	if (north) options++;
	if (south) options++;
	if (east) options++;
	if (west) options++;
	if (options == 0)
	{
		world[x][y].age++;
		world[x][y].turnUpdated = day;
	}
	else
	{
		int direction = rand() % 4 + 1;
		while (true)
		{
			if (direction == 1 && north)
			{
				y2 = CheckTop(y2 - 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= fishBreed)
				{
					world[x][y] = Tile('f', 0, day);
					world[x2][y2].age = 0;
					numFish++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 2 && south)
			{
				y2 = CheckTop(y2 + 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= fishBreed)
				{
					world[x][y] = Tile('f', 0, day);
					world[x2][y2].age = 0;
					numFish++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 3 && east)
			{
				x2 = CheckSides(x2 + 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= fishBreed)
				{
					world[x][y] = Tile('f', 0, day);
					world[x2][y2].age = 0;
					numFish++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 4 && west)
			{
				x2 = CheckSides(x2 - 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= fishBreed)
				{
					world[x][y] = Tile('f', 0, day);
					world[x2][y2].age = 0;
					numFish++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else
			{
				direction = direction % 4 + 1; 
			}
		}
	}
}
void MoveShark(Tile world[width][height], int x, int y, int day, int &numShark)
{
	int x2 = x;
	int y2 = y;
	bool north = IsEmpty(world, x, y - 1);
	bool south = IsEmpty(world, x, y + 1);
	bool east = IsEmpty(world, x + 1, y);
	bool west = IsEmpty(world, x - 1, y);
	int options = 0;
	if (north) options++;
	if (south) options++;
	if (east) options++;
	if (west) options++;
	if (options == 0)
	{
		world[x][y].age++;
		world[x][y].turnUpdated = day;		
	}
	else
	{
		int direction = rand() % 4 + 1;
		while (true)
		{
			if (direction == 1 && north)
			{
				y2 = CheckTop(y2 - 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 2 && south)
			{
				y2 = CheckTop(y2 + 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 3 && east)
			{
				x2 = CheckSides(x2 + 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 4 && west)
			{
				x2 = CheckSides(x2 - 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else
			{
				direction = direction % 4 + 1; 
			}
		}
	}
}
void MoveSharkEat(Tile world[width][height], int x, int y, int day, int &numShark, int &numFish)
{
	int x2 = x;
	int y2 = y;
	bool north = HasFish(world, x, y - 1);
	bool south = HasFish(world, x, y + 1);
	bool east = HasFish(world, x + 1, y);
	bool west = HasFish(world, x - 1, y);
	int options = 0;
	if (north) options++;
	if (south) options++;
	if (east) options++;
	if (west) options++;
	if (options == 0)
	{
		world[x][y].food++;
		if (world[x][y].food >= starve)
		{
			world[x][y] = Tile(day);
			numShark--;
		}
		else
		{
			MoveShark(world, x, y, day, numShark);
		}
	}
	else
	{
		int direction = rand() % 4 + 1;
		numFish--;
		while (true)
		{
			if (direction == 1 && north)
			{
				y2 = CheckTop(y2 - 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				world[x2][y2].food = 0;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 2 && south)
			{
				y2 = CheckTop(y2 + 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				world[x2][y2].food = 0;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 3 && east)
			{
				x2 = CheckSides(x2 + 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				world[x2][y2].food = 0;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else if (direction == 4 && west)
			{
				x2 = CheckSides(x2 - 1);
				world[x2][y2] = world[x][y];
				world[x2][y2].age++;
				world[x2][y2].turnUpdated = day;
				world[x2][y2].food = 0;
				if (world[x2][y2].age >= sharkBreed)
				{
					world[x][y] = Tile('s', 0, day);
					world[x2][y2].age = 0;
					numShark++;
				}
				else
				{
					world[x][y] = Tile(day);
				}
				break;
			}
			else
			{
				direction = direction % 4 + 1; //FIXME
			}
		}
	}
}
bool IsEmpty(Tile world[width][height], int x, int y)
{
	int destX = x;
	int destY = y;
	destX = CheckSides(x);
	destY = CheckTop(y);
	if (world[destX][destY].type == 'w') return true;
	else return false;
}
bool HasFish(Tile world[width][height], int x, int y)
{
	int destX = x;
	int destY = y;
	destX = CheckSides(x);
	destY = CheckTop(y);
	if (world[destX][destY].type == 'f') return true;
	else return false;
}
int CheckSides(int dest)
{
	if (dest == -1) return width - 1;
	if (dest == width) return 0;
	return dest;
}
int CheckTop(int dest)
{
	if (dest == -1) return height - 1;
	if (dest == height) return 0;
	return dest;
}
