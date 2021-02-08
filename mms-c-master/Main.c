/*Main.c*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "API.h"
#include "uthash.h"

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
Weight from goal, adapted from "8by6Maze.txt":
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
*/

// Implement an int hash table to append int "arrays" in C.
struct number_hash{
    int value;
    int index;
    UT_hash_handle hh; /* Makes structure hashable*/
};


























/*
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
	    cells[y][x]=5;
	}
    }
    else if(R && F && !L){
        if(o==0){
	    cells[y][x]=7;
	}
	else if(o==1){
	    cells[y][x]=6;
	}
	else if(o==2){
	    cells[y][x]=5;
	}
	else if(o==3){
	    cells[y][x]=8;
	}
    }
    else if(F){
        if(o==0){
	    cells[y][x]=2;
	}
	else if(o==1){
	    cells[y][x]=3;
	}
	else if(o==2){
	    cells[y][x]=4;
	}
	else if(o==3){
	    cells[y][x]=1;
	}
    }
    else if(L){
        if(o==0){
	    cells[y][x]=1;
	}
	else if(o==1){
	    cells[y][x]=2;
	}
	else if(o==2){
	    cells[y][x]=3;
	}
	else if(o==3){
	    cells[y][x]=3;
	}
    }
    else if(R){
        if(o==0){
	    cells[y][x]=3;
	}
	else if(o==1){
	    cells[y][x]=4;
	}
	else if(o==2){
	    cells[y][x]=1;
	}
	else if(o==3){
	    cells[y][x]=2;
	}
    }

}

bool isAccessible(int x, int y, int x1, int y1){
    // Returns True if mouse can move to (x1,y1) from(x,y)
    if(x==x1){
	if(y>y1){
	    if(cells[y][x]== 4 || cells[y][x]== 5 || cells[y][x]== 6 || cells[y][x]== 10 || cells[y][x]== 11 || cells[y][x]== 12 || cells[y][x]== 14){
	        //4,5,6,10,11,12,14.
	        return(false);
	    }
	    else{
	        return(true);
	    }

    	}
	else{
	    if(cells[y][x]== 2 || cells[y][x]== 7 || cells[y][x]== 8 || cells[y][x]== 10 || cells[y][x]== 12 || cells[y][x]== 13 || cells[y][x]== 14){
	        //2,7,8,10,12,13,14
		return(false);
	    }
	    else{
	        return(true);
	    }
	}
    }

    else if(y==y1){
        //
        if(x>x1){
            if(cells[y][x]==1 || cells[y][x]==5 || cells[y][x]==8 || cells[y][x]==9 || cells[y][x]==11 || cells[y][x]==13 || cells[y][x]==14){
                return (false);
	    }
            else{
                return (true);
	    }
	}
	else{
            if(cells[y][x]==3 || cells[y][x]==6 || cells[y][x]==7 || cells[y][x]==9 || cells[y][x]==11 || cells[y][x]==12 || cells[y][x]==13){
                return (false);
	    }
            else{
                return (true);
	    }
	}
    }

}

void getSurrounds(int x, int y, int* x0, int* y0, int* x1, int* y1, int* x2, int* y2, int* x3, int* y3){
    // Return x1, y1, x2, y2, x3, y3, x4, y4 (surrounding square)
    // north,east,south,west.
    *x3=x-1;
    *y3=y;
    *x0=x;
    *y0=y+1;
    *x1=x+1;
    *y1=y;
    *x2=x;
    *y2=y-1;
    if(*x1>=16){
        *x1=-1;
    }
    if(*y0>=16){
        *y0=-1;
    }
}

int min(int arr[]){
    int m;
    for(int j=0; arr[j] != '\0'; j++){
        for(int i=0; arr[i] != '\0'; i++){
            if (arr[i] < arr[j]){
                m = arr[i];
	    }
        }
    }
    return(m);
}


bool isConsistant(int x, int y){
    // Returns True if value of current square is one 
    // greater than the miniumum value in an accessible neighbour
    int x0,y0,x1,y1,x2,y2,x3,y3;
    getSurrounds(x,y,&x0,&y0,&x1,&y1,&x2,&y2,&x3,&y3);
    /* Location Test
    fprintf(stderr, "When robot is at (%d,%d):\n\tN(%d,%d), \nW(%d,%d)\t\tE(%d,%d), \n\tS(%d,%d)\n", x,y,x0,y0,x3,y3,x1,y1,x2,y2);
    */
    int val= flood[x][y];
    int minVals[4]={-1,-1,-1,-1};
    if(x0>=0 && y0>=0){
        if(isAccessible(x,y,x0,y0)){
	    minVals[0]=flood[y0][x0];
        }
    }
    if(x1>=0 && y1>=0){
        if(isAccessible(x,y,x1,y1)){
            minVals[1]=flood[y1][x1];
	}
    }
    if(x2>=0 && y2>=0){
        if(isAccessible(x,y,x2,y2)){
            minVals[2]=flood[y2][x2];
	}
    }
    if(x3>=0 && y3>=0){
        if(isAccessible(x,y,x3,y3)){
            minVals[3]=flood[y3][x3];
	}
    }

    int minCount = 0;
    for(int i = 0; i<4;i++){
        //
	if(minVals[i]==-1){
	    //
	}
	else if(minVals[i]==val+1){
	    //
	}
	else if(minVals[i]==val-1){
	    minCount++;
	    //
	}
    }
    //minVal=min(minVals)
    //return(minVal)
    if(minCount>0){
        return(true);
    }
    else{
        return(false);
    }
}

void makeConsistant(int x, int y){
    int x0,y0,x1,y1,x2,y2,x3,y3;
    getSurrounds(x,y,&x0,&y0,&x1,&y1,&x2,&y2,&x3,&y3);
    int val=flood[y][x];
    int minVals[4]={-1,-1,-1,-1};
    if(x0>=0 && y0>=0){
	if(isAccessible(x, y, x0, y0)){
            minVals[0]=flood[y0][x0];
	    // if(flood[y0][x0]<minVal){}
	}
    }
    if(x1>=0 && y1>=0){
        if(isAccessible(x, y, x1, y1)){
            minVals[1]=flood[y1][x1];
            // if(flood[y1][x1]<minVal){}
	}
    }
    if(x2>=0 && y2>=0){
        if(isAccessible(x, y, x2, y2)){
            minVals[2]=flood[y2][x2];
            // if(flood[y2][x2]<minVal){}
	}
    }
    if(x3>=0 && y3>=0){
        if(isAccessible(x, y, x3, y3)){
            minVals[3]=flood[y3][x3];
            // if(flood[y3][x3]<minVal){}
	}
    }
    for(int i=0;i<4;i++){
        if(minVals[i]==-1){
	    minVals[i]=1000;
	}
    }
    int minVal=min(minVals);// New Function above - int min(int arr)
    flood[y][x]=minVal+1;
}

void floodFill(x,y,xprev,yprev){
    // Updates Flood[][] to current cell(x,y).
    if(!isConsistant(x,y)){
        //
	flood[y][x]=flood[yprev][xprev]+1;
    }
    int stack[]; // New Function above - append(int*,  int )
    append(stack, ) //stack append(x), stack append(y)

}




void logTxt(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

int main(int argc, char* argv[]) {
    logTxt("Running...");
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");
    //testing
    int x=1;
    int y=1;
    int x0,y0,x1,y1,x2,y2,x3,y3;
    getSurrounds(x,y,&x0,&y0,&x1,&y1,&x2,&y2,&x3,&y3);
    fprintf(stderr, "When robot is at (%d,%d):\n\tN(%d,%d), \nW(%d,%d)\t\tE(%d,%d), \n\tS(%d,%d)\n", x,y,x0,y0,x3,y3,x1,y1,x2,y2);
    // end test
    
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
