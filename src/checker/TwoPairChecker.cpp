#include <iostream>
#include "../../header/checker/TwoPairChecker.h"

HandRank TwoPairChecker::check(const Hand& hand) {
    if (hand.isTwoPair) {
        std::cout << "Detected Two Pair\n";
        return HandRank::TWO_PAIR;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
