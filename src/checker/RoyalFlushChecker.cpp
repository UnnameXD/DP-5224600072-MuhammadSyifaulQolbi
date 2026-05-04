#include <iostream>
#include <vector>
#include <algorithm>
#include "../../header/checker/RoyalFlushChecker.h"
#include "../../header/checker/HandUtils.h"

// Royal Flush: 10, J, Q, K, A all of the same suit
HandRank RoyalFlushChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5 && allSameSuit(hand)) {
        std::vector<int> ranks;
        for (const auto& c : hand.cards) ranks.push_back(c.rank);
        std::sort(ranks.begin(), ranks.end());
        if (ranks == std::vector<int>{10, 11, 12, 13, 14}) {
            std::cout << "Detected Royal Flush\n";
            return HandRank::ROYAL_FLUSH;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
