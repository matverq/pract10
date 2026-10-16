#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>


using namespace std;

const int width = 40;
const int height = 20;

const int up = 0;
const int down = 1;
const int LEFT = 2;
const int RIGHT = 3;

struct Node {
    int x;
    int y;
    Node* next;
};

struct Record {
    char name[50];
    int score;
};

Node* head = nullptr;
int len;
int score;
int dir;
int foodx, foody;


Node* makeNode(int x, int y) {
    Node* n = new Node;
    n->x = x;
    n->y = y;
    n->next = nullptr;
    return n;
}

void freeSnake() {
    while (head != nullptr) {
        Node* t = head;
        head = head->next;
        delete t;
    }
}

bool onSnake(int x, int y) {
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->x == x && cur->y == y) return true;
        cur = cur->next;
    }
    return false;
}

void newFood() {
    do {
        foodx = rand() % (width - 2) + 1;
        foody = rand() % (height - 2) + 1;
    } while (onSnake(foodx, foody));
}

void startGame() {
    freeSnake();

    int cx = width / 2;
    int cy = height / 2;

    head = makeNode(cx, cy);
    head->next = makeNode(cx - 1, cy);
    head->next->next = makeNode(cx - 2, cy);

    len = 3;
    score = 0;
    dir = RIGHT;
    newFood();
}

void draw() {
    system("cls");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                cout << "#";
            else if (head != nullptr && head->x == j && head->y == i)
                cout << "0";
            else if (onSnake(j, i))
                cout << "o";
            else if (j == foodx && i == foody)
                cout << "*";
            else
                cout << " ";
        }
        cout << "\n";
    }
    cout << "Счет: " << score << "\n";
    cout << "WASD - движение, P - сохранить, Q - выход\n";
}

bool step() {
    int nx = head->x;
    int ny = head->y;

    if (dir == up) ny--;
    if (dir == down) ny++;
    if (dir == LEFT) nx--;
    if (dir == RIGHT) nx++;

    if (nx <= 0 || nx >= width - 1) return false;
    if (ny <= 0 || ny >= height - 1) return false;

    bool eat = false;
    if (nx == foodx && ny == foody) eat = true;

    Node* cur = head;
    while (cur != nullptr) {
        if (!eat && cur->next == nullptr) break;
        if (cur->x == nx && cur->y == ny) return false;
        cur = cur->next;
    }

    Node* nh = makeNode(nx, ny);
    nh->next = head;
    head = nh;

    if (eat) {
        score += 10;
        len++;
        newFood();
    }
    else {
        cur = head;
        while (cur->next->next != nullptr) cur = cur->next;
        delete cur->next;
        cur->next = nullptr;
    }

    return true;
}

void saveState(const char* filename) {
    ofstream f(filename, ios::binary);
    if (!f) return;

    f.write((char*)&score, sizeof(score));
    f.write((char*)&dir, sizeof(dir));
    f.write((char*)&foodx, sizeof(foodx));
    f.write((char*)&foody, sizeof(foody));
    f.write((char*)&len, sizeof(len));

    Node* cur = head;
    while (cur != nullptr) {
        f.write((char*)&cur->x, sizeof(int));
        f.write((char*)&cur->y, sizeof(int));
        cur = cur->next;
    }
    f.close();
}

bool loadGame(const char* filename) {
    ifstream f(filename, ios::binary);
    if (!f) return false;

    freeSnake();

    f.read((char*)&score, sizeof(score));
    f.read((char*)&dir, sizeof(dir));
    f.read((char*)&foodx, sizeof(foodx));
    f.read((char*)&foody, sizeof(foody));
    f.read((char*)&len, sizeof(len));

    Node* tail = nullptr;
    for (int i = 0; i < len; i++) {
        Node* n = new Node;
        f.read((char*)&n->x, sizeof(int));
        f.read((char*)&n->y, sizeof(int));
        n->next = nullptr;

        if (head == nullptr) {
            head = n;
            tail = n;
        }
        else {
            tail->next = n;
            tail = n;
        }
    }
    f.close();
    return true;
}

void addRecord(const char* name, int sc) {
    ofstream f("records.dat", ios::binary | ios::app);
    if (!f) return;

    Record r;
    int i = 0;
    while (name[i] != 0 && i < 49) {
        r.name[i] = name[i];
        i++;
    }
    r.name[i] = 0;
    r.score = sc;

    f.write((char*)&r, sizeof(Record));
    f.close();
}

void showRecords() {
    system("cls");
    cout << "ТАБЛИЦА РЕКОРДОВ\n\n";

    ifstream f("records.dat", ios::binary);
    if (!f) {
        cout << "Рекордов пока нет\n";
        _getch();
        return;
    }

    Record r;
    while (f.read((char*)&r, sizeof(Record))) {
        cout << r.name << ": " << r.score << "\n";
    }
    f.close();
    _getch();
}

void playGame() {
    bool run = true;

    while (run) {
        if (_kbhit()) {
            char c = _getch();

            if (c == 'w' && dir != down) dir = up;
            if (c == 's' && dir != up) dir = down;
            if (c == 'a' && dir != RIGHT) dir = LEFT;
            if (c == 'd' && dir != LEFT) dir = RIGHT;

            if (c == 'p') saveState("save.dat");
            if (c == 'q') {
                run = false;
                break;
            }
        }
        draw();

        if (step() == false) {
            cout << "\nИгра окончена! Счет: " << score << "\n";
            cout << "Введите имя: ";

            char name[50];
            cin >> name;
            addRecord(name, score);

            cout << "Нажмите клавишу...";
            _getch();
            run = false;
        }
        Sleep(150);
    }
    freeSnake();
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    while (true) {
        system("cls");
        cout << "1. Новая игра\n";
        cout << "2. Загрузить игру\n";
        cout << "3. Таблица рекордов\n";
        cout << "4. Выход\n\n";
        cout << "Выберите пункт: ";

        char choice = _getch();

        if (choice == '1') {
            startGame();
            playGame();
        }
        else if (choice == '2') {
            if (loadGame("save.dat")) {
                playGame();
            }
        }
        else if (choice == '3') {
            showRecords();
        }
        else if (choice == '4') {
            freeSnake();
            return 0;
        }
    }
}
