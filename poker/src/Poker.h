#include "Deck.h"
#include "Hand.h"
#include <vector>
#include <string>
using namespace std;

const int N_SUITS = 4;
const int N_RANKS = 13;

enum colors {
    WHITE = 3,
    BLACK,
    YELLOW,
    GREEN,
    RED
};

enum {
    ACE_CARD,
    TWO_CARD,
    THREE_CARD,
    FOUR_CARD,
    FIVE_CARD,
    SIX_CARD,
    SEVEN_CARD,
    EIGHT_CARD,
    NINE_CARD,
    TEN_CARD,
    JACK_CARD,
    QUEEN_CARD,
    KING_CARD
};

enum {
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

class Poker {
    Deck deck;
    Hand DealerHand;
    Hand PlayerHand;
    Hand revealedCards;
    bool gameOver = false;
    bool playerWins = false;
    bool tie = false;
    int playerMoney = 8300;
    int pot = 0;

    int playerScore;
    int playerHandValue;
    int enemyScore;
    int enemyHandValue;

    int buttonSelect = 0;
    vector<string> buttons = {"CALL", "RAISE"};

    public:
    Poker();
    
    void restart_game();
    void start_game();
    bool update();

    void draw_button(string text, int x, int y, int padding);
    int button_length(string text, int padding);
    int hand_value(Hand hand, int* score);

    void draw();

    void call();
    void raise();

};