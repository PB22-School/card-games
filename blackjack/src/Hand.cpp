#include "Hand.h"

Hand::Hand(ulong visibility) {
    this->visibility = visibility;
}

void Hand::clear() {
    cards.clear();
}

void Hand::add_card(Card card) {
    cards.push_back(card);
}

Card Hand::drop_card(int cardIndex) {
    Card card = cards.at(cardIndex);
    cards.erase(cards.begin() + cardIndex);
    return card;
}

bool Hand::equal() {

    if (cards.empty()) {
        return false;
    }

    int last = cards[0].getValue();
    for (int i = 1; i < cards.size(); i++) {
        if (last != cards[i].getValue()) {
            return false;
        }
    }

    return true;
}

int Hand::size() {
    return cards.size();
}

int Hand::getValue() {

    int sum = 0;
    for (int i = 0; i < cards.size(); i++) {
        sum += cards[i].getValue();
    }

    return sum;
}

void Hand::reveal() {
    visibility = ulong(-1);
}

void Hand::draw(int x, int y) {
    // cards are four chars long
    for (int i = 0; i < cards.size(); i++) {
        cards[i].display(x + (i * 6), y, visibility & (ulong(1) << i));
    }
}