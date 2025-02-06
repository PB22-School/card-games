#include "Card.h"
#include "randomNumbers.h"
#include <vector>
const int ACE = 1;
const int KING = 13;
const int MIN_FACE_CARD = 10;
const int DECKSIZE_MAX = 52;
using namespace std;

class Deck {
    vector<Card> cards;
    randomNumbers rng;

    public:
    Deck();
    Card getCard();
    void shuffleDeck();
    void resetDeck();
};