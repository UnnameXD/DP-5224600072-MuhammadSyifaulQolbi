#include <iostream>
#include "../../header/checker/PairChecker.h"
#include "../../header/checker/HandUtils.h"

// Pair: exactly two cards of the same rank
HandRank PairChecker::check(const Hand& hand) {
    auto counts = getRankCounts(hand);
    for (const auto& [rank, cnt] : counts) {
        if (cnt == 2) {
            std::cout << "Detected Pair\n";
            return HandRank::PAIR;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
