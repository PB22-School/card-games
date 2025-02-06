#include "randomNumbers.h"
#include "random"

randomNumbers::randomNumbers() {
    srand(time(NULL));
    next *= ulong(random() * rand());
}

ulong randomNumbers::get_next() {
    next *= 31209839 + 2;
    return next;
}