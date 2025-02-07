#include "board.h"
#include "ai.h"
#include "ncursesw/ncurses.h"
#include <string>
#include <locale.h>
#include <iostream>
using namespace std;

enum {
    YELLOW = 5,
    RED,
    GREEN,
    WHITE
};

int selectedSquare = 0;
const string pieces[3] = {"     ", "  X  ", "  O  "};

void draw_button(string text, int x, int y, int padding) {

    int len = text.length();
    int half = padding / 2;
    int twice = (padding * 2) + 1;

    int width = twice + len;
    int height = twice;

    mvhline(y, x + 1, ACS_HLINE, width - 1);
    mvhline(y + twice + 1, x + 1, ACS_HLINE, width - 1);

    mvvline(y + 1, x, ACS_VLINE, height);
    mvvline(y + 1, x + width, ACS_VLINE, height);

    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y + height + 1, x, ACS_LLCORNER);
    mvaddch(y, x + width, ACS_URCORNER);
    mvaddch(y + height + 1, x + width, ACS_LRCORNER);

    mvaddstr(y + padding + 1, x + padding + 1, text.c_str());

}

int button_length(string text, int padding) {
    return text.length() + (padding * 2) + 2;
}

void draw(Board* board) {

    int len = button_length(pieces[0], 1);
    int yLen = 5;
    int startX = 60;
    int startY = 20;

    for (int i = 0; i < 9; i++) {
        int x = i % 3;
        int y = i / 3;
        if (selectedSquare == i) {
            color_set(YELLOW, nullptr);
        }
        else {
            color_set(WHITE, nullptr);
        }
        draw_button(pieces[board->boardRep[i]], startX + (x * len), startY + (y * yLen), 1);
    }
}

void update(int ch, Board* board) {

    bool changes = true;

    switch (ch) {
        case 'w':
            selectedSquare -= 3;
            break;
        case 'a':
            selectedSquare -= 1;
            break;
        case 'd':
            selectedSquare += 1;
            break;
        case 's':
            selectedSquare += 3;
            break;
        default:
            changes = false;
            break;
    }

    if (selectedSquare >= 9) {
        selectedSquare -= 9;
    }
    else if (selectedSquare < 0) {
        selectedSquare += 9;
    }

    if (changes) {
        draw(board);
    }
}

int main() {

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    start_color();
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);

    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);

    bool gameRunning = true;
    bool gameOver = false;
    Board board;
    draw(&board);

    int ch;

    while (gameRunning) {

        ch = getch();
        if (ch == 'q') {
            gameRunning = false;
        } 

        if (gameOver && ch != ERR) {
            gameOver = false;
            board = Board();
            draw(&board);
        }
        else {
            update(ch, &board);
        }
    }

    endwin();
}