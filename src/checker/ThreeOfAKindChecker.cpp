#include <iostream>
#include "../../header/checker/ThreeOfAKindChecker.h"
#include "../../header/checker/HandUtils.h"

// Three of a Kind: exactly three cards of the same rank
HandRank ThreeOfAKindChecker::check(const Hand& hand) {
    auto counts = getRankCounts(hand);
    for (const auto& [rank, cnt] : counts) {
        if (cnt == 3) {
            std::cout << "Detected Three of a Kind\n";
            return HandRank::THREE_OF_A_KIND;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
