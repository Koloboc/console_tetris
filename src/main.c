#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "shape.h"

#define FLD_HEIGHT 20
#define FLD_WIDTH  20


enum _direction{
	LEFT,
	RIGHT,
	DOWN,
	DROP,
	ROTATE
};

typedef struct _Shape{
	char points[MPP];
	char symbol;
	int x;
	int y;
}Shape;

int score = 0;
int level = 0;

char fld[FLD_HEIGHT][FLD_WIDTH + 1];
char lines[FLD_HEIGHT][FLD_WIDTH + 1];


int collision(Shape*, int, int);
int move(Shape *shape, int dir);

void rotate_shape(Shape *shape, int rot){
	Shape sh;
	int ind;

	while(rot){
		for(int y = 0; y < MAX_POINTS; y++){
			for(int x = 0; x < MAX_POINTS; x++){
				ind = y * MAX_POINTS + x;
				if(x + shape->x >= FLD_WIDTH - 1)
					shape->x--;
				if(x + shape->x <= 0)
					shape->x++;
				if(lines[shape->y + y][shape->x + x] != CHAR_EMPTY)
					return;

				sh.points[ind] = shape->points[(MAX_POINTS - x - 1) * MAX_POINTS + y ];
			}
		}
		for(int i = 0; i < MPP; i++){
			shape->points[i] = sh.points[i];
		}
		rot--;
	}
}

void init_shape(Shape *shape, int x, int y){
	int rotate = rand() % 3;

	shape->symbol = CHAR_SHAPE;
	shape->x = x;
	shape->y = y;
	memcpy(shape->points, shapes[rand() % MAX_SHAPES], MPP);
	rotate_shape(shape, rotate);
}

void fool_lines(){

	int y = FLD_HEIGHT - 1;

	while(y){
	//for(int y = FLD_HEIGHT - 1; y > 1; y--) {
		int del_line = 1;
		for(int x = 1; x < FLD_WIDTH - 2; x++){
			if(lines[y][x] == CHAR_EMPTY){
				del_line = 0;
				break;
			}
		}
		if(del_line){
			for(int j = y; j > 0; j--)
				for(int i = 0; i < FLD_WIDTH; i++){
					lines[j][i] = lines[j - 1][i];
				}
			y++; // Оставляем ту же строку для проверки (поскольку она изменилась)
			score++;
		}
		y--;
	}
}

void clear_fld(){
	fld[0][0] = '|';
	fld[0][FLD_WIDTH - 1] = CHAR_VERTLINE;
	fld[0][FLD_WIDTH] = '\0';

	for(int i = 1; i < FLD_WIDTH - 1; i++)
		fld[0][i] = CHAR_EMPTY;

	for(int i = 1; i < FLD_HEIGHT; i++)
		memcpy((void*)&fld[i][0], (void*)&fld[0][0], FLD_WIDTH);

	for(int i = 1; i < FLD_WIDTH - 1; i++)
		fld[FLD_HEIGHT - 1][i] = CHAR_HORIZLINE;
}

void init_lines(){
	lines[0][FLD_WIDTH] = '\0';
	for(int i = 1; i < FLD_WIDTH - 1; i++)
		lines[0][i] = CHAR_EMPTY;

	for(int i = 1; i < FLD_HEIGHT; i++)
		memcpy((void*)&lines[i][0], (void*)&lines[0][0], FLD_WIDTH);
}

void show_field(){
	for(int i = 0; i < FLD_HEIGHT; i++){
		if(i == 2){
			printf("%s  lines: %d\n", fld[i], score);
		}else if(i == 3){
			printf("%s  level: %d\n", fld[i], level);
		}else{
			printf("%s\n", fld[i]);
		}
	}
}

void put_shape(Shape *shape){
	for(int i = 0; i < MAX_POINTS; i++){
		for(int j = 0; j < MAX_POINTS; j++){
			if(shape->points[ i * MAX_POINTS + j ] == 'X')
				fld[(shape->y + i)][shape->x + j] = shape->symbol;
		}
	}
}

void put_lines(){
	for(int y = 1; y < FLD_HEIGHT - 1; y++){
		for(int x = 1; x < FLD_WIDTH - 1; x++){
			if(lines[y][x] != CHAR_EMPTY){
				fld[y][x] = lines[y][x];
			}
		}
	}
}

int collision(Shape *shape, int x, int y){
	int w = 0;
	int h = 0;
	int l = MAX_POINTS;

	for(int i = MAX_POINTS - 1; i >= 0; i--)
		for(int j = MAX_POINTS - 1; j >= 0; j--)
			if(shape->points[i * MAX_POINTS + j] ==  'X'){
				if(i > h) h = i; // Высота
				if(j > w) w = j; //break;
				if(l > j) l = j; // left
			}

	if((x + w >= FLD_WIDTH - 1) || (x + l <= 0))
		return 1;
	if(y + h >= FLD_HEIGHT - 1)
		return 1;

	for(int j = h; j >= 0; j--){
		for(int i = 0; i <= w; i++){
			if(shape->points[j * MAX_POINTS + i] != CHAR_EMPTY)
			{
				if(lines[y + j][x + i] != CHAR_EMPTY)
					return 1;
			}
		}
	}
	return 0;
}

void add_shape_lines(Shape *shape){
	for(int i = 0; i < MAX_POINTS; i++){
		for(int j = 0; j < MAX_POINTS; j++){
			if(shape->points[i * MAX_POINTS + j] == 'X')
				lines[shape->y + i][shape->x + j] = CHAR_LINES;
		}
	}
	fool_lines();
}

void drop_shape(Shape *shape){
	while(move(shape, DOWN));
}

int move(Shape *shape, int dir){
	switch(dir){
		case LEFT:
			if(!collision(shape, shape->x - 1, shape->y))
				shape->x--;
			break;
		case RIGHT:
			if(!collision(shape, shape->x + 1, shape->y))
				shape->x++;
			break;
		case DROP:
			if(!collision(shape, shape->x, shape->y + 1)){
				shape->y++;
			}else{
				add_shape_lines(shape);
				init_shape(shape, FLD_WIDTH / 2, 0);
			}
			break;
		case DOWN: // Падать вниз
			if(!collision(shape, shape->x, shape->y + 1)){
				shape->y++;
			}else{
				add_shape_lines(shape);
				init_shape(shape, FLD_WIDTH / 2, 0);
				return 0;
			}
			break;
		case ROTATE:
			rotate_shape(shape, 1);
			break;
	}
	return 1;
}

int main(int argc, char **argv){
	Shape obj;
	char ch;
	fd_set fd_r;
	struct timeval tv;

	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	srand(time(NULL));
	init_shape(&obj, FLD_WIDTH / 2, 0);
	init_lines();
	FD_ZERO(&fd_r);
	
	do{
		FD_SET(0, &fd_r);
		tv.tv_sec = 0;
		tv.tv_usec = 500000;

		printf ("\033[0d\033[2J");
		printf ("\n");

		clear_fld();
		put_shape(&obj);
		put_lines();
		show_field();

		int sel = select(1, &fd_r, NULL, NULL, &tv);
		if(sel == -1){
			perror("select:");
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
			return 0;
		}else if( sel == 0){ // 0 - время вышло. не 0 есть ввод
			move(&obj, DOWN); // ВНИЗ
		}else{
			read(0, &ch, 1);
			if(ch == 'a') move(&obj, LEFT); // ЛЕВО
			if(ch == 'd') move(&obj, RIGHT); // ПРАВО
			if(ch == 's') move(&obj, DOWN); // ВНИЗ
			if(ch == 'w') move(&obj, ROTATE); // Поворот
			if(ch == ' ') drop_shape(&obj); // УПАСТЬ
		}
		//check_fly(&obj);
	}while(ch != 'q');

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return 0;
}

