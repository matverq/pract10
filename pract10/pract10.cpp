#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>

using namespace std;

int width = 40;
int height = 20;

int up = 0;
int down = 1;
int LEFT = 2;
int RIGHT = 3;

struct Segment {
    int x;
    int y;
    Segment* next;
    Segment(int _x, int _y) {
        x = _x;
        y = _y;
        next = nullptr;
    }
};

struct Game {
    Segment* head;
    int length;
    int score;
    int dir;
    int foodx;
    int foody;
};

Segment* createSegment(int x, int y) {
    return new Segment(x, y);
}

void deleteSnake(Segment* head) {
    while (head !=nullptr) {
        Segment* temp = head;
        head = head->next;
        delete temp;
    }
}

bool isSnake(Segment* head, int x, int y) {
    Segment* cur = head;
    while (cur != nullptr) {
        if (cur->x == x && cur->y == y) return true;
        cur = cur->next;
    }
    return false;
}

void spawnFood(Game& g) {
    do {
        g.foodx = rand() % (width - 2) + 1;
        g.foody = rand() % (height - 2) + 1;
    } while (isSnake(g.head, g.foodx, g.foody));
}

void startGame(Game& g) {
    int cx = width / 2;
    int cy = height / 2;

    g.head = createSegment(cx, cy);
    g.head->next= createSegment(cx-1, cy);
    g.head->next->next= createSegment(cx-2, cy);

    g.length = 3;
    g.score = 0;
    g.dir = RIGHT;
    spawnFood(g);
}

void drawGame(Game& g) {
    system("cls");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) { cout << "#"; }
            else if (g.head != nullptr && g.head->x == x && g.head->y == y) { cout << "0"; }
            else if (isSnake(g.head, x, y)) { cout << "o"; }
            else if (x == g.foodx && y == g.foody) { cout << "*"; }
            else { cout << " "; }
        }
        cout << "\n";
    }
    cout << "Счет: " << g.score << "\n";
    
}

bool uSnake(Game& g) {
    int newx = g.head->x;
    int newy = g.head->y;

    if (g.dir == up) { newy--; }
    if (g.dir == down) { newy++; }
    if (g.dir == LEFT) { newx--; }
    if (g.dir == RIGHT) { newx++; }

    if (newx <= 0 || newx >= width - 1) { return false; }
    if(newy<= 0 || newy >= width - 1) { return false; }

    bool eating = false;
    if (newx == g.foodx && newy == g.foody) { eating = true; }

    Segment* cur = g.head;
    while (cur != nullptr) {
        if (!eating && cur->next == nullptr) { break; }
        if (cur->x == newx && cur->y == newy) { return false; }
        cur = cur->next;
    }

    Segment* newHead = createSegment(newx, newy);
    newHead->next = g.head;
    g.head = newHead;

    if (eating) {
        g.score = g.score + 10;
        g.length++;
        spawnFood(g);
    }
    else {
        cur = g.head;
        while (cur->next->next != nullptr) {
            cur = cur->next;
        }
        delete cur->next;
        cur->next = nullptr;
    }
    return true;
}

void playGame(Game& g) {
    bool run = true;
    
    while (run) {
        if (_kbhit()) {
            char c = tolower(_getch());
            if (c == 'w' && g.dir != down) { g.dir = up; }
            if (c == 'w' && g.dir != down) { g.dir = up; }
            if (c == 'w' && g.dir != down) { g.dir = up; }
            if (c == 'w' && g.dir != down) { g.dir = up; }
        }
    }
}

int main()
{
    std::cout << "Hello World!\n";
}