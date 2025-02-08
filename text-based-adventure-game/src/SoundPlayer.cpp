#include "SoundPlayer.h"

#ifdef __APPLE__
#include <thread>

void playApple(string filename) {
    string cmd = "afplay 'static/sounds/" + filename + "'";
    system(cmd.c_str());
}

#endif



SoundPlayer::SoundPlayer(string filename) {
    switchSound(filename);
}

void SoundPlayer::switchSound(string filename) {
    this->filename = filename;
}

void SoundPlayer::play() {

    // No sound on Linux yet, sorry!

    #ifdef _WIN32
    PlaySound("static/sounds/" + filename, NULL, SND_FILENAME | SND_ASYNC);
    #endif
    #ifdef __APPLE__
    // this is probably not a great way to do this, but it works.
    thread t(playApple, filename);
    t.detach();
    #endif
}