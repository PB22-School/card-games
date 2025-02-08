#include "ai.h"
#include "board.h"

enum scores {
    ONE_IN_A_ROW = 10,
    TWO_IN_A_ROW = 100,
    WIN = 9999,
    LOSS = -9999,
    TIE = 0,
    REALLY_BIG_NUMBER = 999999999
};

int max(int x, int y) {
    if (x > y) {
        return x;
    }

    return y;
}

int howManyOnes(short x) {
    int c = 0;
    while (x) {
        c++;
        x &= x - 1;
    }

    return c;
}

int nOnesToScore(int x) {
    switch (x) {
        case 0:
            return 0;
        case 1:
            return ONE_IN_A_ROW;
        case 2:
            return TWO_IN_A_ROW;
        case 3:
            return WIN;
        default:
            return 0;
    }
}

Ai::Ai() {}

// where bot is minimizing
int Ai::eval(Board* board) {

    if (board->fullBoard == board->piecesBB) {
        return TIE;
    }

    int playerScore = 0;
    int enemyScore = 0;
    for (int i = 0; i < 7; i++) {
        playerScore += nOnesToScore(howManyOnes(board->winLines[i] & board->piecesBB & ~board->enemyBB));
        enemyScore += nOnesToScore(howManyOnes(board->winLines[i] & board->piecesBB & board->enemyBB));
    }

    return playerScore - enemyScore;
}

/*
function negamax(node, depth, color) is
    if depth = 0 or node is a terminal node then
        return color × the heuristic value of node
    value := −∞
    for each child of node do
        value := max(value, −negamax(child, depth − 1, −color))
    return value
*/

int Ai::negamax(Board* board, int depth, bool isPlayer) {
    if (depth == 0 || board->IsGameOver()) {
        return eval(board) * (isPlayer ? 1 : -1);
    }

    int value = -REALLY_BIG_NUMBER;
    MoveList moves = board->GetMoves();
    for (int i = 0; i < moves.size; i++) {
        board->Make(moves.moves[i]);
        value = max(value, -negamax(board, depth - 1, !isPlayer));
        board->UnMake();
    }

    return value;
}

void Ai::turn(Board* board) {
    MoveList moves = board->GetMoves();
    Move bestMove;
    int bestMoveScore = -REALLY_BIG_NUMBER;

    for (int i = 0; i < moves.size; i++) {
        board->Make(moves.moves[i]);
        int cEval = -negamax(board, 10, false);
        board->UnMake();
        if (cEval > bestMoveScore) {
            bestMove = moves.moves[i];
            bestMoveScore = cEval;
        }
    }

    board->visualMake(bestMove);
}