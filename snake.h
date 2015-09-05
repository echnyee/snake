#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <string>

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
	bool eat;
	snakepart food;
	std::vector <snakepart> snake;

    void initialize_screen();
    void customize_outlook();
    void create_border();
    void initialize_snake();
	void putfood();
    void initialize_points();

	bool collision();
    void add_points_and_update_speed();

	void movesnake();
    void read_command();
    void update_tail();
    void update_head();

public:
	snakeclass();
	~snakeclass();
	void start();
};
#endif
