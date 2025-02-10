#include "TextWriter.h"
#include "SoundPlayer.h"
#include "InputReciever.h"
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

    // these booleans are used like signals
    bool inputDone = true;
    bool textLineDone = false;

    // https://pixabay.com/sound-effects/typewriter-typing-68696/
    // SoundPlayer soundPlayer("type.wav");

    InputReciever inputReciever;
    TextWriter textWriter("start.txt", nullptr, &inputReciever, sWidth, sHeight, &textLineDone, &inputDone);

    // ReadLogic readLogic(&textWriter, &textLineDone, &inputReciever, &inputDone);

    bool gameRunning = true;
    int ch;
    double msNow = getCurrentMs();
    double delta = 0;
    
    double drawTime = 0;
    const double drawTimeWait = 0.01;

    while (gameRunning) {

        int ch = getch();
        if (ch == '7') {
            gameRunning = false;
        }

        delta = (getCurrentMs() - msNow) / 1000.0;
        msNow = getCurrentMs();
        
        drawTime += delta;

        textWriter.update(delta);
        if (ch != ERR && inputDone) {
            textWriter.unPause();
        }
        if (!inputDone) {
            inputReciever.update(ch, delta);
        }

        if (drawTime > drawTimeWait) {
            clear();

            drawTime = 0;
            textWriter.draw(true);

            if (!inputDone) {
                inputReciever.draw();
            }

            refresh();
        }

    }

    endwin();
}