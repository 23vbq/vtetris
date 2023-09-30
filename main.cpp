#define ERROR_COLOR 12
#define DEFAULT_COLOR 7
#define G_WIDTH 11
#define G_HEIGHT 20

#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>

#include <windows.h>

#include "vector2.h"

using namespace std;

HANDLE hConsole;
bool game_loop = true;
char input;

int Blocks[G_HEIGHT][G_WIDTH] = { {0} };
int** renderbuffer;

Vector2 currentPos = Vector2::zero;
vector<Vector2> currentBlocks = vector<Vector2>();
int currentColor = 0;
int currentMaxOffsets[3] = {0, 0, 0}; // Left, Right, Down

bool InputHandler();
void NewCurrentBlock();
void MoveDown();
void MoveLeft();
void MoveRight();
void MakeCurrentStationary();
bool IsRowValid(int &row);
void ClearRow(int &row);
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
        InputHandler();
        MoveDown();
        RenderScreen();
        Sleep(100);
    }
    // Free renderbuffer
    for (int i = 0; i < G_HEIGHT; i++)
        free(renderbuffer[i]);
    free(renderbuffer);
    return 0;
}

bool InputHandler(){
    if (!_kbhit()) return false;

    input = _getch();
    if (input == 3){
        game_loop = false;
        return true;
    }
    if (input < 0){
        input = _getch();
        if (input == 75) MoveLeft();
        else if (input == 77) MoveRight();
        return true;
    }
    return false;
}
void NewCurrentBlock(){
    currentPos = Vector2(G_WIDTH / 2, 0);
    currentBlocks.clear();
    currentBlocks.push_back(Vector2(0, 0));
    // currentBlocks.push_back(Vector2(-1, 0));
    // currentBlocks.push_back(Vector2(1, 0));
    currentColor = 4;
    currentMaxOffsets[0] = 0;
    currentMaxOffsets[1] = 0;
    currentMaxOffsets[2] = 0;
}
void MoveDown(){
    currentPos.y += 1;
    for(Vector2 &current : currentBlocks){
        Vector2 blockPos = current + currentPos;
        if (blockPos.y >= G_HEIGHT || Blocks[(int)blockPos.y + 1][(int)blockPos.x] != 0){
            MakeCurrentStationary();
            return;
        }
    }
}
void MoveLeft(){
    int pos = currentPos.x + currentMaxOffsets[0];
    if (pos <= 0 || Blocks[(int)currentPos.y][pos - 1] != 0) return;
    currentPos.x -= 1;
}
void MoveRight(){
    int pos = currentPos.x + currentMaxOffsets[1];
    if (pos >= G_WIDTH - 1 || Blocks[(int)currentPos.y][pos + 1] != 0) return;
    currentPos.x += 1;
}
void MakeCurrentStationary(){
    set<int> rows = set<int>();
    for(Vector2 &current : currentBlocks){
        Blocks[(int)current.y + (int)currentPos.y][(int)current.x + (int)currentPos.x] = currentColor;
        rows.insert((int)current.y + (int)currentPos.y);
    }
    // Check rows
    for(int row : rows){
        if (IsRowValid(row))
            ClearRow(row);
    }
    NewCurrentBlock();
}
bool IsRowValid(int &row){
    for (int i = 0; i < G_WIDTH; i++)
        if (Blocks[row][i] == 0) return false;
    return true;
}
void ClearRow(int &row){
    for (int i = 0; i < G_WIDTH; i++)
        Blocks[row][i] = 0;
    for (int i = row - 1; i >= 0; i--){
        //Blocks[i + 1][0] = Blocks[i][0];
        memcpy(&Blocks[i + 1][0], &Blocks[i][0], G_WIDTH * sizeof(int));
    }
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