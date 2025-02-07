#include "board.h"

Board::Board() {
    for (int i = 0; i < 9; i++) {
        boardRep[i] = 0;
    }
}