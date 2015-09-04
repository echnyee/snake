#include "snake.h"
#include <chrono>

snakepart::snakepart(int col, int row) {
	x = col;
	y = row;
}

snakepart::snakepart() {
	x = 0;
	y = 0;
}

snakeclass::snakeclass() {
    initialize_screen();
    customize_outlook();
	
    // initialize speed & direction
    interval = 110000;
	direction = 'l';

    // haven't eat
	eat = false;
    // generate a random seed
	srand(time(0));

    create_border();
    initialize_snake();
	putfood();
    initialize_points();
	refresh();
}

void snakeclass::initialize_screen()
{
	initscr();
	nodelay(stdscr, true);
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	getmaxyx(stdscr, maxheight, maxwidth);
}

void snakeclass::customize_outlook()
{
    bodychar = 'o';
	borderchar = (char)219;
	foodchar = '*';
}

void snakeclass::create_border()
{
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
}

void snakeclass::initialize_snake()
{
	for (int i = 0; i < 5; i++)
    {
		snake.push_back(snakepart(40+i, 10));
		move(snake[i].y, snake[i].x);
		addch(bodychar);
	}
}

void snakeclass::initialize_points()
{
    points = 0;
	move(maxheight - 1, 0);
	printw("%d", points);
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
	if(snake[0].x == 0 || snake[0].x == maxwidth - 2 || snake[0].y == 0 || snake[0].y == maxheight - 2)
		return true;
	
	for(int i = 3; i < snake.size(); i++)
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return true;

	if(snake[0].x == food.x && snake[0].y == food.y)
	{
		eat = true;
		putfood();
        add_points_and_update_speed();
	}
    else
    {
        eat = false;
    }
	return false;
}

void snakeclass::add_points_and_update_speed()
{
	points += 10;
	move(maxheight - 1, 0);
	printw("%d", points);
	refresh();

	if ((points%100) == 0)
		interval -= 10000;
}

void snakeclass::movesnake()
{
    read_command();
    update_tail();
    update_head();
	refresh();
}

void snakeclass::read_command()
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
}

void snakeclass::update_tail()
{
	if(!eat)
	{
		snakepart tail = snake[snake.size() - 1];
		move(tail.y, tail.x);
		addch(' ');
		refresh();
		snake.pop_back();
	}
}

void snakeclass::update_head()
{
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
            break;
	}
	move(snake[0].y, snake[0].x);
	addch(bodychar);
}

void snakeclass::start()
{
	using namespace std;
	while(true)
	{
		chrono::monotonic_clock::time_point start = chrono::monotonic_clock::now();
    	if(collision())
    	{
            int midx = maxwidth/2 - 4;
            int midy = maxheight/2;
    		move(midy, midx);
            std::string gameover = "Game Over!";
    		move(maxheight/2, maxwidth/2-gameover.size()/2);
    		printw(gameover.c_str());
    		break;
    	}
    	movesnake();
    	if(direction == 'q')
    		break;
		chrono::monotonic_clock::time_point end = chrono::monotonic_clock::now();
		chrono::monotonic_clock::duration d = end - start;
		int time_elapsed = chrono::duration_cast<chrono::microseconds>(d).count();
		int sleep_time = interval - time_elapsed;
		if ((sleep_time) > 0)
		{
    		usleep(sleep_time);
		}
	}
}

snakeclass::~snakeclass()
{
	nodelay(stdscr, false);
	flushinp();
	getch();
	endwin();
}

