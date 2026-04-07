#include <iostream>
#include "../../header/checker/Straight.h"

HandRank PairChecker::check(const Hand& hand) {
    std::cout << "Detected Straight\n";
    return HandRank::PAIR;
}