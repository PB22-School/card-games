#pragma once
#include <string>
using namespace std;

class InputReciever {

    bool takingInput;
    string input;
    bool* callback;

    bool blinkOn = false;
    double blinkIter = 0;
    double blinkTime = 0.5;

    int recieverX = 2;
    int recieverY = 15;

    public:

    InputReciever();
    void startRecieving(bool* callback);
    void update(int ch, double delta);
    void draw() const;
    string getInput() const;
};