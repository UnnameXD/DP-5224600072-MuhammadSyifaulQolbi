#include <iostream>
#include "../../header/checker/RoyalFlushChecker.h"

HandRank RoyalFlushChecker::check(const Hand& hand) {
    if (hand.isRoyalFlush) {
        std::cout << "Detected Royal Flush\n";
        return HandRank::ROYAL_FLUSH;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
