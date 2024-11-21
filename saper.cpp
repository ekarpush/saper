#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
const int WIDTH = 8;  // Ширина поля
const int HEIGHT = 9; // Высота поля
const int MINES = 10; // Количество мин

class Minesweeper {
public:
    Minesweeper() {
        initField();
        placeMines();
        calculateHints();
        bool flagged[HEIGHT][WIDTH];
     
    }

    void play() {
        std::string command;
        while (!gameOver && openedCells < (WIDTH * HEIGHT - MINES)) {
            printField();
            std::cout << "Введите команду (open- o/flag - f) и координаты (например, o a1): ";
            std::getline(std::cin, command);
            processCommand(command);
        }

        if (gameOver) {
            std::cout << "Вы попали на мину! Игра окончена." << std::endl;
        } else {
            std::cout << "Поздравляем! Вы разминировали все мины!" << std::endl;
        }
    }

private:
    char field[HEIGHT][WIDTH];
    bool revealed[HEIGHT][WIDTH];
    bool flagged[HEIGHT][WIDTH];
    bool unflagged[HEIGHT][WIDTH];
   
    int openedCells = 0;
    bool gameOver = false;

    void initField() {
        std::memset(field, '.', sizeof(field));
        std::memset(revealed, false, sizeof(revealed));
        std::memset(flagged, false, sizeof(flagged));
        std::memset(unflagged, false, sizeof(unflagged));
    }

    void placeMines() {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < MINES; ) {
            int x = rand() % HEIGHT;
            int y = rand() % WIDTH;
            if (field[x][y] != '*') {
                field[x][y] = '*';
                ++i;
            }
        }
    }

    void calculateHints() {
        for (int x = 0; x < HEIGHT; ++x) {
            for (int y = 0; y < WIDTH; ++y) {
                if (field[x][y] == '*') continue;

                int mineCount = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH && field[nx][ny] == '*') {
                            ++mineCount;
                        }
                    }
                }
                if (mineCount > 0) {
                    field[x][y] = '0' + mineCount;
                }
            }
        }
    }

    void printField() const {
        std::cout << "   a  b  c  d  e  f  g  h" << std::endl;
        for (int i = 0; i < HEIGHT; ++i) {
            std::cout << i + 1 << " ";
            for (int j = 0; j < WIDTH; ++j) {
                if (revealed[i][j]) {
                    std::cout << field[i][j] << ' '<< ' ';
                } else if (unflagged[i][j]) {
                    std::cout << ' '<< '.' << ' ';
                } else if (flagged[i][j] and  !unflagged[i][j]) {
                    std::cout << ' '<< 'F' << ' ';
                //} else if (field[i][j] == '0'){
                   // std::cout<< ' ' << '0' << ' ';
                } else {
                    std::cout<< ' ' << '.' << ' ';
                }
            }
            std::cout << std::endl;
        }
    }

    void processCommand(const std::string &command) {
        char action;
        char col;
        int row;

        if (sscanf(command.c_str(), "%c %c%d", &action, &col, &row) != 3) {
            std::cout << "Неверная команда. Попробуйте снова." << std::endl;
            return;
        }

        int x = row - 1;
        int y = col - 'a';

        if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
            std::cout << "Координаты вне поля. Попробуйте снова." << std::endl;
            return;
        }

        if (action == 'o') {
            openCell(x, y);
        } else if (action == 'f') {
            flagCell(x, y);
        } else if (action == 'u') {
            unflagCell(x, y);
       
        } else {
            std::cout << "Неверная команда. Используйте open или flag." << std::endl;
        }
    }

    void openCell(int x, int y) {
        if (revealed[x][y] || flagged[x][y]) return;

        revealed[x][y] = true;
        openedCells++;

        if (field[x][y] == '*') {
            gameOver = true;
            return;
        }

        if (field[x][y] == '.') { // Если рядом нет мин
            for (int dx = -1; dx <= 1; ++dx) {
                  for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH) {
                        openCell(nx, ny);
                    }
                }
            }
        }
    }

    void flagCell(int x, int y) {
        if (!revealed[x][y]) {
     
          flagged[x][y] = !flagged[x][y];
      }   }
    void unflagCell(int x, int y) {
        if (!revealed[x][y]) {
     
          unflagged[x][y] = !unflagged[x][y];
          flagged[x][y] = 0;
          
      }   }
};

int main() {
    Minesweeper game;
    game.play();
    return 0;
}