#include "textWriter.h"
#include "ncursesw/ncurses.h"
#include <locale.h>
#include <iostream>
using namespace std;

enum {
    WHITE = 5,
    BLACK
};

double getCurrentSeconds() {
    return chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
}

int main(int argc, char* argv[]) {
    
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    start_color();
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);

    init_pair(BLACK, COLOR_BLACK, COLOR_WHITE);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);

    int sWidth;
    int sHeight;

    getmaxyx(stdscr, sHeight, sWidth); 

    // textWriter typeWriter("src/default.txt", sWidth, sHeight);

    bool gameRunning = true;
    int ch;
    double secsNow = getCurrentSeconds();
    double delta;
    
    mvaddstr(0,0, "hello");

    while (gameRunning) {
        delta = getCurrentSeconds() - secsNow;
        secsNow = getCurrentSeconds();

        // typeWriter.update(delta);

    }

    endwin();
}