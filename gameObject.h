#pragma once
class Tile
{
public:
	char type; //f=fish s=shark w=water
	char symbol; //ascii representation of tile
	int food; //how long since a shark has eaten
	int age; //age of entity
	int turnUpdated; //turn tile was updated
	Tile(); //blank tile
	Tile(int turn); //blank tile later in game
	Tile(char c, int a, int turn); //create a fish or shark
};
