#include <iostream>
#include "../../header/checker/HighCardChecker.h"

// High Card: no other combination matched
HandRank HighCardChecker::check(const Hand& hand) {
    std::cout << "Detected High Card\n";
    return HandRank::HIGH_CARD;
}
