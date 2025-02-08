#include "Poker.h"
#include "ncursesw/ncurses.h"

#include <iostream>
using namespace std;

const char* handLevels[10] = {"High Card", "Pair", "Two Pair", "Three Of A Kind", 
                             "Straight", "Flush", "Full House", "Four Of A Kind", "Straight Flush",
                             "Royal Flush"};

string itos(int x) {
    string s = "";

    if (!x) {
        return "0";
    }

    x = abs(x);

    while (x) {
        s += (x % 10) + '0';
        x /= 10;
    }

    for (int i = 0; i < s.length() / 2; i++) {
        char c = s[i];
        s[i] = s[s.length() - (i + 1)];
        s[s.length() - (i + 1)] = c;
    }

    return s;
}

void Poker::restart_game() {
    gameOver = false;
    pot = 0;

    tie = false;

    deck.resetDeck();
    DealerHand.clear();
    PlayerHand.clear();
    revealedCards.clear();

    DealerHand.visibility = 0;

    for (int _ = 0; _ < 2; _++) {
        DealerHand.add_card(deck.getCard());
        PlayerHand.add_card(deck.getCard());
    }

    draw();
}

Poker::Poker() : DealerHand(0) {
    restart_game();

    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(RED, COLOR_WHITE, COLOR_RED);
}

void Poker::start_game() {

    gameOver = false;
    draw();

    while (true) {
        if (!gameOver) {
            if (update()) {
                return;
            }
        }
        else if (gameOver) {
            mvaddstr(32, 80, "Press 'Q' to Quit.");
            mvaddstr(34, 80, "Press Any Other Key to Replay.");
            char c = getch();
            if (c == 'q') {
                return;
            }
            else if (c != ERR) {
                restart_game();
            }
        }
    }

}

// returns if game is over
bool Poker::update() {
    bool changes = true;

    char gotChar = getch();

    switch (gotChar) {
        case 'q':
            return true;

        case 'a':
            buttonSelect = buttonSelect - 1;
            if (buttonSelect <= -1) {
                buttonSelect = buttons.size() - 1;
            }
            break;

        case 'd':
            buttonSelect = (buttonSelect + 1) % buttons.size();
            break;
        
        case ' ':
            if (buttons[buttonSelect] == "CALL") {
                call();
            }
            else if (buttons[buttonSelect] == "RAISE") {
                raise();
            }
            break;

        default:
            changes = false;
            break;
    }

    if (changes) {
        draw();
    }

    return false;
}

int Poker::hand_value(Hand hand, int* score) {

    int suits[N_SUITS] = {0,0,0,0};
    int ranks[N_RANKS] = {0,0,0, 0,0,0, 0,0,0, 0,0,0,0};
    int highestHandCard = max(hand.cards[0].getRank(), hand.cards[1].getRank());

    for (int i = 0; i < revealedCards.size(); i++) {
        suits[revealedCards.cards[i].getSuit()]++;
        ranks[revealedCards.cards[i].getRank() - 1]++;
    }

    for (int i = 0; i < hand.size(); i++) {
        suits[hand.cards[i].getSuit()]++;
        ranks[hand.cards[i].getRank() - 1]++;
    }

    // Flush = 5 same suit cards
    bool flush = false;
    for (int i = 0; i < N_SUITS; i++) {
        if (suits[i] >= 5) {
            flush = true;
            break;
        }
    }
    
    // Straight = 5 increasing cards
    bool straight = false;
    int straightStart;
    int inc = 0;

    // Pairs
    bool setPair = true;
    int pair1 = -1;
    int pair2 = -1;
    int threeOfAKind = -1;
    int fourOfAKind = -1;

    *score = highestHandCard;

    for (int i = 0; i < N_RANKS; i++) {

        if (ranks[i] == 2) {
            if (setPair) {
                pair1 = i;
            }
            else {
                pair2 = i;
            }

            setPair = !setPair;
        }
        else if (ranks[i] == 3) {
            threeOfAKind = max(threeOfAKind, i);
        }
        else if (ranks[i] == 4) {
            fourOfAKind = max(fourOfAKind, i);
        }


        if (ranks[i] != 0) {
            inc++;

            if (inc == 1) {
                straightStart = i;
            }
        }
        else {
            inc = 0;
        }

        if (inc >= 5) {
            straight = true;
            break;
        }
        else if (inc == 4 && i == KING_CARD && ranks[ACE_CARD]) {
            straight = true;
            break;
        } 
    }

    // Royal Flush ?
    if (straight && flush && straightStart == TEN_CARD) {
        *score = 0;
        return ROYAL_FLUSH;
    }

    // Straight Flush ?
    if (straight && flush) {
        *score = straightStart;
        return STRAIGHT_FLUSH;
    }

    // Four Of A Kind ?
    if (fourOfAKind != -1) {
        *score = fourOfAKind;
        return FOUR_OF_A_KIND;
    } 
    
    // Full House ?
    if (threeOfAKind != -1 && pair1 != -1) {
        *score = (threeOfAKind * 2) + max(pair1, pair2);
        return THREE_OF_A_KIND;
    }

    // Flush ?
    if (flush) {
        return FLUSH;
    }

    // Straight ?
    if (straight) {
        *score = straightStart;
        return STRAIGHT;
    }

    // Three Of A Kind ?
    if (threeOfAKind != -1) {
        *score = threeOfAKind;
        return THREE_OF_A_KIND;
    }

    // Two Pair ?
    if (pair1 != -1 && pair2 != -1) {
        *score = pair1 + pair2;
        return TWO_PAIR;
    }

    // Pair ?
    if (pair1 != -1) {
        *score = pair1;
        return PAIR;
    }

    return HIGH_CARD;
}

void Poker::call() {

    if (gameOver) {
        return;
    }

    if (revealedCards.size() < 5) {
        revealedCards.add_card(deck.getCard());
    }
    else if (revealedCards.size() == 5) {
        gameOver = true;
        DealerHand.reveal();

        playerHandValue = hand_value(PlayerHand, &playerScore);
        enemyHandValue = hand_value(DealerHand, &enemyScore);

        if (playerHandValue > enemyHandValue) {
            playerWins = true;
        }
        else if (playerHandValue < enemyHandValue) {
            playerWins = false;
        }
        else {
            if (playerScore > enemyScore) {
                playerWins = true;
            }
            else if (playerScore < enemyScore) {
                playerWins = false;
            }
            else {
                tie = true;
            }
        }

        if (tie) {
            playerMoney += pot;
            return;
        }
        else if (playerWins) {
            playerMoney += pot * 2;
        }
    }
}

void Poker::raise() {

    if (gameOver) {
        return;
    }

    pot += 100;
    playerMoney -= 100;
}

/*

  0 1 2 3 4 5 6 7
0 [ - - - - - - ]
1 | * * * * * * |
2 | * T e s t * |
3 | * * * * * * |
4 [ - - - - - - ]

padding = 1 ^^^^^
(represented by *'s)

*/
void Poker::draw_button(string text, int x, int y, int padding) {

    int len = text.length();
    int half = padding / 2;
    int twice = (padding * 2) + 1;

    int width = twice + len;
    int height = twice;

    mvhline(y, x + 1, ACS_HLINE, width - 1);
    mvhline(y + twice + 1, x + 1, ACS_HLINE, width - 1);

    mvvline(y + 1, x, ACS_VLINE, height);
    mvvline(y + 1, x + width, ACS_VLINE, height);

    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y + height + 1, x, ACS_LLCORNER);
    mvaddch(y, x + width, ACS_URCORNER);
    mvaddch(y + height + 1, x + width, ACS_LRCORNER);

    mvaddstr(y + padding + 1, x + padding + 1, text.c_str());

}

int Poker::button_length(string text, int padding) {
    return text.length() + (padding * 2) + 2;
}

void Poker::draw() {

    clear();
    DealerHand.draw(50, 10);
    PlayerHand.draw(50, 30);
    revealedCards.draw(65, 20);

    color_set(YELLOW, nullptr);
    mvaddstr(5, 10, "Use the 'a' and 'd' keys to control the buttons!");
    mvaddstr(6, 10, "Use the SPACEBAR to select an option!");

    int sum = 0;

    for (int i = 0; i < buttons.size(); i++) {
        if (buttonSelect == i) {
            color_set(YELLOW, nullptr);
        }
        else {
            color_set(WHITE, nullptr);
        }

        // mvaddstr(20, 20 + (i * 3), buttons[i].c_str());
        draw_button(buttons[i], sum + 20 + (i * 2), 50, 2);
        sum += button_length(buttons[i], 2);

    }

    if (playerMoney >= 0) {
        color_set(GREEN, nullptr);
        mvaddstr(10, 80, "Money: $");
        mvaddstr(10, 88, itos(playerMoney).c_str());
        mvaddstr(10, 88 + itos(playerMoney).length(), "!");
    }
    else {
        color_set(RED, nullptr);
        mvaddstr(10, 80, "Money: -$");
        mvaddstr(10, 89, itos(playerMoney).c_str());
        mvaddstr(10, 89 + itos(playerMoney).length(), "!");
    }

    
    color_set(WHITE, nullptr);
    mvaddstr(12, 80, "Pot: $");
    mvaddstr(12, 86, itos(pot).c_str());
    mvaddstr(12, 86 + itos(pot).length(), "!");



    if (gameOver) {

        color_set(WHITE, nullptr);
        mvaddstr(10, 20, "Dealer's Hand:");
        mvaddstr(11, 20, handLevels[enemyHandValue]);
        mvaddstr(20, 20, "Your Hand:");
        mvaddstr(21, 20, handLevels[playerHandValue]);

        if (tie) {
            color_set(YELLOW, nullptr);
            mvaddstr(30, 80, "YOU TIED! YOU LOST $");
            mvaddstr(30, 100, itos(pot / 2).c_str());
            mvaddstr(30, 100 + itos(pot / 2).length(), "!");
        }
        else if (playerWins) {
            color_set(GREEN, nullptr);
            mvaddstr(30, 80, "YOU WIN $");
            mvaddstr(30, 89, itos(pot * 2).c_str());
            mvaddstr(30, 89 + itos(pot * 2).length(), "!");
        }
        else {
            color_set(RED, nullptr);
            mvaddstr(30, 80, "YOU LOST $");
            mvaddstr(30, 90, itos(pot).c_str());
            mvaddstr(30, 90 + itos(pot).length(), "!");
        }
    }
}