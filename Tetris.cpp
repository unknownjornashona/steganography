#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WIDTH = 10;
const int HEIGHT = 20;

struct Tetromino {
    std::vector<std::vector<int>> shape;
    int x, y;

    Tetromino(std::vector<std::vector<int>> s) : shape(s), x(WIDTH / 2 - 1), y(0) {}
};

class Tetris {
private:
    std::vector<std::vector<int>> board;
    Tetromino currentTetromino;
    bool gameOver;
    int score;

public:
    Tetris() : board(HEIGHT, std::vector<int>(WIDTH, 0)),
               currentTetromino({{1, 1, 1, 1}}), // I 型方块
               gameOver(false), score(0) {}

    void draw() {
        clear();
        mvprintw(0, 0, "得分: %d", score);
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (board[i][j] != 0) {
                    mvaddch(i + 1, j, '#');
                } else {
                    mvaddch(i + 1, j, '.');
                }
            }
        }
        // 显示当前方块
        for (int i = 0; i < currentTetromino.shape.size(); i++) {
            for (int j = 0; j < currentTetromino.shape[i].size(); j++) {
                if (currentTetromino.shape[i][j] != 0) {
                    mvaddch(currentTetromino.y + 1 + i, currentTetromino.x, '#');
                }
            }
        }
        refresh();
    }

    void drop() {
        if (canMove(currentTetromino.x, currentTetromino.y + 1)) {
            currentTetromino.y++;
        } else {
            placeTetromino();
            clearRows();
            currentTetromino = Tetromino({{1, 1, 1, 1}}); // 重置为 I 型方块
            if (!canMove(currentTetromino.x, currentTetromino.y)) {
                gameOver = true;
            }
        }
    }

    bool canMove(int x, int y) {
        for (int i = 0; i < currentTetromino.shape.size(); i++) {
            for (int j = 0; j < currentTetromino.shape[i].size(); j++) {
                if (currentTetromino.shape[i][j] != 0) {
                    int newX = x + j;
                    int newY = y + i;
                    if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || newY < 0 || board[newY][newX] != 0) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void placeTetromino() {
        for (int i = 0; i < currentTetromino.shape.size(); i++) {
            for (int j = 0; j < currentTetromino.shape[i].size(); j++) {
                if (currentTetromino.shape[i][j] != 0) {
                    board[currentTetromino.y + i][currentTetromino.x + j] = 1;
                }
            }
        }
    }

    void clearRows() {
        for (int i = HEIGHT - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < WIDTH; j++) {
                if (board[i][j] == 0) {
                    full = false;
                    break;
                }
            }
            if (full) {
                board.erase(board.begin() + i);
                board.insert(board.begin(), std::vector<int>(WIDTH, 0));
                score += 100; // 每消除一行加分
            }
        }
    }

    void showMenu() {
        clear();
        mvprintw(0, 0, "开始游戏 (1)");
        mvprintw(1, 0, "退出 (2)");
        refresh();

        int choice;
        while (true) {
            choice = getch();
            if (choice == '1') {
                break; // 开始游戏
            } else if (choice == '2') {
                endwin();
                exit(0); // 退出游戏
            }
        }
    }

    void run() {
        showMenu(); // 显示菜单
        while (!gameOver) {
            draw();
            drop();
            int ch = getch();
            if (ch == 'a' && canMove(currentTetromino.x - 1, currentTetromino.y)) {
                currentTetromino.x--;
            } else if (ch == 'd' && canMove(currentTetromino.x + 1, currentTetromino.y)) {
                currentTetromino.x++;
            } else if (ch == 's') {
                drop();
            }
            napms(500); // 控制下落速度，单位是毫秒
        }
        mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 5, "游戏结束!");
        refresh();
        getch(); // 等待用户按键
    }
};

int main() {
    initscr();            // 初始化 ncurses
    cbreak();             // 关闭行缓冲，字符立即传送到程序
    noecho();             // 不显示输入的字符
    keypad(stdscr, TRUE); // 启用功能键

    Tetris game;
    game.run();

    endwin(); // 结束 ncurses
    return 0;
}
