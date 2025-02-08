#pragma once
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

class SoundPlayer {

    string filename;

    public:
    SoundPlayer(string filename);
    void switchSound(string filename);
    void play();
};