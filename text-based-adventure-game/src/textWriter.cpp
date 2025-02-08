#include "TextWriter.h"
#include "ncursesw/ncurses.h"
#include <sstream>

enum {
    YELLOW = 6,
    GREEN,
    BLACK,
    RED,
    WHITE
};

TextWriter::TextWriter(string filename, SoundPlayer* soundPlayer, int screenWidth, int screenHeight) : stream("static/text/" + filename) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->soundPlayer = soundPlayer;

    if (!stream.is_open()) {
        mvaddstr(0,0, "Something went wrong, and the program couldn't open the file.");
        refresh();
        return;
    }

    newLine();
}

void TextWriter::update(double delta) {

    if (paused) {
        return;
    }

    iterTime += delta;

    if ( adding && iterTime > (1.0 / chps) ) {
        iterTime = 0;
        visible_characters++;

        if (visible_characters >= textLine.length()) {
            adding = false;
            paused = true;
            return;
        }

        if (textLine[visible_characters] == '[') {
            bool takingAway = false;
            int endPos = textLine.find_first_of(']', visible_characters);

            if (textLine[visible_characters + 1] == '/') {
                // means we're taking away whatever effect
                takingAway = true;
            }

            string substr = textLine.substr(visible_characters + 1 + takingAway, endPos + 1);

            if (substr == "bold") {
                if (takingAway) {
                    attroff(A_BOLD);
                }
                else {
                    attron(A_BOLD);
                }
            }
            else if (substr == "italic") {
                if (takingAway) {
                    attroff(A_ITALIC);
                }
                else {
                    attron(A_ITALIC);
                }
            }

            else if (substr == "yellow") {
                if (takingAway) {
                    color_set(WHITE, nullptr);
                }
                else {
                    color_set(YELLOW, nullptr);
                }
            }
            
            else if (substr == "green") {
                if (takingAway) {
                    color_set(WHITE, nullptr);
                }
                else {
                    color_set(GREEN, nullptr);
                }
            }
            
            else if (substr == "black") {
                if (takingAway) {
                    color_set(WHITE, nullptr);
                }
                else {
                    color_set(BLACK, nullptr);
                }
            }
            
            else if (substr == "red") {
                if (takingAway) {
                    color_set(WHITE, nullptr);
                }
                else {
                    color_set(RED, nullptr);
                }
            }
            
            textLine = textLine.substr(0, visible_characters) + textLine.substr(endPos + 1, textLine.length());
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

// if the screen is being completely reset each draw, reset=true
void TextWriter::draw(bool reset) {

    if (adding && soundPlayer != nullptr) {
        soundPlayer->play();
    }

    if (reset) {
        string substr = textLine.substr(0, visible_characters);
        mvaddstr(readerY, readerX, substr.c_str());
        return;
    }

    mvaddch(readerY + cursorY, readerX + cursorX, textLine[visible_characters]);
    cursorX++;
    
    // if the cursor would go over the screen, make it wrap around
    cursorY += cursorX / screenWidth;
    cursorX %= screenWidth;
}

void TextWriter::changeCHPS(int nChps) {
    chps = nChps;
}

void TextWriter::changeRMCHPS(int nRMCHPS) {
    rm_chps = nRMCHPS;
}

void TextWriter::newLine() {
    visible_characters = 0;
    getline(stream, textLine);
    cursorX = readerX;
    cursorY = readerY;
}

void TextWriter::unPause() {
    if (adding) {
        newLine();
    }

    paused = false;
}