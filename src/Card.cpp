#include "Card.h"

Card::Card(int r, char s) : rank(r), suit(s) {}

std::string Card::toString() const {
    std::string rankStr;
    if      (rank == 11) rankStr = "J";
    else if (rank == 12) rankStr = "Q";
    else if (rank == 13) rankStr = "K";
    else if (rank == 14) rankStr = "A";
    else                 rankStr = std::to_string(rank);
    return rankStr + "-" + suit;
}

int Card::getChipValue() const {
    if (rank >= 2  && rank <= 9)  return rank;
    if (rank >= 10 && rank <= 13) return 10;
    if (rank == 14)               return 11;
    return 0;
}
