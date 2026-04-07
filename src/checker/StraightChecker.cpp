#include <iostream>
#include "../../header/checker/StraightChecker.h"

HandRank StraightChecker::check(const Hand& hand) {
    std::cout << "Detected Straight\n";
    return HandRank::STRAIGHT;
}