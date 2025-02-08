// BB-Code Parser & Typewriter effect
#pragma once
#include <string>
#include <fstream>
using namespace std;

class textWriter {
    ifstream stream;
    char* textLine;
    int visible_characters;

    bool adding; // or removing
    bool paused;

    double chps = 1; // chars per second
    double rm_chps = 5; // remove chars per second

    double iterTime = 0;

    int screenWidth;
    int screenHeight;

    int readerX = 20;
    int readerY = 30;

    int cursorX;
    int cursorY;

    public:
    textWriter(string filename, int screenWidth, int screenHeight);
    void update(double delta);

    void changeCHPS(int nChps);
    void changeRMCHPS(int nRMCHPS);

    void newLine();
    void unPause();
};