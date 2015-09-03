#include "snake.h"

snakepart::snakepart(int col, int row) {
	x = col;
	y = row;
}

snakepart::snakepart() {
	x = 0;
	y = 0;
}

snakeclass::snakeclass() {
	initscr();
	nodelay(stdscr, true);
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	getmaxyx(stdscr, maxheight, maxwidth);
	bodychar = 'x';
	borderchar = (char)219;
	foodchar = '*';
	food.x = 0;
	food.y = 0;
	for (int i = 0; i < 5; i++)
		snake.push_back(snakepart(40+i, 10));
	points = 0;
	interval = 110000;
	get = false;
	direction = 'l';
	srand(time(0));
	putfood();
	for (int i = 0; i < maxwidth - 1; i++)
	{
		move(0, i);
		addch(borderchar);
	}
	for (int i = 0; i < maxwidth - 1; i++)
	{
		move(maxheight - 2, i);
		addch(borderchar);
	}
	for(int i = 0; i < maxheight - 2; i++)
	{
		move(i, 0);
		addch(borderchar);
	}
	for(int i = 0; i < maxheight - 2; i++)
	{
		move(i,maxwidth-2);
		addch(borderchar);
	}
	for(int i = 0; i < snake.size(); i++)
	{
		move(snake[i].y, snake[i].x);
		addch(bodychar);
	}
	move(maxheight - 1, 0);
	printw("%d", points);
	refresh();
}

snakeclass::~snakeclass()
{
	nodelay(stdscr, false);
	getch();
	getch();
	endwin();
}

void snakeclass::putfood()
{
	while (true)
	{
		int tmpx = rand()%(maxwidth - 3) + 1;
		int tmpy = rand()%(maxheight - 3) + 1;
		for(int i = 0; i < snake.size(); i++)
			if (snake[i].x == tmpx && snake[i].y == tmpy)
				continue;
		food.x = tmpx;
		food.y = tmpy;
		move(food.y, food.x);
		addch(foodchar);
		refresh();
		break;
	}
}

bool snakeclass::collision()
{
	if(snake[0].x == 0 || snake[0].x == maxwidth - 1 || snake[0].y == 0 || snake[0].y == maxheight - 2)
		return true;
	
	for(int i = 3; i < snake.size(); i++)
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return true;

	if(snake[0].x == food.x && snake[0].y == food.y)
	{
		get = true;
		putfood();
		// add points and print on the screen
		points += 10;
		move(maxheight - 1, 0);
		printw("%d", points);
		refresh();
		if ((points%100) == 0)
			interval -= 10000;
	}else{get = false;}
	return false;
}

void snakeclass::movesnake()
{
	int input_k = getch();
	switch (input_k)
	{
		case KEY_LEFT:
			if(direction != 'r')
				direction = 'l';
			break;
		case KEY_UP:
			if(direction != 'd')
				direction = 'u';
			break;
		case KEY_RIGHT:
			if(direction != 'l')
				direction = 'r';
			break;
		case KEY_DOWN:
			if(direction != 'u')
				direction = 'd';
			break;
		case KEY_BACKSPACE:
			direction = 'q';
			break;
	}
	if(!get)
	{
		snakepart tail = snake[snake.size() - 1];
		move(tail.y, tail.x);
		addch(' ');
		refresh();
		snake.pop_back();
	}
	snakepart head = snake[0];
	switch(direction)
	{
		case 'l':
			snake.insert(snake.begin(), snakepart(head.x-1, head.y));
			break;
		case 'r':
			snake.insert(snake.begin(), snakepart(head.x+1, head.y));
			break;
		case 'u':
			snake.insert(snake.begin(), snakepart(head.x, head.y-1));
			break;
		case 'd':
			snake.insert(snake.begin(), snakepart(head.x, head.y+1));
	}
	move(snake[0].y, snake[0].x);
	addch(bodychar);
	refresh();
}

void snakeclass::start()
{
	while(true)
	{
    	if(collision())
    	{
    		move(maxwidth/2 - 4, maxheight/2);
    		printw("Game over");
    		break;
    	}
    	movesnake();
    	if(direction == 'q')
    		break;
    	usleep(interval);
	}
}
