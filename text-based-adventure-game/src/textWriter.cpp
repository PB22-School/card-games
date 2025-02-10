#include "TextWriter.h"
#include "ncursesw/ncurses.h"

enum {
    YELLOW = 6,
    GREEN,
    BLACK,
    RED,
    WHITE
};

const string punctuation = ".?!";
const unordered_map<string, string> synonyms = {
    {"look", "inspect"}
};

TextWriter::TextWriter(string filename, SoundPlayer* soundPlayer, InputReciever* inputReciever,int screenWidth, int screenHeight, bool* lineDone, bool* inputDone) : stream("static/text/" + filename) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->soundPlayer = soundPlayer;
    this->inputReciever = inputReciever;
    this->lineDone = lineDone;
    this->inputDone = inputDone;
    
    currentFilename = filename;
    newLine();
}

void TextWriter::update(double delta) {

    if (visible_characters == 0 && textLine[0] == '\\') {
        newLine();
        update(10);
    }

    if (*inputDone && waitingForPlayerInput) {
        // the user just inputted something, so we'll check what it was

        string input = inputReciever->getInput();
        paused = false;
        waitingForPlayerInput = false;

        if (key == "branch") {
            // if we're branching for interactive story telling

            visible_characters = 0;
            adding = true;
            int lastLine = lineNumber;

            bool anyMatch = false;

            while (stream.peek() != EOF) {

                getline(stream, textLine);
                if (textLine[0] == '\t') {
                    continue;
                }
                else if (textLine[0] != '\\') {
                    break;
                }

                int closingCurly = textLine.find_first_of('}');
                string reqs = textLine.substr(2, closingCurly - 2);

                int lastSpace = 0;
                int nextSpace = 0;
                string substr;
                bool match = true;
                while (lastSpace != string::npos) {

                    nextSpace = reqs.find_first_of(' ', lastSpace + 1);
                    substr = reqs.substr(lastSpace, nextSpace);
                    lastSpace = nextSpace;

                    int iLastSpace = 0;
                    int iNextSpace = 0;
                    string iSubstr;
                    bool iMatch = false;
                    while (iLastSpace != string::npos) {
                        
                        iNextSpace = input.find_first_of(' ', iLastSpace + 1);
                        iSubstr = input.substr(iLastSpace, iNextSpace);
                        iLastSpace = iNextSpace;

                        if (substr == iSubstr) {
                            iMatch = true;
                            break;
                        }

                    }

                    if (!iMatch) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    anyMatch = true;
                    textLine = textLine.substr(closingCurly + 2);
                    break;
                }

            }

            if (!anyMatch) {
                textLine = "I don't know what that means.";

                // no matches, so we re-ask the question:
                switchFile(currentFilename);

                string s;
                for (int _ = 1; _ < lastLine; _++) {
                    getline(stream, s);
                }

                lineNumber = lastLine - 1;
            }

            return;
        }

        // also pressing enter should make the text start disappearing
        adding = false;

        // textLine.replace(visible_characters, replaceEnd, "");

        if (wordsHint == 0 || input.find_first_of(' ') == string::npos) {
            variables[key] = input;
            return;
        }

        // it makes more sense to me (at least for now) to have word priority be at the end.
        // I'm thinking about cases like "my name is Parker" "I'm Parker" etc.

        int words = 0;
        size_t lastSpace = string::npos - 1;
        while (words < wordsHint && lastSpace != string::npos) {
            lastSpace = input.find_last_of(' ', lastSpace);
            words++;
        }

        if (lastSpace == string::npos) {
            variables[key] = input;
        }
        else {
            variables[key] = input.substr(lastSpace);
        }
        
    }

    if (paused) {
        return;
    }

    iterTime += delta;

    if ( adding && iterTime > (1.0 / chps) ) {
        iterTime = 0.0;

        if (textLine[visible_characters] == ',') {
            iterTime -= softPunctuationAdd;
        }
        else if (punctuation.find(textLine[visible_characters]) != string::npos) {
            iterTime -= punctuationAdd;
        }
        else if (textLine[visible_characters] == '[') {
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
        else if (textLine[visible_characters] == '{') {

            int endCurly = textLine.find_first_of('}', visible_characters) - visible_characters;

            key = textLine.substr(visible_characters + 1, endCurly - 1);
            
            replaceEnd = endCurly;
            parse();
        }

        visible_characters++;

        if (visible_characters >= textLine.length()) {
            adding = false;
            paused = true;
            *lineDone = true;
            return;
        }
    }

    else if ( !adding && iterTime > (1.0 / rm_chps) ) {
        iterTime = 0;
        visible_characters--;

        if (visible_characters == 0) {
            newLine();
        }
    }
}

void TextWriter::startPlayerInput() {
    pause();
    inputReciever->startRecieving(inputDone);
    waitingForPlayerInput = true;
}

void TextWriter::parse() {

    // keyword branch is used for interactive story telling
    if (key == "branch") {
        startPlayerInput();
        textLine.replace(visible_characters, replaceEnd + 1, "");
        return;
    }


    // name=?1
    // variable name = 1 word from the input
    
    // name=1
    // variable name = "1"

    int eqAt = key.find('=');

    if (eqAt != string::npos) { 
        // we know we're setting var before = to something
        string before = key.substr(0, eqAt);
        string after = key.substr(eqAt + 1);

        // if the 'after' starts with a '?', we're asking the player for the value.
        if (after[0] == '?') {
            // after the '?' there might be a hint as to how many words we want to store.
            if (after.length() >= 2) {
                wordsHint = after[1] - '0';
            }
            else {
                wordsHint = 0;
            }

            key = before;
            startPlayerInput();
            textLine.replace(visible_characters, replaceEnd + 1, "");
        }
        else {
            // otherwise, we're setting it's value.
            variables[before] = after;
            textLine.replace(visible_characters, replaceEnd, "");
        }

    }
    else {
        // if there's no setting, we're querying the value of a variable

        // I'm really not sure why I need a +1 here but nowhere else
        textLine.replace(visible_characters, replaceEnd + 1, variables[key]);
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
    lineNumber++;
    visible_characters = 0;
    getline(stream, textLine);
    cursorX = readerX;
    cursorY = readerY;
    paused = false;
    adding = true;
}

void TextWriter::switchFile(string filename) {
    // A stack overflow post said I didn't need to .close() but I'm too scared of memory leaks.
    stream.close();

    currentFilename = filename;
    lineNumber = -1;
    stream = fstream("static/text/" + filename);
}

void TextWriter::unPause() {

    if (!paused) {
        // this is probably not an amazing way to do this, I'm honestly just being a bit lazy
        while (!paused || visible_characters == 0) {
            update(10);
        }

        return;
    }

    paused = false;
}

void TextWriter::pause() {
    paused = true;
}