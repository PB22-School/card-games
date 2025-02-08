#include "TextWriter.h"
#include "SoundPlayer.h"
#include "ncursesw/ncurses.h"
#include <locale.h>
#include <iostream>
using namespace std;

enum {
    YELLOW = 6,
    GREEN,
    BLACK,
    RED,
    WHITE
};

double getCurrentMs() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

int main(int argc, char* argv[]) {
    
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    start_color();
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);

    int sWidth;
    int sHeight;

    getmaxyx(stdscr, sHeight, sWidth); 

    // https://pixabay.com/sound-effects/typewriter-typing-68696/
    // SoundPlayer soundPlayer("type.wav");

    TextWriter textWriter("default.txt", nullptr, sWidth, sHeight);

    bool gameRunning = true;
    int ch;
    double msNow = getCurrentMs();
    double delta = 0;
    
    // mvaddstr(0,0, "hello");
    double drawTime = 0;
    const double drawTimeWait = 0.01;

    while (gameRunning) {

        if (getch() == 'q') {
            gameRunning = false;
        }

        delta = (getCurrentMs() - msNow) / 1000.0;
        msNow = getCurrentMs();

        drawTime += delta;

        textWriter.update(delta);

        if (drawTime > drawTimeWait) {
            drawTime = 0;
            clear();
            textWriter.draw(true);
            refresh();
        }

    }

    endwin();
}