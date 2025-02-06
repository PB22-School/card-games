#include "Deck.h"

void Deck::shuffleDeck() {
    for (int i = 0; i < cards.size(); i++) {
        Card c1 = cards[i];

        int rand = rng.get_next() % cards.size();

        cards[i] = cards[rand];
        cards[rand] = c1;
    }
}

Deck::Deck() {
    resetDeck();
}

Card Deck::getCard() {

    int index = rng.get_next() % cards.size();

    Card retCard = cards.at(index);
    cards.erase(cards.begin() + index);

    return retCard;
}

void Deck::resetDeck() {
    cards.clear();

    for (int cardIndex = ACE; cardIndex < DECKSIZE_MAX; cardIndex++) {
        cards.push_back(Card(cardIndex / KING, (cardIndex % KING) + 1));
    }

    shuffleDeck();
}