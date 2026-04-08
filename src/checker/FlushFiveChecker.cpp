#include <iostream>
#include "../../header/checker/FlushFiveChecker.h"

HandRank FlushFiveChecker::check(const Hand& hand) {
    if (hand.isFlushFive) {
        std::cout << "Detected Flush Five\n";
        return HandRank::FLUSH_FIVE;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
