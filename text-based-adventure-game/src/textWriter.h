// BB-Code Parser & Typewriter effect
#pragma once
#include "SoundPlayer.h"
#include "InputReciever.h"
#include <string>
#include <fstream>
#include <unordered_map>
using namespace std;

class TextWriter {
    SoundPlayer* soundPlayer;
    
    // User Input & Variables
    InputReciever* inputReciever;

    int wordsHint;
    string key;
    unordered_map<string, string> variables;

    int replaceEnd;
    bool waitingForPlayerInput = false;

    bool paused;

    bool* lineDone;
    bool* inputDone;

    string currentFilename;
    int lineNumber;

    // Drawing
    fstream stream;
    string textLine;
    int visible_characters;

    bool adding; // or removing

    double chps = 30; // chars per second
    double punctuationAdd = 0.5;
    double softPunctuationAdd = 0.2;
    double rm_chps = 120; // remove chars per second
    double iterTime = 0;

    int screenWidth;
    int screenHeight;

    int readerX = 2;
    int readerY = 10;

    int cursorX;
    int cursorY;

    public:
    TextWriter(string filename, SoundPlayer* soundPlayer, InputReciever* inputReciever, int screenWidth, int screenHeight, bool* lineDone = nullptr, bool* inputDone = nullptr);

    void update(double delta);
    void parse();

    void draw(bool reset);

    void changeCHPS(int nChps);
    void changeRMCHPS(int nRMCHPS);

    void startPlayerInput();

    void newLine();
    void switchFile(string filename);

    void unPause();
    void pause();
};