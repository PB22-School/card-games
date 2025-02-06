#include "Card.h"
#include "ncursesw/ncurses.h"

Card::Card() {
    suit = 0;
    rank = 0;
}

Card::Card(int suit, int rank) {
    this->suit = suit;
    this->rank = rank;
}

int Card::getValue() const {
    return rank;
}


/*
  0123
0 [--]
1 |4 |
2 |* |
3 [--]

*/
void Card::display(int x, int y, bool visible = false) {

    if (suit == 0 || suit == 3) {
        color_set(COLOR_CARD_BLACK, nullptr);
    }
    else {
        color_set(COLOR_CARD_RED, nullptr);
    }

    if (visible) {
        switch (suit) {
        case 0:
            mvaddstr(y + 2, x + 1, "♣ ");
            break;
        case 1:
            mvaddstr(y + 2, x + 1, "♦ ");
            break;
        case 2:
            mvaddstr(y + 2, x + 1, "♥ ");
            break;
        case 3:
            mvaddstr(y + 2, x + 1, "♠ ");
            break;
        default:
            mvaddstr(y + 2, x + 1, "? ");
            break;
        }
    }
    else {
        mvaddstr(y + 1, x + 1, "█ ");
        mvaddstr(y + 2, x + 1, " █");
    }

    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + 3, ACS_URCORNER);
    mvaddch(y + 3, x, ACS_LLCORNER);
    mvaddch(y + 3, x + 3, ACS_LRCORNER);
    mvhline(y, x + 1, ACS_HLINE, 2);
    mvhline(y + 3, x + 1, ACS_HLINE, 2);
    mvvline(y + 1, x, ACS_VLINE, 2);
    mvvline(y + 1, x + 3, ACS_VLINE, 2);

    if (!visible) {
        return;
    }

    switch (rank) {
    case 1:
        mvaddstr(y + 1, x + 1, "A ");
        break;
    case 2:
        mvaddstr(y + 1, x + 1, "2 ");
        break;
    case 3:
        mvaddstr(y + 1, x + 1, "3 ");
        break;
    case 4:
        mvaddstr(y + 1, x + 1, "4 ");
        break;
    case 5:
        mvaddstr(y + 1, x + 1, "5 ");
        break;
    case 6:
        mvaddstr(y + 1, x + 1, "6 ");
        break;
    case 7:
        mvaddstr(y + 1, x + 1, "7 ");
        break;
    case 8:
        mvaddstr(y + 1, x + 1, "8 ");
        break;
    case 9:
        mvaddstr(y + 1, x + 1, "9 ");
        break;
    case 10:
        mvaddstr(y + 1, x + 1, "10");
        break;
    case 11:
        mvaddstr(y + 1, x + 1, "J ");
        break;
    case 12:
        mvaddstr(y + 1, x + 1, "Q ");
        break;
    case 13:
        mvaddstr(y + 1, x + 1, "K ");
        break;
    default:
        mvaddstr(y + 1, x + 1, "? ");
        break;
    }

}