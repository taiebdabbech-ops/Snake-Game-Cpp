#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

enum eDirection { STOP, RIGHT, LEFT, UP, DOWN };
enum eDifficulty { EASY = 200, MEDIUM = 100, HARD = 50 };

struct Segment {
    int x;
    int y;
    Segment* next;
};

int width  = 20;
int height = 20;
int x, y;
int fruitX, fruitY;
int giftX, giftY;
int score;
int bestScore = 0;
int nTail     = 0;
int maxTail   = 0;
eDirection dir;
bool gameover;
bool hardMode  = true;
int difficulty = MEDIUM;

Segment* head = NULL;

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

void ClearList()
{
    Segment* current = head;
    while (current != NULL)
    {
        Segment* temp = current;
        current = current->next;
        delete temp;
    }
    head  = NULL;
    nTail = 0;
}

void PrependSegment(int sx, int sy)
{
    Segment* newSeg = new Segment;
    newSeg->x    = sx;
    newSeg->y    = sy;
    newSeg->next = head;  
    head         = newSeg; 
    nTail++;
}

void RemoveLastSegment()
{
    if (head == NULL) return;
    if (head->next == NULL)
    {
        delete head;
        head  = NULL;
        nTail = 0;
        return;
    }
    Segment* current = head;
    while (current->next->next != NULL)
        current = current->next;
    delete current->next;
    current->next = NULL;
    nTail--;
}

void RemoveSegments(int count)
{
    for (int i = 0; i < count && head != NULL; i++)
        RemoveLastSegment();
}

bool IsOnTail(int px, int py)
{
    Segment* current = head;
    while (current != NULL)
    {
        if (current->x == px && current->y == py) return true;
        current = current->next;
    }
    return false;
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
    score = 0; gameover = false; maxTail = 0; dir = STOP;
    ClearList();
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
                    if (IsOnTail(j, i)) cout << "o";
                    else                cout << " ";
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
    if (dir == STOP) return;

  
    PrependSegment(x, y);

    switch (dir)
    {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
        default:         break;
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

    if (IsOnTail(x, y)) { gameover = true; return; }

    if (x == fruitX && y == fruitY)
    {
        score += 10;
        if (nTail > maxTail) maxTail = nTail;
        fruitX = rand() % width; fruitY = rand() % height;
        
    }
    else
    {
        
        RemoveLastSegment();
    }

    if (x == giftX && y == giftY)
    {
        if (nTail > 10) { RemoveSegments(3); score += 5; }
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
    cout << "   SNAKE GAME - VERSION 3     " << endl;
    cout << "==============================" << endl;
    cout << "  1. FACILE    (enroulement, vitesse lente)"       << endl;
    cout << "  2. MOYEN     (murs = mort, vitesse normale)"     << endl;
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
    ClearList();
    gotoxy(0, height + 5);
    cout << "==============================" << endl;
    if (score >= bestScore && score > 0) cout << "  *** NOUVEAU RECORD ! ***    " << endl;
    cout << "  GAME OVER !" << endl;
    cout << "  Score final  : " << score     << endl;
    cout << "  Meilleur     : " << bestScore  << endl;
    cout << "  Taille max   : " << maxTail   << endl;
    cout << "==============================" << endl;
    return 0;
}