#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

enum eDirection { STOP, RIGHT, LEFT, UP, DOWN };
enum eDifficulty { EASY = 200, MEDIUM = 100, HARD = 50 };

int width  = 20;
int height = 20;
int x, y;
int fruitX, fruitY;
int giftX, giftY;
int score;
int bestScore = 0;
int tailX[100];
int tailY[100];
int nTail;
int maxTail = 0;
eDirection dir;
bool gameover;
bool hardMode  = true;
int difficulty = MEDIUM;

int obstacleX[5] = {5, 15, 10,  3, 18};
int obstacleY[5] = {5, 10, 15, 10,  5};
int numObstacles = 5;

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void LoadBestScore()
{
    ifstream file("bestScore.txt");
    if (file.is_open()) { file >> bestScore; file.close(); }
}

void SaveBestScore()
{
    ofstream file("bestScore.txt");
    file << bestScore;
    file.close();
}

void Setup()
{
    srand((unsigned)time(0));
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    do {
        giftX = rand() % width;
        giftY = rand() % height;
    } while (giftX == fruitX && giftY == fruitY);
    score = 0; gameover = false; nTail = 0; maxTail = 0; dir = STOP;
    LoadBestScore();
}

void Draw()
{
    gotoxy(0, 0);
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;
    for (int i = 0; i < height; i++)
    {
        cout << "#";
        for (int j = 0; j < width; j++)
        {
            if (j == x && i == y) cout << "O";
            else if (j == fruitX && i == fruitY) cout << "F";
            else if (j == giftX  && i == giftY)  cout << "*";
            else
            {
                bool printed = false;
                for (int obs = 0; obs < numObstacles; obs++)
                {
                    if (j == obstacleX[obs] && i == obstacleY[obs])
                    { cout << "X"; printed = true; break; }
                }
                if (!printed)
                {
                    bool tailPrinted = false;
                    for (int k = 0; k < nTail; k++)
                    {
                        if (tailX[k] == j && tailY[k] == i)
                        { cout << "o"; tailPrinted = true; break; }
                    }
                    if (!tailPrinted) cout << " ";
                }
            }
        }
        cout << "#" << endl;
    }
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;
    cout << "Score : " << score << "  |  Meilleur : " << bestScore << "  |  Queue : " << nTail << "   " << endl;
    cout << "z=haut  s=bas  q=gauche  d=droite  o=quitter" << endl;
    cout << "O=serpent  F=fruit  *=cadeau  X=obstacle      " << endl;
}

void Input()
{
    if (_kbhit())
    {
        char key = _getch();
        switch (key)
        {
            case 'q': case 'Q': dir = LEFT;      break;
            case 'd': case 'D': dir = RIGHT;     break;
            case 'z': case 'Z': dir = UP;        break;
            case 's': case 'S': dir = DOWN;      break;
            case 'o': case 'O': gameover = true; break;
        }
    }
}

void Logic()
{
    int prevX = tailX[0], prevY = tailY[0];
    tailX[0] = x; tailY[0] = y;
    int prev2X, prev2Y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i]; prev2Y = tailY[i];
        tailX[i] = prevX;  tailY[i] = prevY;
        prevX = prev2X;    prevY = prev2Y;
    }
    switch (dir)
    {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
        case STOP:       break;
    }
    if (hardMode)
    {
        if (x < 0 || x >= width || y < 0 || y >= height) { gameover = true; return; }
    }
    else
    {
        if (x < 0)       x = width  - 1;
        if (x >= width)  x = 0;
        if (y < 0)       y = height - 1;
        if (y >= height) y = 0;
    }
    for (int i = 0; i < numObstacles; i++)
        if (x == obstacleX[i] && y == obstacleY[i]) { gameover = true; return; }
    for (int i = 0; i < nTail; i++)
        if (x == tailX[i] && y == tailY[i]) { gameover = true; return; }
    if (x == fruitX && y == fruitY)
    {
        score += 10; nTail++;
        if (nTail > maxTail) maxTail = nTail;
        fruitX = rand() % width; fruitY = rand() % height;
    }
    if (x == giftX && y == giftY)
    {
        if (nTail > 10) { nTail -= 3; score += 5; }
        do {
            giftX = rand() % width;
            giftY = rand() % height;
        } while (giftX == fruitX && giftY == fruitY);
    }
}

int main()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    cout << "==============================" << endl;
    cout << "        SNAKE GAME            " << endl;
    cout << "==============================" << endl;
    cout << "  1. FACILE    (enroulement, vitesse lente)"      << endl;
    cout << "  2. MOYEN     (murs = mort, vitesse normale)"    << endl;
    cout << "  3. DIFFICILE (obstacles + murs, vitesse rapide)" << endl;
    cout << "Votre choix (1-3) : ";
    int choice; cin >> choice;
    switch (choice)
    {
        case 1: difficulty = EASY;   hardMode = false; break;
        case 2: difficulty = MEDIUM; hardMode = true;  break;
        case 3: difficulty = HARD;   hardMode = true;  break;
        default:difficulty = MEDIUM; hardMode = true;
    }
    system("cls");
    Setup();
    while (!gameover) { Draw(); Input(); Logic(); Sleep(difficulty); }
    if (score > bestScore) { bestScore = score; SaveBestScore(); }
    gotoxy(0, height + 5);
    cout << "==============================" << endl;
    if (score >= bestScore && score > 0) cout << "  *** NOUVEAU RECORD ! ***    " << endl;
    cout << "  GAME OVER !" << endl;
    cout << "  Score final  : " << score    << endl;
    cout << "  Meilleur     : " << bestScore << endl;
    cout << "  Taille max   : " << maxTail  << endl;
    cout << "==============================" << endl;
    return 0;
}
