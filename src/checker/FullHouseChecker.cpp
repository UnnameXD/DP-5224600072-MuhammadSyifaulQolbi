#include <iostream>
#include "../../header/checker/FullHouseChecker.h"
#include "../../header/checker/HandUtils.h"

// Full House: three cards of one rank + two cards of another rank
HandRank FullHouseChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5) {
        auto counts = getRankCounts(hand);
        bool hasThree = false, hasTwo = false;
        for (const auto& [rank, cnt] : counts) {
            if (cnt == 3) hasThree = true;
            else if (cnt == 2) hasTwo = true;
        }
        if (hasThree && hasTwo) {
            std::cout << "Detected Full House\n";
            return HandRank::FULL_HOUSE;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
