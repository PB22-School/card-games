#include "InputReciever.h"
#include "ncursesw/ncurses.h"

InputReciever::InputReciever() {
    takingInput = false;
    input = "";
}

void InputReciever::startRecieving(bool* callback) {
    blinkOn = false;
    blinkIter = 0;
    takingInput = true;
    input = "";
    *callback = false;
    this->callback = callback;
}

void InputReciever::update(int ch, double delta) {

    blinkIter += delta;

    if (blinkIter >= blinkTime) {
        blinkIter = 0;
        blinkOn = !blinkOn;
    }

    if (!takingInput || ch == ERR) {
        return;
    }

    blinkIter = 0;

    if (ch == '\n') {
        takingInput = false;
        *callback = true;
        return;
    }
    else if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) {
        if (input.length() == 0) {
            return;
        }
        input.erase(input.length() - 1);
        return;
    }

    input += ch;
}

void InputReciever::draw() const {
    // mvaddstr(recieverY, recieverX - 1, "| ");
    mvaddstr(recieverY, recieverX, input.c_str());

    if (blinkOn) {
        mvaddstr(recieverY, recieverX + input.length(), "█");
    }
}

string InputReciever::getInput() const {
    return input;
}
