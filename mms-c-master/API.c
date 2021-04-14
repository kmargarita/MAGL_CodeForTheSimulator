#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 32

int getInteger(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int value = atoi(response);
    return value;
}

int getBoolean(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int value = (strcmp(response, "true\n") == 0);
    return value;
}

int getAck(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int success = (strcmp(response, "ack\n") == 0);
    return success;
}

int API_mazeWidth() {
    return getInteger("mazeWidth");
}

int API_mazeHeight() {
    return getInteger("mazeHeight");
}

int API_wallFront() {
    return getBoolean("wallFront");
}

int API_wallRight() {
    return getBoolean("wallRight");
}

int API_wallLeft() {
    return getBoolean("wallLeft");
}

int API_moveForward() {
    return getAck("moveForward");
}

void API_turnRight() {
    getAck("turnRight");
}

void API_turnLeft() {
    getAck("turnLeft");
}

void API_setWall(int x, int y, char direction) {
    printf("setWall %d %d %c\n", x, y, direction);
    fflush(stdout);
}

void API_clearWall(int x, int y, char direction) {
    printf("clearWall %d %d %c\n", x, y, direction);
    fflush(stdout);
}

void API_setColor(int x, int y, char color) {
    printf("setColor %d %d %c\n", x, y, color);
    fflush(stdout);
}

void API_clearColor(int x, int y) {
    printf("clearColor %d %d\n", x, y);
    fflush(stdout);
}

void API_clearAllColor() {
    printf("clearAllColor\n");
    fflush(stdout);
}

void API_setText(int x, int y, char* text) {
    printf("setText %d %d %s\n", x, y, text);
    fflush(stdout);
}

void API_clearText(int x, int y) {
    printf("clearText %d %d\n", x, y);
    fflush(stdout);
}

void API_clearAllText() {
    printf("clearAllText\n");
    fflush(stdout);
}

int API_wasReset() {
    return getBoolean("wasReset");
}

void API_ackReset() {
    getAck("ackReset");
}

// Katherine's API functions:

/*  orient:
    0- North
    1- East
    2- South
    3- West
*/
int API_orientation(int orient, char turning){
    if(turning=='L'){
        orient--;
        if(orient == -1){
            orient = 3;
        }
    }
    else if(turning=='R'){
        orient++;
        if(orient==4){
            orient = 0;
        }
    }
    else if(turning=='B'){
        if(orient==0){
            orient = 2;
        }
        else if(orient==1){
            orient=3;
        }
        else if(orient==2){
            orient = 0;
        }
        else if(orient==3){
            orient = 1;
        }
    }
    return orient;
}

/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C"
 * with slight modification to optimize for specific architecture:
 * If itoa doesn't compile, alternatives: 
 * http://www.strudel.org.uk/itoa/
 */
    
void strreverse(char* begin, char* end){
    char aux;
    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}
    
void itoa(int value, char* str, int base){
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr=str;
    int sign;
    div_t res;
    // Validate base
    if (base<2 || base>35){ *wstr='\0'; return; }
    // Take care of sign
    if ((sign=value) < 0) value = -value;
    // Conversion. Number is reversed.
    do {
        res = div(value,base);
        *wstr++ = num[res.rem];
    }while(value=res.quot);
    if(sign<0) *wstr++='-';
    *wstr='\0';
    // Reverse string
    strreverse(str,wstr-1);
}