#include <iostream>
#include "../../header/checker/StraightFlushChecker.h"
#include "../../header/checker/HandUtils.h"

// Straight Flush: 5 consecutive ranks all of the same suit
HandRank StraightFlushChecker::check(const Hand& hand) {
    if (isConsecutiveRanks(hand) && allSameSuit(hand)) {
        std::cout << "Detected Straight Flush\n";
        return HandRank::STRAIGHT_FLUSH;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
