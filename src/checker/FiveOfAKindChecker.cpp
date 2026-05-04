#include <iostream>
#include "../../header/checker/FiveOfAKindChecker.h"
#include "../../header/checker/HandUtils.h"

// Five of a Kind: all 5 cards share the same rank (any suits)
HandRank FiveOfAKindChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5) {
        auto counts = getRankCounts(hand);
        if (counts.size() == 1) {
            std::cout << "Detected Five of a Kind\n";
            return HandRank::FIVE_OF_A_KIND;
        }
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
