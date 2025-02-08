#pragma once

typedef unsigned char u8;

struct Move {
    char data;
    Move();
    Move(char pos, bool isEnemy);
    bool getIsEnemy();
    short getToBitboard();
};

struct Stack {
    private:
    Move stack_moves[64];
    u8 size;

    public:
    Stack();
    bool isEmpty();
    u8 getSize();
    Move pop();
    void push(Move move);
};

struct MoveList {
    int size;
    Move moves[64];
    MoveList();
    void addMove(Move move);
};

class Board {

    Stack stack;

    public:
    short winLines[8] = {0x7, 0x38, 0x1c0, 0x49, 0x92, 0x124, 0x111, 0x54};
    short fullBoard = 0x1ff;

    bool isPlayerTurn;
    bool playerIsOs;

    short piecesBB;
    short enemyBB;

    Board();
    MoveList GetMoves();
    void Make(Move move);
    void visualMake(Move move);
    void UnMake();
    bool IsGameOver();
    bool boardChanged;

    char boardRep[9];
};