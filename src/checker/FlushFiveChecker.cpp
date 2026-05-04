#include <iostream>
#include "../../header/checker/FlushFiveChecker.h"
#include "../../header/checker/HandUtils.h"

// Flush Five: all 5 cards share the same rank AND the same suit
HandRank FlushFiveChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5) {
        auto counts = getRankCounts(hand);
        if (counts.size() == 1 && allSameSuit(hand)) {
            std::cout << "Detected Flush Five\n";
            return HandRank::FLUSH_FIVE;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
