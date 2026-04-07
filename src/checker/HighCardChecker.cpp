#pragma once
#include <iostream>
#include "../../header/PokerHandChecker.h"

class HighCardChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        std::cout << "Detected High Card\n";
        return HandRank::HIGH_CARD;
    }
};