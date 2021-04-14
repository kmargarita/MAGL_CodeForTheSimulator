/* Main.c */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "API.h"
#include "uthash.h"

// Mode (0: Flood-Fill, 1: A*, 2: Left-Wall Follower)
int Mode = 0;
// Return_Home (0:false, 1: true)
int Return_Home = 0;


// Dimensions(8,6) goal at (3,2),(3,3),(4,2),(4,3). Start at (0,0).
int x = 0;
int y = 0;
int xprev=0;
int yprev=0;
int o = 0; // orientation
int x0,y_0,x1,y_1,x2,y2,x3,y3;
int L,R,F;
//y1 & y0 are built-in functions of the gcc compiler -> y_0,y_1

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

/*
int cells[8][6] = {{0,0,0,0,0,0},
                   {0,0,0,0,0,0},
                   {0,0,0,0,0,0},
                   {0,0,0,0,0,0},
                   {0,0,0,0,0,0},
                   {0,0,0,0,0,0},
                   {0,0,0,0,0,0},
                   {0,0,0,0,0,0}};

// Default Flood for a wall-less 8x6 Matrix, goal(3,2),(3,3),(4,2),(4,3).
int flood[8][6] = {{5,4,3,3,4,5},
                   {4,3,2,2,3,4},
                   {3,2,1,1,2,3},
                   {2,1,0,0,1,2},
                   {2,1,0,0,1,2},
                   {3,2,1,1,2,3},
                   {4,3,2,2,3,4},
                   {5,4,3,3,4,5}};
int cells[8][6] =                 



                {{5,4,3,2,2,3,4,5},
	               {4,3,2,1,1,2,3,4},
		           {3,2,1,0,0,1,2,3},
		           {3,2,1,0,0,1,2,3},
		           {4,3,2,1,1,2,3,4},
		           {5,4,3,2,2,3,4,5}};

*/

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

Updating The Walls: (cells[y][x]=)

o   o     o---o     o   o     o   o     o   o     o   o     o---o     o---o
|     = 1       = 2     | = 3       = 4 |     = 5     | = 6     | = 7 |     = 8 
o   o     o   o     o   o     o---o     o---o     o---o     o   o     o   o

o   o     o---o     o   o     o---o     o---o     o---o     o   o     o---o
|   | = 9       =10 |   | =11     | =12 |   | =13 |     =14       =15 |   | =16. 
o   o     o---o     o   o     o---o     o   o     o---o     o   o     o---o
*/

#define SIZE 300
#define EMPTY -1

struct stack {
    int items[SIZE];
    int top;   
};

void initialize(struct stack* stackPtr) {
     stackPtr->top = -1;
     //
}


// Returns true iff the stack pointed to by stackPtr is full.
int full(struct stack* stackPtr) {
    return (stackPtr->top == SIZE - 1);
    //
}

int push(struct stack* stackPtr, int value) {
    // Check if the stack is full.
    if (full(stackPtr)){
        return 0;
    }
    // Add value to the top of the stack and adjust the value of the top.
    stackPtr->items[stackPtr->top+1] = value;
    (stackPtr->top)++;
    return 1;
}

// Returns true iff the stack pointed to by stackPtr is empty.
int empty(struct stack* stackPtr) {
    return (stackPtr->top == -1);
    //
}

int pop(struct stack* stackPtr) {
    
    int retval;
    // Check the case that the stack is empty.
    if (empty(stackPtr)){
        return EMPTY;
    }
    // Retrieve the item from the top of the stack, adjust the top and return
    // the item.
    retval = stackPtr->items[stackPtr->top];
    (stackPtr->top)--;
    return retval;
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
int top(struct stack* stackPtr) {
    // Take care of the empty case.
    if (empty(stackPtr))
        return EMPTY;
    // Return the desired item.
    return stackPtr->items[stackPtr->top];
}


void updateWalls(int x, int y, int o, int L, int R, int F){ // checked
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
            cells[y][x]=10;
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
        if(o==1){
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
    	    cells[y][x]=4;
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

bool isAccessible(int x, int y, int x1, int y_1){ // checked
    // Returns True if mouse can move to (x1,y_1) from(x,y)
    if(x==x1){
    	if(y>y_1){
    	    if(cells[y][x]== 4 || cells[y][x]== 5 || cells[y][x]== 6 || cells[y][x]== 10 || cells[y][x]== 11 || cells[y][x]== 12 || cells[y][x]== 14){
    	        return(false);//4,5,6,10,11,12,14.
    	    }
    	    else{
    	        return(true);
    	    }
        }
    	else{
    	    if(cells[y][x]== 2 || cells[y][x]== 7 || cells[y][x]== 8 || cells[y][x]== 10 || cells[y][x]== 12 || cells[y][x]== 13 || cells[y][x]== 14){
    		return(false);//2,7,8,10,12,13,14
    	    }
    	    else{
    	        return(true);
    	    }
    	}
    }
    else if(y==y_1){
        if(x>x1){
            if(cells[y][x]==1 || cells[y][x]==5 || cells[y][x]==8 || cells[y][x]==9 || cells[y][x]==11 || cells[y][x]==13 || cells[y][x]==14){
                return (false);//1,5,8,9,11,13,14
            }
            else{
                return (true);
            }
        }
    	else{
            if(cells[y][x]==3 || cells[y][x]==6 || cells[y][x]==7 || cells[y][x]==9 || cells[y][x]==11 || cells[y][x]==12 || cells[y][x]==13){
                return (false);//3,6,7,9,11,12,13
            }
            else{
                return (true);
            }
        }
    }

}
//checked
void getSurrounds(int x, int y, int* x0, int* y_0, int* x1, int* y_1, int* x2, int* y2, int* x3, int* y3){
    // Return x1, y_1, x2, y2, x3, y3, x4, y4 (surrounding square)
    // north,east,south,west.
    *x3=x-1;
    *y3=y;
    *x0=x;
    *y_0=y+1;
    *x1=x+1;
    *y_1=y;
    *x2=x;
    *y2=y-1;
    if(*x1>=6){ // if over border of (8,6) map
        *x1=-1;
    }
    if(*y_0>=8){
        *y_0=-1;
    }
    /*   
         -1
    o---o---o---o
    |     0     | Same notation as orient 'o':
    o   o   o   o    0- North
 -1 | 3   x   1 |    1- East
    o   o   o   o    2- South
    |     2     |    3- West
    o---o---o---o
    */
}

int min(int arr[]){//checked for arr[4]
    int m = arr[0];
    for(int j=0; j<4; j++){
        for(int i=0; i<4; i++){
            if (arr[i] < arr[j] && arr[i]<m){
                m = arr[i];
            }
        }
    }
    return(m);
}


bool isConsistant(int x, int y){//checked
    // Returns True if value of current square is one 
    // greater than the miniumum value in an accessible neighbour
    getSurrounds(x,y,&x0,&y_0,&x1,&y_1,&x2,&y2,&x3,&y3);
    int val = flood[y][x];
    int minVals[4]={-1,-1,-1,-1};
    if(x0>=0 && y_0>=0){
        if(isAccessible(x,y,x0,y_0)){
            minVals[0]=flood[y_0][x0];
        }
    }
    if(x1>=0 && y_1>=0){
        if(isAccessible(x,y,x1,y_1)){
            minVals[1]=flood[y_1][x1];
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
    for(int i = 0; i<4; i++){
    	if(minVals[i]==-1){
    	}
    	else if(minVals[i]==val+1){
    	}
    	else if(minVals[i]==val-1){
    	    minCount++;
    	}
    }
    if(minCount>0){
        return(true);
    }
    else{
        return(false);
    }
}

void makeConsistant(int x, int y){ //checked
    getSurrounds(x,y,&x0,&y_0,&x1,&y_1,&x2,&y2,&x3,&y3);
    int findmin[4]={-1,-1,-1,-1};
    if(x0>=0 && y_0>=0){
    	if(isAccessible(x, y, x0, y_0)){
            findmin[0]=flood[y_0][x0];
    	}
    }
    if(x1>=0 && y_1>=0){
        if(isAccessible(x, y, x1, y_1)){
            findmin[1]=flood[y_1][x1];
        }
    }
    if(x2>=0 && y2>=0){
        if(isAccessible(x, y, x2, y2)){
            findmin[2]=flood[y2][x2];
        }
    }
    if(x3>=0 && y3>=0){
        if(isAccessible(x, y, x3, y3)){
            findmin[3]=flood[y3][x3];
        }
    }
    for(int i=0; i<4;i++){
        if(findmin[i]==-1){
            findmin[i]=1000;
        }
    }
    int minVal=min(findmin);
    fprintf(stderr, "makeConsistant\n {%d, %d, %d, %d}, min:%d\n",findmin[0],findmin[1],findmin[2],findmin[3], minVal );
    flood[y][x]=minVal+1;
}

void floodFill(int x, int y, int xprev, int yprev){
    // Updates Flood[][] to current cell(x,y).
    if(!isConsistant(x,y)){
        flood[y][x]=flood[yprev][xprev]+1;
    }
    int count = 0;
    struct stack Cellstack;
    initialize(&Cellstack);
    push(&Cellstack, x);
    push(&Cellstack, y);
    count +=2;
    getSurrounds(x,y,&x0,&y_0,&x1,&y_1,&x2,&y2,&x3,&y3);
    // Append stack surrounding varibles:
    if(x0>=0 && y_0>=0){
        if(isAccessible(x,y,x0,y_0)){
            push(&Cellstack, x0);
            push(&Cellstack, y_0);
            count +=2;
        }
    }
    if(x1>=0 && y_1>=0){
        if(isAccessible(x,y,x1,y_1)){
            push(&Cellstack, x1);
            push(&Cellstack, y_1);
            count +=2;
        }
    }
    if(x2>=0 && y2>=0){
        if(isAccessible(x,y,x2,y2)){
            push(&Cellstack, x2);
            push(&Cellstack, y2);
            count +=2;
        }
    }
    if(x3>=0 && y3>=0){
        if(isAccessible(x,y,x3,y3)){
            push(&Cellstack, x3);
            push(&Cellstack, y3);
            count +=2;
        }
    }
    fprintf(stderr, "sizeof(Cellstack)/sizeof(int):%ld\n", sizeof(Cellstack)/sizeof(int));
    fprintf(stderr, "Cellstack:\n");
    fprintf(stderr, "count of stack: %d\n", count);
    while(!empty(&Cellstack)){
        int yrun = pop(&Cellstack);
        int xrun = pop(&Cellstack);
        count -=2;
        //fprintf(stderr, "Cellstack.items[%d]: %d\n", count, Cellstack.items[count]);
        if(isConsistant(xrun,yrun)){
            //break;
        }
        else{
            makeConsistant(xrun, yrun);
            push(&Cellstack, xrun);
            push(&Cellstack, yrun);
            count +=2;
            getSurrounds(xrun,yrun,&x0,&y_0,&x1,&y_1,&x2,&y2,&x3,&y3);
            if(x0>=0 && y_0>=0){
                if(isAccessible(xrun,yrun,x0,y_0)){
                    push(&Cellstack, x0);
                    push(&Cellstack, y_0);
                    count +=2;
                }
            }
            else if(x1>=0 && y_1>=0){
                if(isAccessible(xrun,yrun,x1,y_1)){
                    push(&Cellstack, x1);
                    push(&Cellstack, y_1);
                    count +=2;
                }
            }
            else if(x2>=0 && y2>=0){
                if(isAccessible(xrun,yrun,x2,y2)){
                    push(&Cellstack, x2);
                    push(&Cellstack, y2);
                    count +=2;
                }
            }
            else if(x3>=0 && y3>=0){
                if(isAccessible(xrun,yrun,x3,y3)){
                    push(&Cellstack, x3);
                    push(&Cellstack, y3);
                    count +=2;
                }
            }
            
            // remove all values at the end
            /*
            if(count>=10){
                break;
            }
            */
            
        }
        fprintf(stderr, "count of stack: %d\n", count);
    }
    /*
    while(!empty(&Cellstack)){
        int remove = pop(&Cellstack);
    }
    */
}


char toMove(int x, int y, int xprev, int yprev, int o){
    getSurrounds(x,y,&x0,&y_0,&x1,&y_1,&x2,&y2,&x3,&y3);
    fprintf(stderr, "\n");
    int prev = 0;
    long int minVals[4] = {1000,1000,1000,1000};
    if(isAccessible(x,y,x0,y_0)){
        if((x0 == xprev) && (y_0 == yprev)){
            prev = 0;
        }
        else{}
        minVals[0]=flood[y_0][x0];
    }
    if(isAccessible(x,y,x1,y_1)){
        if((x1 == xprev) && (y_1 == yprev)){
            prev = 1;
        }
        else{}
        minVals[1]=flood[y_1][x1];
    }
    if(isAccessible(x,y,x2,y2)){
        if((x2 == xprev) && (y2 == yprev)){
            prev = 2;
        }
        else{}
        minVals[2]=flood[y2][x2];
    }
    if(isAccessible(x,y,x3,y3)){
        if((x3 == xprev) && (y3 == yprev)){
            prev = 3;
        }
        else{}
        minVals[3]=flood[y3][x3];
    }
    int minVal=minVals[0]; //flood[y_0][x0]
    int minCell=0;
    int noMovements=0;

    fprintf(stderr,"to move minVals[]:{%ld, %ld, %ld, %ld} %ld, flood[%d][%d]:%d\n", minVals[0], minVals[1], minVals[2], minVals[3], (sizeof(&minVals)/sizeof(int)), y_0, x0,flood[y_0][x0]);
    
    for(int i=0; i<4; i++){ // i in minvals
        if(minVals[i]!=1000){
            noMovements++;
        }
        else{
            fprintf(stderr,"noMovements:%d\n", noMovements);
        }
    }
    fprintf(stderr,"minVal:%d\n", minVal);
    for(int i=0; i<4; i++){
        if(minVals[i]<minVal){
            if(noMovements==1){
                minVal = minVals[i];
                minCell = i;
            }
            else{
                if(i==prev){
                }
                else{
                    minVal=minVals[i];
                    minCell = i;
                }
            }
        }
        else{
            //nothing
        }
    }

    fprintf(stderr,"minCell:%d, o:%d\n", minCell,o);
    if(minCell==o){
        fprintf(stderr, "move, 'F'\n"); // 1
        return('F');
    }
    else if((minCell==o-1)||(minCell==o+3)){//0,4
        fprintf(stderr, "move, 'L'\n");
        return('L');
    }
    else if((minCell==o+1)||(minCell==o-3)){ //2,-3
        fprintf(stderr, "move, 'R'\n");
        return('R');
    }
    else{
        fprintf(stderr, "move, 'B'\n");// minCell=3
        return('B');
    }
}

/*
char toMoveBack(int x, int y, int xprev, int yprev, int o){
    getSurrounds(x,y,&x0,&y_0,&x1,&y_1,&x2,&y2,&x3,&y3);
    //fprintf(stderr, "When robot is at (%d,%d):\n\tN(%d,%d), \nW(%d,%d)\t\tE(%d,%d), \n\tS(%d,%d)\n", x,y,x0,y_0,x3,y3,x1,y_1,x2,y2);
    int prev = 0;
    int minVals[4] = {1000,1000,1000,1000};
    if(isAccessible(x,y,x0,y_0)){
        if(x0 == xprev && y_0 == yprev){
            prev = 0;
        }
        minVals[0]=flood[y_0][x0];
    }
    if(isAccessible(x,y,x1,y_1)){
        if(x1 == xprev && y_1 == yprev){
            prev = 1;
        }
        minVals[1]=flood[y_1][x1];
    }
    if(isAccessible(x,y,x2,y2)){
        if(x2 == xprev && y2 == yprev){
            prev = 2;
        }
        minVals[2]=flood[y2][x2];
    }
    if(isAccessible(x,y,x3,y3)){
        if(x3 == xprev && y3 == yprev){
            prev = 3;
        }
        minVals[3]=flood[y3][x3];
    }
    int minVal=minVals[0];
    int minCell=0;
    int noMovements=0;
    for(int i=0; i < sizeof(minVal); i++){ // May Cause Problem
        if(i!=1000){
            noMovements++;
        }
    }
    for(int i=0; i<4; i++){
        if((minVals[i]!=1000) && (minVals[i]>minVal)){
            if(noMovements==1){
                minVal = minVals[i];
                minCell = i;
            }
            else{
                if(i==prev){
                    break;
                }
                else{
                    minVal=minVals[i];
                    minCell = i;
                }
            }
        }
    }
    if(minCell==o){
        fprintf(stderr, "move Back, 'F'\n");
        return('F');
    }
    else if((minCell==o-1)||(minCell==o+3)){
        fprintf(stderr, "move Back, 'L'\n");
        return('L');
    }
    else if((minCell==o+1)||(minCell==o-3)){
        fprintf(stderr, "move Back, 'R'\n");
        return('R');
    }
    else{
        fprintf(stderr, "move Back, 'B'\n");
        return('B');
    }
}
*/

void showFlood(int xrun, int yrun){ //checked
    for(int i=0; i<6;i++){
        for(int j=0; j<8; j++){
            char floodVal[i];
            itoa(flood[i][j], floodVal, 10); // 10 is decimal
            //fprintf(stderr, "floodVal: %s\n", floodVal);
            API_setText(j, i, floodVal);
        }
    }
    /*print floodfill to std err
    for(int i=5; i>=0;i--){
        for(int j=0; j<8; j++){
            fprintf(stderr, "[%d]", flood[i][j]);
        }
    fprintf(stderr, "\n");
    }*/
}


void logTxt(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

int main(int argc, char* argv[]) {
    int dmw = API_mazeWidth(); // default is 16
    int dmh = API_mazeHeight(); // default is 16
    // fprintf(stderr, "Maze Width, Maze Height = (%d,%d)\n", mw, mh);
    int mw = 8;
    int mh = 6;
    logTxt("Running...");
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "abc");

    /* testing get surrounds
    int x=1;
    int y=1;
    int x0,y_0,x1,y_1,x2,y2,x3,y3;
    getSurrounds(x,y,&x0,&y_0,&x1,&y_1,&x2,&y2,&x3,&y3);
    fprintf(stderr, "When robot is at (%d,%d):\n\tN(%d,%d), \nW(%d,%d)\t\tE(%d,%d), \n\tS(%d,%d)\n", x,y,x0,y_0,x3,y3,x1,y_1,x2,y2);
    // end test */
    
    while (1) {

        // Update the walls
        F = API_wallFront();
        R = API_wallRight();
        L = API_wallLeft();
        updateWalls(x, y, o, L, R, F); //update cells[y][x]
        //Print Coordinates
        
        if (flood[y][x] != 0){ // if not at the centre
            floodFill(x, y, xprev, yprev);//here
        }
        else{ // if at centre
            return(0); // end program.
        }
        
        char direction;
        direction = toMove(x, y, xprev, yprev, o);

        if(direction=='L'){
            API_turnLeft();
            o = API_orientation(o, 'L');
        }
        else if(direction=='R'){
            API_turnRight();
            o = API_orientation(o, 'R');
        }
        else if(direction =='B'){ 
            //go back by turing left twice
            API_turnLeft();
            o = API_orientation(o, 'L');
            API_turnLeft();
            o = API_orientation(o, 'L');
        } 
        //logTxt("Im moving forward now");1
        fprintf(stderr, "\nAPI_updateCoordinates:\n");
        fprintf(stderr, "Flood:\n   [%d]\n[%d][%d][%d]\n   [%d]\n", flood[x0][y_0],flood[y3][x3],flood[y][x],flood[y_1][x1],flood[y2][x2]);
        fprintf(stderr, "o=%d,L=%d, R=%d, F=%d\n\tN(%d, %d),\nW(%d, %d), X(%d, %d),  E(%d, %d),\n\tS(%d, %d)\n", o, L, R, F,x0,y_0,x3,y3,x,y,x1,y_1,x2,y2);
        if(cells[y][x]==1){
            fprintf(stderr, "o   o\n|    \no   o\n");
        }
        if(cells[y][x]==2){
            fprintf(stderr, "o---o\n     \no   o\n");
        }
        if(cells[y][x]==3){
            fprintf(stderr, "o   o\n    |\no   o\n");
        }
        if(cells[y][x]==4){
            fprintf(stderr, "o   o\n     \no---o\n");
        }
        if(cells[y][x]==5){
            fprintf(stderr, "o   o\n|    \no---o\n");
        }
        if(cells[y][x]==6){
            fprintf(stderr, "o   o\n    |\no---o\n");
        }
        if(cells[y][x]==7){
            fprintf(stderr, "o---o\n    |\no   o\n");
        }
        if(cells[y][x]==8){
            fprintf(stderr, "o---o\n|    \no   o\n");
        }
        if(cells[y][x]==9){
            fprintf(stderr, "o   o\n|   |\no   o\n");
        }
        if(cells[y][x]==10){
            fprintf(stderr, "o---o\n     \no---o\n");
        }
        if(cells[y][x]==11){
            fprintf(stderr, "o   o\n|   |\no   o\n");
        }
        if(cells[y][x]==12){
            fprintf(stderr, "o---o\n    |\no---o\n");
        }
        if(cells[y][x]==13){
            fprintf(stderr, "o---o\n|   |\no   o\n");
        }
        if(cells[y][x]==14){
            fprintf(stderr, "o---o\n|    \no---o\n");
        }
        if(cells[y][x]==15){
            fprintf(stderr, "o   o\n     \no   o\n");
        }
        if(cells[y][x]==16){
            fprintf(stderr, "o---o\n|   |\no---o\n");
        }
        showFlood(x,y);
        API_moveForward();
        xprev = x;
        yprev = y;

        // Update Coordinates
        if(o==0){
            y++;
        }
        if(o==1){
            x++;
        }
        if(o==2){
            y--;
        }
        if(o==3){
            x--;
        }
        logTxt("------------------------------");
        
        /* Left wall Follower:

        if (!API_wallLeft()) {
            API_turnLeft();
        }
        while (API_wallFront()) {
            API_turnRight();
        }
        API_moveForward();*/

    }
}
