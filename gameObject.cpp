#include "gameObject.h"
Tile:: Tile()
{
  type = 'w';
  symbol = ' ';
  food = 0;
  age = 0;
  turnUpdated = 0;
}
Tile::Tile(int turn)
{
  type = 'w';
  symbol = ' ';
  food = 0;
  age = 0;
  turnUpdated = turn;
}
Tile::Tile(char c, int a, int turn)
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

