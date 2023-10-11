#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>
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

int score = 0;
int level = 0;
int m_pause = -1;
Shape next;
struct termios oldt, newt;

wchar_t fld[FLD_HEIGHT][FLD_WIDTH + 1];
wchar_t lines[FLD_HEIGHT][FLD_WIDTH];

void rotate_shape(Shape *shape, int rot){
	Shape sh;
	while(rot){
		for(int y = 0; y < MAX_POINTS; y++){
			for(int x = 0; x < MAX_POINTS; x++){
				if(x + shape->x >= FLD_WIDTH - 1)
					shape->x--;
				if(x + shape->x <= 0)
					shape->x++;
				if(lines[shape->y + y][shape->x + x] == CHAR_LINES)
					return;

				sh.points[y * MAX_POINTS + x] = shape->points[x * MAX_POINTS + ((MAX_POINTS - 1) - y)];
			}
		}
		wcsncpy(shape->points, sh.points, MPP);
		rot--;
	}
}

void init_shape(Shape *shape, int x, int y){
	int rotate = rand() % 3;

	shape->x = x;
	shape->y = y;
	memcpy(shape->points, shapes[rand() % MAX_SHAPES],  sizeof(wchar_t) * MPP);
	rotate_shape(shape, rotate);
}

// Удаление полных линий
void fool_lines(){
	int y = FLD_HEIGHT - 1;
	while(y){
		int del_line = 1;
		for(int x = 1; x < FLD_WIDTH - 2; x++){
			if(lines[y][x] != CHAR_LINES){
				del_line = 0;
				y--;
				break;
			}
		}
		if(del_line){
			for(int j = y; j > 0; j--)
				for(int i = 0; i < FLD_WIDTH; i++){
					lines[j][i] = lines[j - 1][i];
				}
			score++;
		}
	}
	level = score / UPLEVEL;
}

void clear_fld(){
	fld[0][0] = CHAR_VERTLINE;
	fld[0][FLD_WIDTH - 1] = CHAR_VERTLINE;

	for(int i = 1; i < FLD_WIDTH - 1; i++)
		fld[0][i] = CHAR_EMPTY;

	for(int i = 1; i < FLD_HEIGHT; i++)
		wcsncpy((void*)&fld[i][0], (void*)&fld[0][0], FLD_WIDTH);

	for(int i = 1; i < FLD_WIDTH - 1; i++)
		fld[FLD_HEIGHT - 1][i] = CHAR_HORIZLINE;
}

void init_lines(){
	lines[0][FLD_WIDTH] = '\0';
	for(int i = 1; i < FLD_WIDTH - 1; i++)
		lines[0][i] = CHAR_EMPTY;

	for(int i = 1; i < FLD_HEIGHT; i++)
		wcsncpy(lines[i], lines[0], FLD_WIDTH - 1);
}

void show_field(){
	for(int i = 0; i < FLD_HEIGHT; i++){
		if(i == 2){
			wprintf(L"%ls  lines: %d", fld[i], score);
		}else if(i == 3){
			wprintf(L"%ls  level: %d", fld[i], level);
		}else if(i >= 4 && i < 8){
			wprintf(L"%ls  ", fld[i]);
			for(int n = 0; n < MAX_POINTS; n++)
				wprintf(L"%c", next.points[(i - MAX_POINTS) * MAX_POINTS + n]);
		}else{
			wprintf(L"%ls", fld[i]);
		}
		wprintf(L"\n");
	}
}

void put_shape(Shape *shape){
	for(int i = 0; i < MAX_POINTS; i++){
		for(int j = 0; j < MAX_POINTS; j++){
			if(shape->points[ i * MAX_POINTS + j ] == L'X')
				fld[(shape->y + i)][shape->x + j] = SHAPE_SYMBOL;
		}
	}
}

void put_lines(){
	for(int y = 1; y < FLD_HEIGHT - 1; y++){
		for(int x = 1; x < FLD_WIDTH - 1; x++){
			if(lines[y][x] == CHAR_LINES){
				fld[y][x] = CHAR_LINES;
			}
		}
	}
}

int collision(Shape *shape, int x, int y){
	int w = 0;
	int h = 0;
	int left = MAX_POINTS;
	wchar_t *points = shape->points;

	// Измеряем ширину, высоту, левый край
	for(int i = MAX_POINTS - 1; i >= 0; i -= 1)
		for(int j = MAX_POINTS - 1; j >= 0; j -= 1)
			if(points[i * MAX_POINTS + j] != CHAR_EMPTY){
				if(i > h) h = i; // Высота
				if(j > w) w = j; //break;
				if(left > j) left = j; // left
			}

	// Крвя и низ стакана
	if((x + w >= FLD_WIDTH - 1) || (x + left <= 0))
		return 1;
	if(y + h >= FLD_HEIGHT - 1)
		return 1;

	// столкновения с lines
	for(int j = h; j >= 0; j--){
		for(int i = 0; i <= w; i++){
			if(points[j * MAX_POINTS + i] != CHAR_EMPTY) {
				if(lines[y + j][x + i] == CHAR_LINES)
					return 1;
			}
		}
	}
	return 0;
}

void add_shape_lines(Shape *shape){
	for(int i = 0; i < MAX_POINTS; i++){
		for(int j = 0; j < MAX_POINTS; j++){
			if(shape->points[i * MAX_POINTS + j] == SHAPE_SYMBOL)
				lines[shape->y + i][shape->x + j] = CHAR_LINES;
		}
	}
	fool_lines();
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
		case DOWN: // Падать вниз
			if(!collision(shape, shape->x, shape->y + 1)){
				shape->y++;
			}else{
				add_shape_lines(shape);
				wcsncpy(shape->points, next.points, MPP);
				//memcpy(shape->points, next.points, sizeof(wchar_t) * MPP);
				shape->x = next.x;
				shape->y = next.y;
				init_shape(&next, FLD_WIDTH / 2, 0);
				return 0;
			}
			break;
		case ROTATE:
			rotate_shape(shape, 1);
			break;
	}
	return 1;
}

int the_end(){
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	wprintf(L"THE END\n");
	//for(int h = 0; FLD_HEIGHT - 1; h++)
		//wprintf(L"%s\n", fld[h]);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){
	Shape obj;
	char ch;
	fd_set fd_r;
	struct timeval tv;

	setlocale(LC_ALL, "");

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);


	srand(time(NULL));
	init_lines();
	init_shape(&obj, FLD_WIDTH / 2, 0);
	init_shape(&next, FLD_WIDTH / 2, 0);
	FD_ZERO(&fd_r);
	
	do{
		FD_SET(0, &fd_r);
		tv.tv_sec = 0;
		tv.tv_usec = 1000000 - level * 50000;

		wprintf (L"\033[0d\033[2J");
		wprintf (L"\n");

		clear_fld();
		put_shape(&obj);
		put_lines();
		show_field();

		int sel = select(1, &fd_r, NULL, NULL, &tv);
		if(sel == -1){
			perror("select:");
			the_end();
		}else if( sel == 0){ // 0 - время вышло. не 0 есть ввод
			if(m_pause < 0) move(&obj, DOWN); // ВНИЗ
		}else{
			read(0, &ch, 1);
			if(m_pause < 0){
				if(ch == 'a') move(&obj, LEFT); // ЛЕВО
				if(ch == 'd') move(&obj, RIGHT); // ПРАВО
				if(ch == 's') move(&obj, DOWN); // ВНИЗ
				if(ch == 'w') move(&obj, ROTATE); // Поворот
				if(ch == ' ') while(move(&obj, DOWN)); // УПАСТЬ
			}
			if(ch == 'p') m_pause *= -1; // PAUSE
		}
	}while(ch != 'q');

	the_end();
	return 0;
}

