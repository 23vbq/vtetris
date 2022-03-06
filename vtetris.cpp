#include <iostream>

using namespace std;

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
void ssleep(unsigned miliseconds)
{
    Sleep(miliseconds);
}
void cclear()
{
    system("cls");
}
int getkey()
{
    return getch();
}
#else
#include <unistd.h>
#include <stdio.h>
void ssleep(unsigned miliseconds)
{
    usleep(miliseconds * 1000);
}
void cclear()
{
    system("clear");
}
int getkey()
{
    system("/bin/stty raw");
    int c = getchar();
    system("/bin/stty cooked");
    return c;
}
#endif
/*#include <chrono>
#include <thread>*/

#define WIDTH 21
#define HEIGHT 21
#define borderSymbol "#"
#define SPEED 0.5

bool terminateApp = false;

bool renderChange = true;
short p[HEIGHT][WIDTH] = {{0}};
short offset = 0;

void moveLeft()
{
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        if (p[i][0])
        {
            return;
        }
    }
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        for (int j = 1; j < WIDTH; j++)
        {
            p[i][j - 1] = p[i][j];
        }
        p[i][WIDTH - 1] = 0;
    }
    renderChange = true;
    return;
}
void moveRight()
{
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        if (p[i][HEIGHT - 1])
        {
            return;
        }
    }
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        for (int j = WIDTH - 2; j >= 0; j--)
        {
            p[i][j + 1] = p[i][j];
        }
        p[i][0] = 0;
    }
    renderChange = true;
    return;
}

void goDown()
{
    while (!terminateApp)
    {
        for (int i = HEIGHT - 1; i >= 0; i--)
        {
            // p[i] = p[i - 1];
            for (int j = 0; j < WIDTH - 2; j++)
            {
                p[i][j] = p[i - 1][j];
            }
        }
        // p[0] = {0};
        renderChange = true;
        ssleep(1000 * SPEED);
    }
}

void render()
{
    cclear();
    for (int i = 0; i < HEIGHT + 2; i++)
    {
        for (int j = 0; j < WIDTH + 2; j++)
        {
            if (i == 0 || i == (HEIGHT + 2) - 1 || j == 0 || j == (WIDTH + 2) - 1)
            {
                cout << borderSymbol;
            }
            else if (p[i - 1][j - 1] == 1)
            {
                cout << "0";
            }
            else
            {
                cout << " ";
            }
        }
        cout << endl;
    }
    renderChange = false;
}

int main()
{
    p[0][10] = 1;
    render();
    while (true)
    {
        goDown();
        int handler = getkey();
        //  cout << handler << endl;
        if (handler == 97) // A
        {
            // cout << "Lewo" << endl;
            moveLeft();
        }
        else if (handler == 100) // D
        {
            // cout << "Prawo" << endl;
            moveRight();
        }
        else if (handler == 119)
        {
            // w
            cout << "Gora" << endl;
        }
        else if (handler == 115)
        {
            // s
            cout << "Dol" << endl;
        }
        else if (handler == 113) // Q - quit
        {
            return 0;
        }
        if (renderChange)
        {
            render();
        }
    }
}