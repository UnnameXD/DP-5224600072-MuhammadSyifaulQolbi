#include <iostream>
#include "../../header/checker/TwoPairChecker.h"
#include "../../header/checker/HandUtils.h"

// Two Pair: two separate pairs of cards with matching ranks
HandRank TwoPairChecker::check(const Hand& hand) {
    auto counts = getRankCounts(hand);
    int pairs = 0;
    for (const auto& [rank, cnt] : counts)
        if (cnt == 2) pairs++;
    if (pairs >= 2) {
        std::cout << "Detected Two Pair\n";
        return HandRank::TWO_PAIR;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
