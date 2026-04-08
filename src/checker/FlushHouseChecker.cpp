#include <iostream>
#include "../../header/checker/FlushHouseChecker.h"

HandRank FlushHouseChecker::check(const Hand& hand) {
    if (hand.isFlushHouse) {
        std::cout << "Detected Flush House\n";
        return HandRank::FLUSH_HOUSE;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
