#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <wchar.h>

#define MAX_SHAPES 7
#define MAX_POINTS 4
#define MPP MAX_POINTS * MAX_POINTS

#define FILE_SCORE "tetris.score"
#define SCORES 5

const wchar_t SHAPE_SYMBOL = 88; // '#'
const wchar_t CHAR_LINES = 88;   // '#'
const wchar_t CHAR_EMPTY = 32;   // ' ';
const wchar_t CHAR_VERTLINE = L'|';
const wchar_t CHAR_HORIZLINE = L'=';

#define UPLEVEL 5

typedef struct Shape{
	wchar_t points[MPP];
	int x;
	int y;
}Shape;

typedef struct _score{
	char name[10];
	int level;
	int lines;
}st_score;

void rotate_shape(Shape *shape, int rot);
void init_shape(Shape *shape, int x, int y);
void fool_lines();
void clear_fld();
void init_lines();
void show_field();
void put_shape(Shape *shape);
void put_lines();
int collision(Shape *shape, int x, int y);
void add_shape_lines(Shape *shape);
int move(Shape *shape, int dir);
void save_score(st_score*);
void read_score(st_score*);
void print_score(st_score*);

wchar_t shapes[MAX_SHAPES][MAX_POINTS * MAX_POINTS] = {
	{L' ', L'X', L' ', L' ', 
	 L' ', L'X', L' ', L' ', 
	 L' ', L'X', L'X', L' ', 
	 L' ', L' ', L' ', L' '}, // L

	{L' ', L' ', L'X', L' ', 
	 L' ', L' ', L'X', L' ', 
	 L' ', L'X', L'X', L' ', 
	 L' ', L' ', L' ', L' '}, // Г

	{L' ', L' ', L' ', L' ', 
	 L' ', L'X', L'X', L' ', 
	 L' ', L' ', L'X', L'X', 
	 L' ', L' ', L' ', L' '}, // Z

	{L' ', L' ', L' ', L' ', 
	 L' ', L' ', L'X', L'X', 
	 L' ', L'X', L'X', L' ', 
	 L' ', L' ', L' ', L' '}, // ~Z

	{L' ', L'X', L' ', L' ',
	 L' ', L'X', L' ', L' ', 
	 L' ', L'X', L' ', L' ', 
	 L' ', L'X', L' ', L' '}, // V - Line

	{L' ', L' ', L' ', L' ', 
	 L' ', L'X', L'X', L' ', 
	 L' ', L'X', L'X', L' ', 
	 L' ', L' ', L' ', L' '}, // Cube

	{L' ', L' ', L' ', L' ', 
	 L' ', L'X', L'X', L'X', 
	 L' ', L' ', L'X', L' ', 
	 L' ', L' ', L' ', L' '}, // T

};

#endif
