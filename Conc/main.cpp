#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    

	int no_of_rows = 100;
	int no_of_cols = 100;
	int numS = 1000;
	int numF= 1000;

	int xPos;
	int yPos;
	int spriteH = 1000/no_of_rows;
	int spriteW = 1000/no_of_cols;

	//define rectangle shape
	sf::RectangleShape rectangle(sf::Vector2f(spriteH, spriteW));

	//define "sea" matrix of shapes
	std::vector<std::vector<sf::RectangleShape> > matrix;
	matrix.resize(no_of_rows, std::vector<sf::RectangleShape>(no_of_cols, rectangle));

	//position all shapes
	for(int i = 0; i < no_of_rows; ++i){
		xPos = spriteH * i;
		for(int j = 0; j < no_of_cols; ++j){
			yPos = spriteH * j;
			matrix[i][j].setPosition(xPos, yPos);	
			matrix[i][j].setFillColor(sf::Color::Black);	
		}
	}
	

	srand (time(NULL));
	
	//only run if number of sharks and fishes don't exceed grid size
	if((numS + numF) < (no_of_cols*no_of_rows)){
		//create shark and fish colo(u)rs, will clean up 
		//create sharks
		for(int i = 0; i < numS; ++i){
			int x = rand() % no_of_rows;
			int y = rand() % no_of_cols;
			while(matrix[x][y].getFillColor() == sf::Color::Transparent){
				x = rand() % no_of_rows;
				y = rand() % no_of_cols;
			}
			matrix[x][y].setFillColor(sf::Color::Red);

		}
		//create sharks
		for(int i = 0; i < numS; ++i){
			int x = rand() % no_of_rows;
			int y = rand() % no_of_cols;

			while(matrix[x][y].getFillColor() == sf::Color::Transparent){
				x = rand() % no_of_rows;
				y = rand() % no_of_cols;
			}

			matrix[x][y].setFillColor(sf::Color::White);

		}
	} 

	//draw and display loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
		
        for(int i = 0; i < no_of_rows; ++i){
			for(int j = 0; j < no_of_cols; ++j){
				window.draw(matrix[i][j]);
			}
		}
        window.display();
    }

    return 0;
}
