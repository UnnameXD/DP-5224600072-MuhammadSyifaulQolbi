#pragma once
#include <string>

struct Card {
    int  rank;  // 2-14 (11=J, 12=Q, 13=K, 14=A)
    char suit;  // 'H','D','C','S'

    Card(int r, char s);
    std::string toString() const;
    int getChipValue() const; // 2-9=face, 10/J/Q/K=10, A=11
};
