#include <iostream>
#include <list>
#include <Windows.h>
#include <thread>
using namespace std;

#define LEFT 0
#define DOWN 1
#define RIGHT 2
#define UP 3


int nScreenWidth = 120;
int nScreenHeight = 30;


struct sSnakeCordinates{
    int x;
    int y;
};


 
int main(){
    
    while(true) {
        list<sSnakeCordinates> snake = {{60, 15}, {61, 15},{62, 15}, {63, 15}, {64, 15}, {65, 15}, {66, 15},{67, 15}, {68, 15}, {69, 15}};
        // cordinates for the food
        int nFoodXCord = 30; 
        int nFoodYCord = 15;

        // current score
        int nScore = 0;

        int nSnakeDirection = LEFT;

        // boolean to check if the snake is dead
        bool bDead = false;

        // status of arrowkeys
        bool bKeyLeft = false, bKeyRight = false, bKeyLeftOld = false, bKeyRigthOld = false; 
        
        wchar_t *screen = new wchar_t[nScreenHeight*nScreenWidth];
        for (int i = 0; i < nScreenHeight*nScreenWidth; i++){
            screen[i] = L' ';

        }
        HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
        DWORD dwBytesWritten = 0;

        COORD bufferSize = { static_cast<SHORT>(nScreenWidth), static_cast<SHORT>(nScreenHeight) };
        SetConsoleScreenBufferSize(hConsole, bufferSize);

        while(!bDead) {

            // Timing & Input

            // this_thread::sleep_for(200ms);

            // Get input
            auto t1 = chrono::system_clock::now();
            while ((chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 0) ? 120ms : 200ms)){
                bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
                bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

                if (bKeyRight && !bKeyRigthOld) {
                    nSnakeDirection++;
                    if (nSnakeDirection == 4) nSnakeDirection = 0;
                }

                if (bKeyLeft && !bKeyLeftOld) {
                    nSnakeDirection--;
                    if (nSnakeDirection == -1) nSnakeDirection = 3;
                }
                bKeyLeftOld = bKeyLeft;
                bKeyRigthOld = bKeyRight;
            }

             


            // Game Logic
            switch(nSnakeDirection) {
                case LEFT:
                    snake.push_front({snake.front().x - 1, snake.front().y});
                    break;
                case DOWN:
                    snake.push_front({snake.front().x, snake.front().y + 1});
                    break;
                case RIGHT:
                    snake.push_front({snake.front().x + 1, snake.front().y});
                    break;
                case UP:
                    snake.push_front({snake.front().x, snake.front().y - 1});
                    break;

            }

            snake.pop_back();

            // collision detection

            // walls
            if (snake.front().x < 0 || snake.front().y < 3 || snake.front().x >= nScreenWidth  || snake.front().y >= nScreenHeight ){
                bDead = true;
            }

            // itself
            for (list<sSnakeCordinates>::iterator it=snake.begin(); it != snake.end(); ++it){
                if (it != snake.begin() && it->x == snake.front().x && it->y == snake.front().y )
                    bDead = true;
            }

            // food
            if (snake.front().x == nFoodXCord && snake.front().y == nFoodYCord ) {
                nScore++;
                while ( screen[nFoodYCord * nScreenWidth + nFoodXCord] != L' '){
                    nFoodXCord = rand() % nScreenWidth;
                    nFoodYCord = (rand() % (nScreenHeight - 3)) + 3;
                }

                for (int i = 0; i < 5; i++)
                    snake.push_back({ snake.back().x, snake.back().y});

            }

            // Display to player
            // Clear screen
            for (int i = 0; i < nScreenHeight*nScreenWidth; i++) {
                screen[i] = L' ';

            }
            // Draw Stats & Boarder
            for (int i = 0; i < nScreenWidth; i++) {
                screen[i] = L'=';
                screen[2 * nScreenWidth + 5 + i] = L'=';
            }

            swprintf(&screen[nScreenWidth + 5], L"S N A K E !!!!!             Score:%d ", nScore);

            // draw body of snake
            for (auto s : snake) {
                screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'O';

            }

            // drawing head of snake
            screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'+' : L'@';

            screen[nFoodYCord * nScreenWidth + nFoodXCord] = L'%';

            if(bDead)
                swprintf(&screen[nScreenWidth + 5], L"P R E S S  S P A C E  T O  P L A Y  A G A I N ! ! ! !");
            // Display Fram
            WriteConsoleOutputCharacterW(hConsole, screen, nScreenHeight*nScreenWidth, {0,0}, &dwBytesWritten); 

            
        }

        while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
            
    }
    return 0;
}