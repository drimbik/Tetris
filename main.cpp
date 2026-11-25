#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROWS = 20;
const int COLS = 10;
const string RECORD_FILE = "record.txt";

struct Figure {
    string name;
    int color; // Êîä öâåòà äëÿ êîíñîëè Windows
    int shape[4][4];
};

void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int loadHighScore() {
    ifstream file(RECORD_FILE);
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void saveHighScore(int newScore) {
    ofstream file(RECORD_FILE);
    if (file.is_open()) {
        file << newScore;
        file.close();
    }
}

bool canMove(const Figure& fig, int field[ROWS][COLS], int nextX, int nextY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (fig.shape[y][x] == 1) {
                int boardY = nextY + y;
                int boardX = nextX + x;

                if (boardY >= ROWS || boardX < 0 || boardX >= COLS) {
                    return false;
                }
                if (boardY >= 0 && field[boardY][boardX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

void rotateFigure(Figure& fig, int field[ROWS][COLS], int& figX, int figY) {
    if (fig.name == "O") return;

    Figure temp = fig;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            temp.shape[x][3 - y] = fig.shape[y][x];
        }
    }

    int offsets[] = { 0, -1, 1, -2, 2 };
    for (int i = 0; i < 5; i++) {
        int testX = figX + offsets[i];
        if (canMove(temp, field, testX, figY)) {
            figX = testX;
            fig = temp;
            return;
        }
    }
}

void placeFigure(const Figure& fig, int field[ROWS][COLS], int figX, int figY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (fig.shape[y][x] == 1) {
                field[figY + y][figX + x] = fig.color;
            }
        }
    }
}

int clearLines(int field[ROWS][COLS]) {
    int linesCleared = 0;

    for (int i = ROWS - 1; i >= 0; i--) {
        bool isFull = true;
        for (int j = 0; j < COLS; j++) {
            if (field[i][j] == 0) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            linesCleared++;
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < COLS; j++) {
                    field[k][j] = field[k - 1][j];
                }
            }
            for (int j = 0; j < COLS; j++) {
                field[0][j] = 0;
            }
            i++;
        }
    }

    return linesCleared;
}

void lockAndSpawn(Figure& activeFig, Figure& nextFig, Figure figures[7], int field[ROWS][COLS], int& figX, int& figY, int& score, bool& runtime) {
    placeFigure(activeFig, field, figX, figY);

    int cleared = clearLines(field);
    score += cleared * 100;

    activeFig = nextFig;
    nextFig = figures[rand() % 7];
    figX = 3;
    figY = 0;

    if (!canMove(activeFig, field, figX, figY)) {
        runtime = false;
    }
}

void playGame(int& highScore) {
    Figure figures[7] = {
        {"I", 11, {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}}, // Ãîëóáîé
        {"J", 9,  {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}}, // Ñèíèé
        {"L", 6,  {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}}, // Îðàíæåâûé
        {"O", 14, {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}}, // Æ¸ëòûé
        {"S", 10, {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}}, // Çåë¸íûé
        {"T", 13, {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}}, // Ôèîëåòîâûé
        {"Z", 12, {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}}  // Êðàñíûé
    };

    while (true) {
        system("cls");
        int field[ROWS][COLS] = {0};

        Figure activeFig = figures[rand() % 7];
        Figure nextFig = figures[rand() % 7];

        int figX = 3;
        int figY = 0;
        int score = 0;
        int fallCounter = 0;
        bool runtime = true;

        while (runtime) {
            setCursorPosition(0, 0);

            if (_kbhit()) {
                char key = _getch();
                if ((key == 'a' || key == 'A') && canMove(activeFig, field, figX - 1, figY)) figX--;
                if ((key == 'd' || key == 'D') && canMove(activeFig, field, figX + 1, figY)) figX++;
                if ((key == 's' || key == 'S') && canMove(activeFig, field, figX, figY + 1)) figY++;
                if (key == 'w' || key == 'W') rotateFigure(activeFig, field, figX, figY);
                if (key == ' ') {
                    while (canMove(activeFig, field, figX, figY + 1)) {
                        figY++;
                    }
                    lockAndSpawn(activeFig, nextFig, figures, field, figX, figY, score, runtime);
                    fallCounter = 0;
                }
                if (key == 27) return;
            }

            fallCounter++;
            if (fallCounter >= 4) {
                if (canMove(activeFig, field, figX, figY + 1)) {
                    figY++;
                } else {
                    lockAndSpawn(activeFig, nextFig, figures, field, figX, figY, score, runtime);
                }
                fallCounter = 0;
            }

            for (int i = 0; i < ROWS; i++) {
                setColor(7);
                cout << '|';
                for (int j = 0; j < COLS; j++) {
                    int fy = i - figY;
                    int fx = j - figX;

                    int cellColor = field[i][j]; 
                    if (fy >= 0 && fy < 4 && fx >= 0 && fx < 4) {
                        if (activeFig.shape[fy][fx] == 1) {
                            cellColor = activeFig.color; 
                        }
                    }

                    if (cellColor != 0) {
                        setColor(cellColor);
                        cout << "[]";
                    } else {
                        cout << "  ";
                    }
                }
                setColor(7);
                cout << '|';

                if (i == 0) {
                    cout << "   Next:";
                } else if (i == 1 || i == 6) {
                    cout << "  +--------+";
                } else if (i >= 2 && i <= 5) {
                    cout << "  |";
                    for (int nx = 0; nx < 4; nx++) {
                        if (nextFig.shape[i - 2][nx] == 1) {
                            setColor(nextFig.color);
                            cout << "[]";
                            setColor(7);
                        } else {
                            cout << "  ";
                        }
                    }
                    cout << "|";
                } else if (i == 8) {
                    cout << "   Score:  " << score << "   ";
                } else if (i == 9) {
                    cout << "   Record: " << (score > highScore ? score : highScore) << "   ";
                } else if (i == 11) {
                    cout << "   [Space] - Drop";
                } else if (i == 12) {
                    cout << "   [ESC]   - Menu";
                }

                cout << "\n";
            }
            setColor(7);
            cout << "+--------------------+\n";

            Sleep(50);
        }

        bool isNewRecord = false;
        if (score > highScore) {
            highScore = score;
            saveHighScore(highScore);
            isNewRecord = true;
        }

        setCursorPosition(0, ROWS + 2);
        setColor(12);
        cout << "=== GAME OVER! ===\n";
        if (isNewRecord) {
            setColor(14);
            cout << "NEW RECORD: " << highScore << "!\n";
        }
        setColor(7);
        cout << "[R] - Restart   [M] - Main Menu\n";

        while (true) {
            char choice = _getch();
            if (choice == 'r' || choice == 'R') break;
            if (choice == 'm' || choice == 'M' || choice == 27) return;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    hideCursor();

    int highScore = loadHighScore();

    while (true) {
        system("cls");
        setColor(11);
        cout << "====================\n";
        cout << "       TETRIS       \n";
        cout << "====================\n";
        setColor(14);
        cout << "High Score: " << highScore << "\n\n";
        setColor(7);
        cout << "1. Play\n";
        cout << "2. Reset Record\n";
        cout << "3. Exit\n\n";
        cout << "Controls:\n";
        cout << "A / D - Left / Right\n";
        cout << "S     - Soft Drop\n";
        cout << "W     - Rotate\n";
        cout << "Space - Hard Drop\n";

        char menuChoice = _getch();

        if (menuChoice == '1') {
            playGame(highScore);
        } else if (menuChoice == '2') {
            highScore = 0;
            saveHighScore(0);
        } else if (menuChoice == '3') {
            break;
        }
    }

    return 0;
}
