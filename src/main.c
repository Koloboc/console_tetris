#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define FLD_HEIGHT 20
#define FLD_WIDTH  30
#define QUANTITY_SHAPES 2
#define WIDTH_SHAPE 4
#define HEIGHT_SHAPE 4

#undef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#undef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct _TObject{
	int x,y;
	int rx,ry;
	int w,h;
	int rw,rh;
	char symbol;
	int rot;
	char *shape;
	int fly;
} TObject;

char fld[FLD_HEIGHT][FLD_WIDTH + 1];
char lines[FLD_HEIGHT - 1][FLD_WIDTH - 2];

char shapes[QUANTITY_SHAPES * (WIDTH_SHAPE * HEIGHT_SHAPE + 1)] = " *   *   *   **  **  **           ";
//																	              X

void init_object(TObject *obj, int _x, int _y, int _w, int _h, int _rot, char _symbol, char *shape){
	obj->x = _x;
	obj->y = _y;
	obj->w = _w;
	obj->h = _h;
	obj->rot = _rot;
	obj->symbol = _symbol;
	obj->shape = shape;
	obj->rx = WIDTH_SHAPE;
	obj->ry = HEIGHT_SHAPE;
	obj->rw = 0;
	obj->rh = 0;
	obj->fly = 1;
	for(int y = 0; y < HEIGHT_SHAPE; y++){
		for(int x = 0; x < WIDTH_SHAPE; x++){
			if(*(obj->shape + y * HEIGHT_SHAPE + x) == '*'){
				obj->rx = MIN(x, obj->rx);
				obj->rw = MAX(x, obj->rw);
			}
		}
	}
	//FILE *fd = fopen("out", "a+");
	//fprintf(fd, "%d", obj->rw);
	//fclose(fd);
}

void clear_fld(){
	fld[0][0] = '|';
	fld[0][FLD_WIDTH - 1] = '|';
	fld[0][FLD_WIDTH] = '\0';

	for(int i = 1; i < FLD_WIDTH - 1; i++)
		fld[0][i] = ' ';

	for(int i = 1; i < FLD_HEIGHT; i++)
		memcpy((void*)&fld[i][0], (void*)&fld[0][0], FLD_WIDTH);

	for(int i = 1; i < FLD_WIDTH - 1; i++)
		fld[FLD_HEIGHT - 1][i] = '=';
}

void show_field(){
	for(int i = 0; i < FLD_HEIGHT; i++)
		printf("%s\n", fld[i]);
}

void put_object(TObject *obj){
	for(int h = 0; h < HEIGHT_SHAPE; h++){
		for(int w = 0; w < WIDTH_SHAPE; w++)
			if(*(obj->shape + h * HEIGHT_SHAPE + w) == '*')
				fld [obj->y + h] [obj->x + w] = obj->symbol;
	}
}

void move_shape(TObject *obj, int direction){
	switch(direction){
		case 0:  // Падение
			if(obj->y + obj->h != FLD_HEIGHT - 1)
				obj->y++;
			break;
		case 1:  // ЛЕВО
			if(obj->rx + obj->x != 1)
				obj->x--;
			break;
		case 2:  // ПРАВО
			if(obj->x + obj->rw != FLD_WIDTH - 2)
				obj->x++;
			break;
		case 3:  // ВНИЗ
			if(obj->y + obj->h != FLD_HEIGHT - 1)
				obj->y++;
			break;
		default:
			return;
	}
}

void check_fly(TObject *obj){

}

int main(int argc, char **argv){
	TObject obj;
	char ch;
	fd_set fd_r;
	struct timeval tv;

	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	init_object(&obj, FLD_WIDTH / 2, 0, WIDTH_SHAPE, HEIGHT_SHAPE, 0, '#', &shapes[0]);
	FD_ZERO(&fd_r);
	
	do{
		FD_SET(0, &fd_r);
		tv.tv_sec = 0;
		tv.tv_usec = 500000;

		printf ("\033[0d\033[2J");
		printf ("\n");

		clear_fld();
		put_object(&obj);
		show_field();

		int sel = select(1, &fd_r, NULL, NULL, &tv);
		if(sel == -1){
			perror("select:");
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
			return 0;
		}else if( sel == 0){ // 0 - время вышло. не 0 есть ввод
			move_shape(&obj, 0); // ВНИЗ
		}else{
			read(0, &ch, 1);
			if(ch == 'a') move_shape(&obj, 1); // ЛЕВО
			if(ch == 'd') move_shape(&obj, 2); // ПРАВО
			if(ch == ' ') move_shape(&obj, 3); // УПАСТЬ
		}
		check_fly(&obj);
	}while(ch != 'q');

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return 0;
}

