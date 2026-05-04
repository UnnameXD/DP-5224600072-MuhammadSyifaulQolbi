#include <iostream>
#include "../../header/checker/StraightChecker.h"
#include "../../header/checker/HandUtils.h"

// Straight: 5 cards with consecutive ranks (any suits)
HandRank StraightChecker::check(const Hand& hand) {
    if (isConsecutiveRanks(hand)) {
        std::cout << "Detected Straight\n";
        return HandRank::STRAIGHT;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
