// BB-Code Parser & Typewriter effect
#pragma once
#include "SoundPlayer.h"
#include <string>
#include <fstream>
using namespace std;

class TextWriter {
    ifstream stream;
    SoundPlayer* soundPlayer;
    string textLine;
    int visible_characters;

    bool adding; // or removing
    bool paused;

    double chps = 40; // chars per second
    double rm_chps = 60; // remove chars per second

    double iterTime = 0;

    int screenWidth;
    int screenHeight;

    int readerX = 2;
    int readerY = 10;

    int cursorX;
    int cursorY;

    public:
    TextWriter(string filename, SoundPlayer* soundPlayer, int screenWidth, int screenHeight);
    void update(double delta);
    void draw(bool reset);

    void changeCHPS(int nChps);
    void changeRMCHPS(int nRMCHPS);

    void newLine();
    void unPause();
};