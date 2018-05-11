#include <SFML/Graphics.hpp>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <omp.h>

using namespace std;

class Tile
{
public:
  char type;
  char symbol;
  sf::RectangleShape sprite;
  int food;
  int age;
  int turnUpdated;

  Tile()
  {
    type = 'w';
    symbol = ' ';
    sprite = sf::RectangleShape(sf::Vector2f(10,10));
    sprite.setFillColor(sf::Color::Blue);
    food = 0;
    age = 0;
    turnUpdated = 0;
  }
  Tile(int turn)
  {
    type = 'w';
    symbol = ' ';
    sprite = sf::RectangleShape(sf::Vector2f(10,10));
    sprite.setFillColor(sf::Color::Blue);
    food = 0;
    age = 0;
    turnUpdated = turn;
  }
  Tile(char c, int a, int turn)
  {
    if(c == 'f')
      {
	type = 'f';
	symbol = '-';
	sprite = sf::RectangleShape(sf::Vector2f(10,10));
	sprite.setFillColor(sf::Color::Green);
	food = 0;
	age = a;
	turnUpdated = turn;
      }
     else
      {
	type = 's';
	symbol = '#';
	sprite = sf::RectangleShape(sf::Vector2f(10,10));
	sprite.setFillColor(sf::Color::Red);
	food = 0;
	age = a;
	turnUpdated = turn;
      }
  }
};
int height = 200;
int width = 200;
//160000
const int spriteSize = 5;
const int fishBreed = 3;
const int sharkBreed = 10;
const int starve = 3;

void Init(vector<vector<Tile> > &world, int numFish, int numSharks, int spriteH, int spriteW);
void Sim(vector<vector<Tile> > &world, int day, int &numFish,int &numSharks,int spriteH, int spriteW);
void MoveFish(vector<vector<Tile> > &world, int x, int y, int day, int &numFish,int spriteH, int spriteW);
void MoveShark(vector<vector<Tile> > &world, int x, int y, int day, int &numShark,int spriteH, int spriteW);
void MoveSharkEat(vector<vector<Tile> > &world, int x, int y, int day, int &numShark, int &numFish,int spriteH, int spriteW);
bool IsEmpty(vector<vector<Tile> > &world, int x, int y);
bool HasFish(vector<vector<Tile> > &world, int x, int y);
int CheckSides(int dest);
int CheckTop(int dest);

int main(int argc, char *argv[])
{
  if(argc > 1)
    {
      height = atoi(argv[1]);
      width = atoi(argv[1]);
    }
  int screenW = width * spriteSize;
  int screenH = height * spriteSize;
  sf::RenderWindow window(sf::VideoMode(screenW, screenH), "WaTor");
  srand(time(NULL));
  int tiles = width * height ;
  int numFish = 5000;
  int numSharks = 1000;
  int day = 0;
  int to;

  int xPos;
  int yPos;
  int spriteH = screenH/height;
  int spriteW = screenW/width;
  ofstream myfile2;
  vector<vector<Tile> > world;
  world.resize(width);
  #pragma omp parallel for
  for(int i = 0; i < width; ++i)
    {
      world[i].resize(height);
    }
  // Tile world[width][height];
  Init(world, numFish, numSharks, spriteH, spriteW);
  time_t end = time(NULL) + 15;
  // while (true)
  //   {
  //     Draw(world);
  //     cout << "Day: " << day << " Fish: " << numFish << "Sharks: " << numSharks<< endl;
  //     day++;
  //     Sim(world,day,numFish,numSharks);
  //     this_thread::sleep_for(chrono::milliseconds(200));
  //   }
  while (time(NULL) <= end)
    {
      sf::Event event;
      while (window.pollEvent(event))
        {
	  if (event.type == sf::Event::Closed)
	    window.close();
        }

      window.clear(sf::Color::Blue);
      //#pragma omp parallel for
      for(int i = 0; i < width; ++i){
	for(int j = 0; j < height; ++j){
	  window.draw(world[i][j].sprite);
	}
      }
     
      window.display();
      day++;
      Sim(world,day,numFish,numSharks, spriteH, spriteW);
      //this_thread::sleep_for(chrono::milliseconds(50));
    }
  ofstream myfile;
 
  myfile.open ("benchmarking_conc_15sec.txt",ios::app);
  myfile << height << "x" << width << "\t iterations:" << day << "\t " << day/15 << "fps" << "\n";
  myfile.close();
  return 0;
}

void Init(vector<vector<Tile> > &world, int numFish, int numSharks, int spriteH, int spriteW)
{
  int day = 0;
  int xPos = 0;
  int yPos = 0;
  for (int y = 0; y < height; y++)
    {
      yPos = spriteH * y;
      for (int x = 0; x < width; x++)
	{
	  xPos = spriteH * x;
	  world[x][y].sprite.setPosition(xPos,yPos);
	  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));	  
	}
    } 
  while (numFish > 0)
    {
      int x = rand() % width;
      int y = rand() % height;
      yPos = spriteH * y;
      xPos = spriteH * x;
      if (world[x][y].type = 'w')
	{
	  world[x][y] = Tile('f', rand() % fishBreed, day);
	  world[x][y].sprite.setPosition(xPos,yPos);
	  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
	  numFish--;
	}
    }
  while (numSharks > 0)
    {
      int x = rand() % width;
      int y = rand() % height;
      yPos = spriteH * y;
      xPos = spriteH * x;
      if (world[x][y].type = 'w')
	{
	  world[x][y] = Tile('s', rand() % sharkBreed, day);
	  world[x][y].sprite.setPosition(xPos,yPos);
	  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
	  numSharks--;
	}
    }
}

void Sim(vector<vector<Tile> > &world, int day, int &numFish, int &numSharks,int spriteH, int spriteW)
{
  #pragma omp parallel for
  for (int y = 0; y < height; y++)
    {
      #pragma omp paralell
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
		  MoveFish(world, x, y, day, numFish, spriteH, spriteW);
		}
	      else
		{
		  MoveSharkEat(world, x, y, day, numSharks, numFish, spriteH, spriteW);
		}
	    }
	}
    }
}

void MoveFish(vector<vector<Tile> > &world, int x, int y, int day, int &numFish, int spriteH, int spriteW)
{
  int x2 = x;
  int y2 = y;
  int yPos = spriteH * y;
  int xPos = spriteH * x;
  int y2Pos = spriteH * y;
  int x2Pos = spriteH * x;
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
	      y2Pos = spriteH * y2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Green);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= fishBreed)
		{
		  world[x][y] = Tile('f', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numFish++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 2 && south)
	    {
	      y2 = CheckTop(y2 + 1);
	      y2Pos = spriteH * y2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Green);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= fishBreed)
		{
		  world[x][y] = Tile('f', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numFish++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 3 && east)
	    {
	      x2 = CheckSides(x2 + 1);
	      x2Pos = spriteH * x2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Green);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= fishBreed)
		{
		  world[x][y] = Tile('f', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numFish++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 4 && west)
	    {
	      x2 = CheckSides(x2 - 1);
	      x2Pos = spriteH * x2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Green);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= fishBreed)
		{
		  world[x][y] = Tile('f', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numFish++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
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

void MoveShark(vector<vector<Tile> > &world, int x, int y, int day, int &numShark, int spriteH, int spriteW)
{
  int x2 = x;
  int y2 = y;
  int yPos = spriteH * y;
  int xPos = spriteH * x;
  int y2Pos = spriteH * y;
  int x2Pos = spriteH * x;
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
	      y2Pos = spriteH * y2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 2 && south)
	    {
	      y2 = CheckTop(y2 + 1);
	      y2Pos = spriteH * y2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 3 && east)
	    {
	      x2 = CheckSides(x2 + 1);
	      x2Pos = spriteH * x2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 4 && west)
	    {
	      x2 = CheckSides(x2 - 1);
	      x2Pos = spriteH * x2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
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

void MoveSharkEat(vector<vector<Tile> > &world, int x, int y, int day, int &numShark, int &numFish, int spriteH, int spriteW)
{
  int x2 = x;
  int y2 = y;
  int yPos = spriteH * y;
  int xPos = spriteH * x;
  int y2Pos = spriteH * y;
  int x2Pos = spriteH * x;
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
	  world[x][y].sprite.setPosition(xPos,yPos);
	  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
	  numShark--;
	}
      else
	{
	  MoveShark(world, x, y, day, numShark, spriteH, spriteW);
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
	      y2Pos = spriteH * y2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].food = 0;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 2 && south)
	    {
	      y2 = CheckTop(y2 + 1);
	      y2Pos = spriteH * y2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].food = 0;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 3 && east)
	    {
	      x2 = CheckSides(x2 + 1);
	      x2Pos = spriteH * x2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].food = 0;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		}
	      break;
	    }
	  else if (direction == 4 && west)
	    {
	      x2 = CheckSides(x2 - 1);
	      x2Pos = spriteH * x2;
	      world[x2][y2] = world[x][y];
	      world[x2][y2].age++;
	      world[x2][y2].turnUpdated = day;
	      world[x2][y2].food = 0;
	      world[x2][y2].sprite.setFillColor(sf::Color::Red);
	      world[x2][y2].sprite.setPosition(x2Pos,y2Pos);
	      if (world[x2][y2].age >= sharkBreed)
		{
		  world[x][y] = Tile('s', 0, day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
		  world[x2][y2].age = 0;
		  numShark++;
		}
	      else
		{
		  world[x][y] = Tile(day);
		  world[x][y].sprite.setPosition(xPos,yPos);
		  world[x][y].sprite.setSize(sf::Vector2f(spriteH, spriteW));
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

bool IsEmpty(vector<vector<Tile> > &world, int x, int y)
{
  int destX = x;
  int destY = y;
  destX = CheckSides(x);
  destY = CheckTop(y);
  if (world[destX][destY].type == 'w') return true;
  else return false;
}

bool HasFish(vector<vector<Tile> > &world, int x, int y)
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

    
