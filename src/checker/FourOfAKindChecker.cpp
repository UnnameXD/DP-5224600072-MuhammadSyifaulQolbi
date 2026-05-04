#include <iostream>
#include "../../header/checker/FourOfAKindChecker.h"
#include "../../header/checker/HandUtils.h"

// Four of a Kind: four cards of the same rank
HandRank FourOfAKindChecker::check(const Hand& hand) {
    auto counts = getRankCounts(hand);
    for (const auto& [rank, cnt] : counts) {
        if (cnt == 4) {
            std::cout << "Detected Four of a Kind\n";
            return HandRank::FOUR_OF_A_KIND;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
