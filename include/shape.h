#ifndef _SHAPE_H_
#define _SHAPE_H_

#define MAX_SHAPES 7
#define MAX_POINTS 4
#define MPP MAX_POINTS * MAX_POINTS

#define CHAR_SHAPE 'X'
#define CHAR_LINES '#'
#define CHAR_EMPTY ' '
#define CHAR_VERTLINE '|'
#define CHAR_HORIZLINE '='

#define UPLEVEL 5


char shapes[MAX_SHAPES][MAX_POINTS * MAX_POINTS] = {
	{' ', 'X', ' ', ' ', 
	 ' ', 'X', ' ', ' ', 
	 ' ', 'X', 'X', ' ', 
	 ' ', ' ', ' ', ' '}, // L

	{' ', ' ', 'X', ' ', 
	 ' ', ' ', 'X', ' ', 
	 ' ', 'X', 'X', ' ', 
	 ' ', ' ', ' ', ' '}, // Г

	{' ', ' ', ' ', ' ', 
	 ' ', 'X', 'X', ' ', 
	 ' ', ' ', 'X', 'X', 
	 ' ', ' ', ' ', ' '}, // Z

	{' ', ' ', ' ', ' ', 
	 ' ', ' ', 'X', 'X', 
	 ' ', 'X', 'X', ' ', 
	 ' ', ' ', ' ', ' '}, // ~Z

	{' ','X', ' ', ' ',
	 ' ','X', ' ', ' ', 
	 ' ','X', ' ', ' ', 
	 ' ','X', ' ', ' '}, // V - Line

	{' ', ' ', ' ', ' ', 
	 ' ', 'X', 'X', ' ', 
	 ' ', 'X', 'X', ' ', 
	 ' ', ' ', ' ', ' '}, // Cube

	{' ', ' ', ' ', ' ', 
	 ' ', 'X', 'X', 'X', 
	 ' ', ' ', 'X', ' ', 
	 ' ', ' ', ' ', ' '}, // T

};

#endif
