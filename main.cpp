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
	int numS = 100;
	int numF= 100;

	int xPos;
	int yPos;
	int spriteH = 1000/no_of_rows;
	int spriteW = 1000/no_of_cols;

	sf::RectangleShape rectangle(sf::Vector2f(spriteH, spriteW));

	std::vector<std::vector<sf::RectangleShape> > matrix;
	matrix.resize(no_of_rows, std::vector<sf::RectangleShape>(no_of_cols, rectangle));
	for(int i = 0; i < no_of_rows; ++i){
		xPos = spriteH * i;
		for(int j = 0; j < no_of_cols; ++j){
			yPos = spriteH * j;
			matrix[i][j].setPosition(xPos, yPos);	
			matrix[i][j].setFillColor(sf::Color::Transparent);	
		}
	}
	srand (time(NULL));

	if(matrix[0][0].getFillColor() == sf::Color::Transparent){
		std::cout << "true";
	}
	std::cout << "main function\n";
	for(int i = 0; i < no_of_rows; ++i){
		for(int j = 0; j < no_of_cols; ++j){
			int x = rand() % no_of_rows;
			int y = rand() % no_of_cols;
			while(matrix[x][y].getFillColor != sf::Color::Transparent{
				x = rand() % no_of_rows;
				y = rand() % no_of_cols;
			}
		}
	}
    /*while (window.isOpen())
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
    }*/

    return 0;
}
