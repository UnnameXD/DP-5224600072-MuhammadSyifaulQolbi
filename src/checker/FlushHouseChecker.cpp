#include <iostream>
#include "../../header/checker/FlushHouseChecker.h"
#include "../../header/checker/HandUtils.h"

// Flush House: full house (3+2) where all cards share the same suit
HandRank FlushHouseChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5 && allSameSuit(hand)) {
        auto counts = getRankCounts(hand);
        bool hasThree = false, hasTwo = false;
        for (const auto& [rank, cnt] : counts) {
            if (cnt == 3) hasThree = true;
            else if (cnt == 2) hasTwo = true;
        }
        if (hasThree && hasTwo) {
            std::cout << "Detected Flush House\n";
            return HandRank::FLUSH_HOUSE;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
