#pragma once
class Tile
{
public:
	char type; //f=fish s=shark w=water
	char symbol; //ascii representation of tile
	int food; //how long since a shark has eaten
	int age; //age of entity
	int turnUpdated; //turn tile was updated

	Tile() //blank tile
	{
		type = 'w';
		symbol = ' ';
		food = 0;
		age = 0;
		turnUpdated = 0;
	}
	Tile(int turn) //blank tile later in game
	{
		type = 'w';
		symbol = ' ';
		food = 0;
		age = 0;
		turnUpdated = turn;
	}
	Tile(char c, int a, int turn) //create a fish or shark
	{
		if (c == 'f')
		{
			type = 'f';
			symbol = '-';
			food = 0;
			age = a;
			turnUpdated = turn;
		}
		else
		{
			type = 's';
			symbol = '#';
			food = 0;
			age = a;
			turnUpdated = turn;
		}
	}

};
