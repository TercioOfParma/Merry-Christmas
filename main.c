#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
//---------------------------------------------------- CONSTANTS ------------------------------------------------------------------ 
const static int SNOWFLAKE_PAIR = 1;
const static int GREETING_PAIR = 2;
const static int SNOWFLAKE_COUNT = 100;
const static int COLUMN_WIDTH = 25;
const static int ROW_WIDTH = 80;
const static int MAX_WEIGHT = 5;
const static int SUCCESS = 1;
const static int FAIL = 2;
const static int VELOCITY_MODIFIER = 10;
const static char *GREETING = "MERRY CHRISTMAS";
//----------------------------------------------------- STRUCT AND FUNCTION DEFINITIONS ----------------------------------------------
typedef struct
{
	int y,x, weight, xVel, yVel; //the coordinates on the map
	char display;
}snowflake;

typedef struct
{
	int gravity, windSpeed;

}environVars;
environVars *init(int *success);
snowflake **initSnowflakes(int *success);
void deinit(environVars *options, snowflake **flakes);
void computeVelocitiesAndPosition(snowflake **snow, environVars *options);
void drawSnowflakes(snowflake **snow);
void drawGreeting();
void clearScreen();
void changeWindspeed(environVars *options, int change);
void changeGravity(environVars *options, int change);
// -------------------------------------------------- MAIN FUNCTIONS -------------------------------------------------------------
int main(int argc, char *argv[])
{
	int successfulInit = SUCCESS;
	int input = 0;
	environVars *options = init(&successfulInit);
	snowflake **snow = initSnowflakes(&successfulInit);
	if(successfulInit != FAIL)
	{
		while(1 == 1)
		{
			clearScreen();
			computeVelocitiesAndPosition(snow, options);
			drawSnowflakes(snow);
			drawGreeting();
			refresh();
			input = getch();
			switch(input)
			{//for some reason the compiler doesnt like const static int constants for these
				case '+':
					changeWindspeed(options,1);
					break;
				case '-':
					changeWindspeed(options,-1);
					break;
				case 'g':
					changeGravity(options,1);
					break;
				case 'h':
					changeGravity(options,-1);
					break;
			
			}
			input = 0;
		}
	
	}
	deinit(options, snow);

	return 0;
}
//---------------------------------------------------- INIT AND DEINIT FUNCTIONS ------------------------------------------------
environVars *init(int *success)
{
	environVars *temp = malloc(sizeof(environVars));
	//ncurses init
	initscr();
	noecho();
	cbreak();
	start_color();
	halfdelay(2);
	init_pair(SNOWFLAKE_PAIR, COLOR_WHITE, COLOR_BLACK);
	init_pair(GREETING_PAIR, COLOR_RED, COLOR_GREEN);
	temp->gravity = 10;
	temp->windSpeed = 15;
	return temp;
}
snowflake **initSnowflakes(int *success)
{
	snowflake **temp = malloc(sizeof(snowflake *) * SNOWFLAKE_COUNT);
	if(temp == NULL)
	{
		*success = FAIL;
		return NULL;
	
	}
	int i = 0;
	srand(time(NULL));
	for(i = 0; i < SNOWFLAKE_COUNT; i++)
	{
		temp[i] = malloc(sizeof(snowflake));
		if(temp[i] == NULL)
		{
			*success = FAIL;
			return NULL;
	
		}
		temp[i]->y = 0;
		temp[i]->x= rand() % ROW_WIDTH + 1;
		temp[i]->xVel = 0;
		temp[i]->yVel = 0;
		temp[i]->weight = rand() % MAX_WEIGHT + 1;
		temp[i]->display = '*';
	}
	return temp;
}
void deinit(environVars *options, snowflake **flakes)
{
	free(options);
	int i = 0;
	for(i = 0; i < SNOWFLAKE_COUNT; i++)
	{
		free(flakes[i]);
	
	}
	free(flakes);
	endwin();

}
// --------------------------------------------------- Drawing, handling velocity etc. Functions ------------------------------------------------

void computeVelocitiesAndPosition(snowflake **snow, environVars *options)
{
	srand(time(NULL));
	int i = 0;
	int move;
	for(i = 0; i < SNOWFLAKE_COUNT; i++)
	{
		if(snow[i]->xVel == 0 && snow[i]->yVel == 0)
		{
			move = rand () % 3 + 1;
			if(move == 3)
			{
				snow[i]->xVel = (snow[i]->weight * options->gravity ) / VELOCITY_MODIFIER;
				snow[i]->yVel = (snow[i]->weight * options->windSpeed) / VELOCITY_MODIFIER;
			
			
			}
		
		}
		else
		{
			snow[i]->xVel = (snow[i]->weight * options->gravity ) / VELOCITY_MODIFIER;
			snow[i]->yVel = (snow[i]->weight * options->windSpeed) / VELOCITY_MODIFIER;
			snow[i]->x += snow[i]->xVel;
			snow[i]->y += snow[i]->yVel;
		}
		if(snow[i]->y > COLUMN_WIDTH)
		{
			snow[i]->y = 0;
			snow[i]->yVel = 0;
			snow[i]->xVel = 0;
		
		}
		if(snow[i]->x > ROW_WIDTH)
		{
			snow[i]->x = 0;
			
		
		}
	
	
	}

}
void drawSnowflakes(snowflake **snow)
{
	int i = 0;
	attron(COLOR_PAIR(SNOWFLAKE_PAIR));
	for(i = 0; i < SNOWFLAKE_COUNT; i++)
	{
		if(snow[i]->y != 0)
		{
			mvaddch(snow[i]->y, snow[i]->x,snow[i]->display);
		}
	}
	attroff(COLOR_PAIR(SNOWFLAKE_PAIR));

}
void drawGreeting()
{
	attron(COLOR_PAIR(GREETING_PAIR));
	mvprintw(COLUMN_WIDTH / 2, ROW_WIDTH / 2, GREETING);
	attroff(COLOR_PAIR(GREETING_PAIR));


}
void clearScreen()
{
	int i,j;
	for(i = 0; i < COLUMN_WIDTH; i++)
	{
		for(j = 0; j < ROW_WIDTH; j++)
		{
			mvaddch(i,j,' ');
		}
	}


}
void changeWindspeed(environVars *options, int change)
{
	options->windSpeed += change;


}
void changeGravity(environVars *options, int change)
{
	options->gravity += change;


}