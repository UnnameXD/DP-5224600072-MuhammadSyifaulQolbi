#include <iostream>
#include "../../header/checker/FlushHouseChecker.h"

HandRank PairChecker::check(const Hand& hand) {
    std::cout << "Detected Flush House\n";
    return HandRank::FLUSH_HOUSE;
}