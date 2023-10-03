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

int tilemap[G_HEIGHT][G_WIDTH] = { {0} };
int** renderbuffer;
unsigned int blockcolors[] = {
    31, // Blue
    47, // Green
    63, // Cyan
    79, // Red
    95, // Purple
};
unsigned short blockcolors_size = 5;

vector<Block> blocks = vector<Block>();

Vector2 currentPos = Vector2::zero;
vector<Vector2> currentBlocks = vector<Vector2>();
unsigned int currentColor = 0;
int currentMaxOffsets[4] = {0}; // Left, Down, Right, Up

void LoadBlockFromFile(string path);
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
    srand(time(NULL));
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
        // Validate block
        int *blockbuffer_offsets_ptr = blockbuffer.GetMaxOffsets(); // TODO Test with other values of G_WIDTH
        if (blockbuffer_offsets_ptr[3] < 0 ||
            blockbuffer_offsets_ptr[0] < G_WIDTH / 2 * -1 ||
            blockbuffer_offsets_ptr[0] > G_WIDTH / 2)
            throw "Invalid block - " + to_string(blocks.size()); // FIXME info not works
        blocks.push_back(blockbuffer);
    }
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
    Block *bptr = &blocks[rand() % blocks.size()];
    // Block *bptr = &blocks[2];
    for (Vector2 &child : bptr->GetChilds()){
        currentBlocks.push_back(child);
    }
    currentColor = blockcolors[rand() % blockcolors_size];
    memcpy(&currentMaxOffsets[0], bptr->GetMaxOffsets(), 4 * sizeof(int));
}
void MoveDown(){
    currentPos.y += 1;
    for(Vector2 &current : currentBlocks){
        Vector2 blockPos = current + currentPos;
        if (blockPos.y >= G_HEIGHT || tilemap[(int)blockPos.y + 1][(int)blockPos.x] != 0){
            MakeCurrentStationary();
            return;
        }
    }
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
void MakeCurrentStationary(){
    set<int> rows = set<int>();
    for(Vector2 &current : currentBlocks){
        tilemap[(int)current.y + (int)currentPos.y][(int)current.x + (int)currentPos.x] = currentColor;
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
        if (tilemap[row][i] == 0) return false;
    return true;
}
void ClearRow(int &row){
    for (int i = 0; i < G_WIDTH; i++)
        tilemap[row][i] = 0;
    for (int i = row - 1; i >= 0; i--){
        //Blocks[i + 1][0] = Blocks[i][0];
        memcpy(&tilemap[i + 1][0], &tilemap[i][0], G_WIDTH * sizeof(int));
    }
}
void RenderScreen(){
    // Copy blocks data to buffer
    for (int i = 0; i < G_HEIGHT; i++)
        memcpy(&renderbuffer[i][0], &tilemap[i][0], G_WIDTH * sizeof(int));
    // Copy current to buffer
    for (Vector2 &current : currentBlocks){
        cout<<(int)current.y + (int)currentPos.y<<" "<<(int)current.x + (int)currentPos.x<<endl;
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