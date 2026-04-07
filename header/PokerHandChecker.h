#pragma once
#include "Hand.h"

enum class HandRank {
    high_card,
    pair,
    two_pair,
    three_of_a_kind,
    straight,
    flush,
    full_house,
    four_of_a_kind,
    straight_flush,
    royal_flush,
    five_of_a_kind,
    flush_house
};

class PokerHandChecker {
public:
    virtual HandRank check(const Hand& hand) = 0;
    void setNext(PokerHandChecker* next);

protected:
    PokerHandChecker* nextChecker = nullptr;
};