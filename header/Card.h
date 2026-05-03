#pragma once
#include <string>

struct Card {
    int rank;   
    char suit;  

    Card(int r, char s);
    std::string toString() const;
};