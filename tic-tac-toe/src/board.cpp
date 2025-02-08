#include "board.h"
#include <random>

Board::Board() {

    piecesBB = 0;
    enemyBB = 0;

    for (int i = 0; i < 9; i++) {
        boardRep[i] = 0;
    }

    boardChanged = false;
    srand(time(NULL));
    isPlayerTurn = rand() % 2;
    playerIsOs = !isPlayerTurn;
}

MoveList Board::GetMoves() {
    MoveList movelist;

    for (int i = 0; i < 9; i++) {
        if (piecesBB & (short(1) << i)) {
            continue;
        }
        movelist.addMove(Move(i, !isPlayerTurn));
    }

    return movelist;
}

void Board::visualMake(Move move) {
    Make(move);
    boardRep[move.data & 0xf] = move.getIsEnemy() + 1;
}

void Board::Make(Move move) {
    stack.push(move);
    piecesBB |= move.getToBitboard();
    enemyBB |= move.getToBitboard() * move.getIsEnemy();
    boardChanged = true;
    isPlayerTurn = !isPlayerTurn;
}

void Board::UnMake() {
    Move move = stack.pop();
    piecesBB ^= move.getToBitboard();
    enemyBB ^= move.getToBitboard() * move.getIsEnemy();
    boardChanged = false;
    isPlayerTurn = !isPlayerTurn;
}

bool Board::IsGameOver() {

    if (fullBoard == piecesBB) {
        return true;
    }

    for (int i = 0; i < 7; i++) {
        if ((winLines[i] & piecesBB) == winLines[i]) {
            return true;
        }
    }

    return false;
}

Move::Move() {
    data = -1;
}

Move::Move(char pos, bool isEnemy) {
    data = (pos & 0xf) | (isEnemy << 4);
}

bool Move::getIsEnemy() {
    return data & 0x10;
}

short Move::getToBitboard() {
    return short(1) << (data & 0xf);
}

Stack::Stack() {
    size = 0;
}

bool Stack::isEmpty() {
    return size == 0;
}

u8 Stack::getSize() {
    return size;
}

Move Stack::pop() {
    Move move = stack_moves[size - 1];
    size--;
    return move;
}

void Stack::push(Move move) {
    stack_moves[size] = move;
    size++;
}

MoveList::MoveList() {
    size = 0;
}

void MoveList::addMove(Move move) {
    moves[size] = move;
    size++;
}