#ifndef CARD_H
#define CARD_H

#define COLOR_CARD_BLACK 2
#define COLOR_CARD_RED 1

class Card {
    int suit, rank;

    public:
    Card();
    Card(int suit, int rank);
    int getValue() const;
    void display(int x, int y, bool visible);
};

#endif // CARD_H