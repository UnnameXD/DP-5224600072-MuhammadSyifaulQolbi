#include <iostream>
#include "../../header/checker/StraightFlushChecker.h"

HandRank StraightFlushChecker::check(const Hand& hand) {
    if (hand.isStraightFlush) {
        std::cout << "Detected Straight Flush\n";
        return HandRank::STRAIGHT_FLUSH;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
