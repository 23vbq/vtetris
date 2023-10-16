#define ERROR_COLOR 12
#define DEFAULT_COLOR 7
#define G_WIDTH 11
#define G_HEIGHT 20

#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

#include <windows.h>

#include "vector2.h"
#include "Block.h"

using namespace std;

HANDLE hConsole;
bool game_loop = true;
char input;

int score;

int tilemap[G_HEIGHT][G_WIDTH] = { {0} };
int** renderbuffer;
unsigned int blockcolors[] = {
    0x1F, // Blue
    0x2F, // Green
    0x3F, // Cyan (G,B)
    0x4F, // Red
    0x5F, // Purple (R,B)
    0x6F, // (R,G)
    0x7F, // Gray (R,G,B)
};
unsigned short blockcolors_size = 7;

vector<Block> blocks = vector<Block>();

Vector2 currentPos = Vector2::zero;
vector<Vector2> currentBlocks = vector<Vector2>();
unsigned int currentColor = 0;
int currentMaxOffsets[4] = {0}; // Left, Down, Right, Up
bool spawnNew;

void LoadBlockFromFile(string);
bool InputHandler();
void NewCurrentBlock();
bool GameOverCheck();
void MoveDown();
void MoveLeft();
void MoveRight();
void RotateBlock();
bool CanRotateBlock(vector<Vector2>&);
void MakeCurrentStationary();
bool IsRowValid(int&);
void ClearRow(int&);
void RenderScreen();

int main(){
    srand(time(NULL));
    score = 0;
    // Allocate render buffer
    renderbuffer = (int**)malloc(G_HEIGHT * sizeof(int*));
    for (int i = 0; i < G_HEIGHT; i++)
        renderbuffer[i] = (int*)malloc(G_WIDTH * sizeof(int));

    // Prepare variables
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    try{
        LoadBlockFromFile("./blocks.cfg");
    } catch(char const* e){
        printf("Blocks loading: %s", e);
        return 1;
    }
    spawnNew = true;
    // Game loop
    RenderScreen();
    while (game_loop){
        bool spawnNewBuff = spawnNew;
        if (spawnNewBuff){
            NewCurrentBlock();
        }
        InputHandler();
        FlushConsoleInputBuffer(hConsole);
        if (!spawnNewBuff)
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

void LoadBlockFromFile(string path){
    ifstream in(path);
    if (!in.is_open())
        throw "Cannot open file.";
    string line;
    stringstream ssline;
    Block blockbuffer;
    vector<int> vbuff = vector<int>();
    blocks.clear();
    while (getline(in, line)){
        if(line[0] == '#') continue;
        vbuff.clear();
        ssline = stringstream(line);
        while (!ssline.eof()){
            int x;
            ssline>>x;
            vbuff.push_back(x);
        }
        blockbuffer = Block(vbuff);
        blocks.push_back(blockbuffer);
    }
}
bool InputHandler(){
    if (!_kbhit()) return false;

    input = _getch();
    // ctrl+c
    if (input == 3){
        game_loop = false;
        return true;
    }
    // Arrows - move
    if (input < 0){
        input = _getch();
        if (input == 75) MoveLeft();
        else if (input == 77) MoveRight();
        return true;
    }
    // R - rotate
    if (input == 114){
        RotateBlock();
    }
    /*if (input == 116)
        cout<<"aaa";*/
    return false;
}
void NewCurrentBlock(){
    currentPos = Vector2(G_WIDTH / 2, G_HEIGHT - 1);
    currentBlocks.clear();
    Block *bptr = &blocks[rand() % blocks.size()];
    // int kb = 0; // FIXME debug
    // cin>>kb;
    // Block *bptr = &blocks[kb];
    for (Vector2 &child : bptr->GetChilds()){
        currentBlocks.push_back(child);
    }
    currentColor = blockcolors[rand() % blockcolors_size];
    memcpy(&currentMaxOffsets[0], bptr->GetMaxOffsets(), 4 * sizeof(int));
    if (GameOverCheck()){
        game_loop = false; // TODO need more testing , do testów
        currentBlocks.clear();
    }
    spawnNew = false;
}
bool GameOverCheck(){
    for (Vector2 &current : currentBlocks){
        Vector2 pos = current + currentPos;
        if (tilemap[(int)pos.y][(int)pos.x] != 0)
            return true;
    }
    return false;
}
void MoveDown(){
    for(Vector2 &current : currentBlocks){
        Vector2 blockPos = current + currentPos;
        if (blockPos.y <= 0 || tilemap[(int)blockPos.y - 1][(int)blockPos.x] != 0){
            MakeCurrentStationary();
            return;
        }
    }
    currentPos.y -= 1;
}
void MoveLeft(){
    int pos = currentPos.x + currentMaxOffsets[0];
    if (pos <= 0 || tilemap[(int)currentPos.y][pos - 1] != 0) return;
    currentPos.x -= 1;
}
void MoveRight(){
    int pos = currentPos.x + currentMaxOffsets[2];
    if (pos >= G_WIDTH - 1 || tilemap[(int)currentPos.y][pos + 1] != 0) return;
    currentPos.x += 1;
}
void RotateBlock(){
    // Clockwise rotation with check
    vector<Vector2> cbcopy(currentBlocks);
    for (Vector2& child : cbcopy){
        // x = cos(90) * x + -sin(90) * y => 0 * x - 1 * y
        float x = -1 * child.y;
        // y = sin(90) * x + cos(90) * y => 1 * x + 0 * y
        float y = child.x;
        child = Vector2(x, y);
    }
    if (!CanRotateBlock(cbcopy))
        return;
    currentBlocks = cbcopy;
    // Set offsets
    int l = currentMaxOffsets[3];
    currentMaxOffsets[3] = currentMaxOffsets[2];
    currentMaxOffsets[2] = currentMaxOffsets[1] * -1;
    currentMaxOffsets[1] = currentMaxOffsets[0];
    currentMaxOffsets[0] = l * -1;
}
bool CanRotateBlock(vector<Vector2>& cb){
    for (Vector2& child : cb){
        Vector2 pos = child + currentPos;
        if (pos.x >= G_WIDTH)
            return false;
        if (pos.x < 0 || pos.y < 0)
            return false;
        if (tilemap[(int)pos.y][(int)pos.x] != 0)
            return false;
    }
    return true;
}
void MakeCurrentStationary(){
    set<int> rows = set<int>();
    for(Vector2 &current : currentBlocks){
        tilemap[(int)current.y + (int)currentPos.y][(int)current.x + (int)currentPos.x] = currentColor;
        rows.insert((int)current.y + (int)currentPos.y);
    }
    // Check rows
    int from = (int)currentPos.y + currentMaxOffsets[1], to = (int)currentPos.y + currentMaxOffsets[3];
    for (int i = from; i <= to; i++){
        if (IsRowValid(i)){
            ClearRow(i);
            score += (G_WIDTH - 1) * 10;
            i--;
            to--;
        }
    }
    spawnNew = true;
    //NewCurrentBlock();
}
bool IsRowValid(int &row){
    for (int i = 0; i < G_WIDTH; i++)
        if (tilemap[row][i] == 0) return false;
    return true;
}
void ClearRow(int &row){
    for (int i = 0; i < G_WIDTH; i++)
        tilemap[row][i] = 0;
    for (int i = row + 1; i < G_HEIGHT; i++){
        //Blocks[i + 1][0] = Blocks[i][0];
        memcpy(&tilemap[i - 1][0], &tilemap[i][0], G_WIDTH * sizeof(int));
    }
}
void RenderScreen(){
    // Copy blocks data to buffer
    for (int i = 0; i < G_HEIGHT; i++)
        memcpy(&renderbuffer[i][0], &tilemap[i][0], G_WIDTH * sizeof(int));
    // Copy current to buffer
    for (Vector2 &current : currentBlocks){
        Vector2 pos = current + currentPos;
        // Check if can be rendered
        if (pos.x < 0 || pos.x >= G_WIDTH) continue;
        if (pos.y < 0 || pos.y >= G_HEIGHT) continue;
        // Add to render
        renderbuffer[(int)pos.y][(int)pos.x] = currentColor;
    }
    system("cls");
    printf("Score: %i\n\n", score);
    for (int i = G_HEIGHT - 1; i >= 0; i--){
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