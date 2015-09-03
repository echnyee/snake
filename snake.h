#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>

#ifndef SNAKE_H
#define SNAKE_H

struct snakepart {
	int x,y;
	snakepart(int col, int row);
	snakepart();
};

class snakeclass {
	int points,interval,maxwidth,maxheight;
	char direction,bodychar,borderchar,foodchar;
	bool get;
	snakepart food;
	std::vector <snakepart> snake;

	void putfood();
	bool collision();
	void movesnake();
public:
	snakeclass();
	~snakeclass();
	void start();
};
#endif
