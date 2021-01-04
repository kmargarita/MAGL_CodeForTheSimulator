/*Main.c*/


#include <stdio.h>
#include <stdbool.h>
#include "API.h"

// Dimensions(8,6) goal at (3,2),(3,3),(4,2),(4,3). Start at (0,0).
int x = 0;
int y = 0;
int o = 0; // orientation

int cells[6][8] = {{0,0,0,0,0,0,0,0},
	           {0,0,0,0,0,0,0,0},
		   {0,0,0,0,0,0,0,0},
		   {0,0,0,0,0,0,0,0},
		   {0,0,0,0,0,0,0,0},
		   {0,0,0,0,0,0,0,0}};

// Default Flood for a wall-less 8x6 Matrix, goal(3,2),(3,3),(4,2),(4,3).
int flood[6][8] = {{5,4,3,2,2,3,4,5},
	           {4,3,2,1,1,2,3,4},
		   {3,2,1,0,0,1,2,3},
		   {3,2,1,0,0,1,2,3},
		   {4,3,2,1,1,2,3,4},
		   {5,4,3,2,2,3,4,5}};

// Flood Weightings for "8by6Maze.txt" with walls
/*
int deadReckoning[6][8] = {{10,9,8,7,6,5,4,5},
                           {11,12,15,16,1,2,3,4},
			   {12,13,14,0,0,3,4,5},
			   {13,12,11,0,0,4,5,6},
			   {14,13,10,9,8,7,6,7},
			   {15,12,11,12,9,10,11,12}};
*/

/*
Adapted from "8by6Maze.txt":
o---o---o---o---o---o---o---o---o
|10   9   8   7   6   5   4   5 |
o   o---o   o---o---o---o   o---o
|11  12 |15  16 | 1  *2  *3   4 |
o   o---o   o---o   o   o   o   o
|12 |13  14 | 0   0 | 3 | 4 | 5 |
o   o   o---o   o   o   o   o   o
|13 |12  11 | 0   0 | 4 | 5 | 6 |
o   o   o   o---o---o---o   o---o
|14 |13 |10   9   8   7   6   7 |
o   o---o   o---o   o---o---o---o
|15 |12  11  12 | 9  10  11  12 | The * Denotes robot choice because 
o---o---o---o---o---o---o---o---o there are equal weights surrounding it.

Updating The Walls: (cells[y][x]=)

o   o     o---o     o   o     o   o     o   o     o   o     o---o     o---o
|     = 1       = 2     | = 3       = 4 |     = 5     | = 6     | = 7 |     = 8 
o   o     o   o     o   o     o---o     o---o     o---o     o   o     o   o

o   o     o---o     o   o     o---o     o---o     o---o     o   o     o---o
|   | = 9       =10 |   | =11     | =12 |   | =13 |     =14       =15 |   | =16. 
o   o     o---o     o   o     o---o     o   o     o---o     o   o     o---o

*/

void updateWalls(int x, int y, int o, int L, int R, int F){
    //
    if(L && R && F){
        if(o==0){
	    cells[y][x]=13;
	}
	else if(o==1){
	    cells[y][x]=12;
	}
        else if(o==2){
            cells[y][x]=11;
        } 
        else if(o==3){
            cells[y][x]=14;
        } 
    }
    else if(L && R && !F){ 
        if(o==0 || o==2){
            cells[y][x]=9;
        }
        else if(o==1 || o==3){
            cells[y][x]=1;
        }
    }  
    else if(L && F && !R){
        if(o==0){
            cells[y][x]=8;
        }
        else if(o==1){
            cells[y][x]=7;
        }
	else if(o==2){
	    cells[y][x]=6;
	}
	else if(o==3){
	    cells[y][x]=8;
	}
    }
    else if(R && F && !L){
        if(o==0){
	    cells[y][x]=7;
	}
    }

}

void logTxt(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

int main(int argc, char* argv[]) {
    logTxt("Running...");
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");
    while (1) {
        if (!API_wallLeft()) {
            API_turnLeft();
        }
        while (API_wallFront()) {
            API_turnRight();
        }
        API_moveForward();
    }
}
