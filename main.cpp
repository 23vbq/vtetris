#define ERROR_COLOR 12
#define DEFAULT_COLOR 7
#define G_WIDTH 11
#define G_HEIGHT 20

#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <windows.h>

#include "vector2.h"

using namespace std;

HANDLE hConsole;
bool game_loop = true;
int input;

int Blocks[G_HEIGHT][G_WIDTH] = { {0} };
int** renderbuffer;

Vector2 currentPos = Vector2::zero;
vector<Vector2> currentBlocks = vector<Vector2>();
int currentColor = 0;

void NewCurrentBlock();
void MoveDown();
void MakeCurrentStationary();
void RenderScreen();

int main(){
    // Allocate render buffer
    renderbuffer = (int**)malloc(G_HEIGHT * sizeof(int*));
    for (int i = 0; i < G_HEIGHT; i++)
        renderbuffer[i] = (int*)malloc(G_WIDTH * sizeof(int));

    // Prepare variables
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    NewCurrentBlock();
    // Game loop
    RenderScreen();
    while (game_loop){
        MoveDown();
        RenderScreen();
        Sleep(250);
    }
    // Free renderbuffer
    for (int i = 0; i < G_HEIGHT; i++)
        free(renderbuffer[i]);
    free(renderbuffer);
    return 0;
}

void NewCurrentBlock(){
    currentPos = Vector2(G_WIDTH / 2, 0);
    currentBlocks.clear();
    currentBlocks.push_back(Vector2(0, 0));
    currentColor = 4;
}
void MoveDown(){
    for(Vector2 &current : currentBlocks){
        Vector2 blockPos = current + currentPos;
        if (blockPos.y >= G_HEIGHT || Blocks[(int)blockPos.y + 1][(int)blockPos.x] != 0){
            MakeCurrentStationary();
            return;
        }
    }
    currentPos.y += 1;
}
void MakeCurrentStationary(){
    for(Vector2 &current : currentBlocks){
        Blocks[(int)current.y + (int)currentPos.y][(int)current.x + (int)currentPos.x] = currentColor;
    }
    NewCurrentBlock();
}
void RenderScreen(){
    // Copy blocks data to buffer
    for (int i = 0; i < G_HEIGHT; i++)
        memcpy(&renderbuffer[i][0], &Blocks[i][0], G_WIDTH * sizeof(int));
    // Copy current to buffer
    for (Vector2 &current : currentBlocks){
        renderbuffer[(int)current.y + (int)currentPos.y][(int)current.x + (int)currentPos.x] = currentColor;
    }
    system("cls");
    for (int i = 0; i < G_HEIGHT; i++){
        printf("#");
        for (int j = 0; j < G_WIDTH; j++){
            if (renderbuffer[i][j] == 0){
                printf("  ");
                continue;
            }
            SetConsoleTextAttribute(hConsole, renderbuffer[i][j]);
            printf("[]");
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR);
        }
        printf("#\n");
    }
    // cout.flush();
}