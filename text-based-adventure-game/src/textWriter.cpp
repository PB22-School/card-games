#include "textWriter.h"
#include "ncursesw/ncurses.h"

textWriter::textWriter(string filename, int screenWidth, int screenHeight) : stream(filename) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // newLine();
}

void textWriter::update(double delta) {

    return;

    if (paused) {
        return;
    }

    iterTime += delta;

    if ( adding && iterTime > (1.0 / chps) ) {
        iterTime = 0;
        visible_characters++;

        if (textLine[visible_characters] == '\0') {
            adding = false;
            paused = true;
        }

    }

    else if ( !adding && iterTime > (1.0 / rm_chps) ) {
        iterTime = 0;
        visible_characters--;

        if (visible_characters == 0) {
            adding = true;
            paused = true;
        }
    }
}

void textWriter::changeCHPS(int nChps) {
    chps = nChps;
}

void textWriter::changeRMCHPS(int nRMCHPS) {
    rm_chps = nRMCHPS;
}

void textWriter::newLine() {
    visible_characters = 0;
    stream.getline(textLine, '\n');
    mvaddstr(0,0, textLine);
    cursorX = readerX;
    cursorY = readerY;
}

void textWriter::unPause() {
    if (adding) {
        newLine();
    }

    paused = false;
}