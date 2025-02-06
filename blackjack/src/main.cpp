#include "Card.h"
#include "BlackJack.h"
#include "ncursesw/ncurses.h"
#include <locale.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    start_color();
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);

    init_pair(COLOR_CARD_RED, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_CARD_BLACK, COLOR_WHITE, COLOR_BLACK);

    BlackJack game = BlackJack();
    game.start_game();

    endwin();
}