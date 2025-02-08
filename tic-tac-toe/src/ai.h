#include "board.h"

class Ai {
    public:

    Ai();
    int eval(Board* board);
    int negamax(Board* board, int depth, bool isPlayer);
    void turn(Board* board);
};