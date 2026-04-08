#include <iostream>
#include "../../header/checker/FlushChecker.h"

HandRank FlushChecker::check(const Hand& hand) {
    if (hand.isFlush) {
        std::cout << "Detected Flush\n";
        return HandRank::FLUSH;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
