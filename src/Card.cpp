#include "../header/Card.h"
#include <iostream>

Card::Card(int r, char s) : rank(r), suit(s) { }

std::string Card::toString() const {
    std::string rankStr;

    if (rank == 11) rankStr = "J";
    else if (rank == 12) rankStr = "Q";
    else if (rank == 13) rankStr = "K";
    else if (rank == 14) rankStr = "A";
    else rankStr = std::to_string(rank);

    return rankStr + "-" + suit;
}

