#include <iostream>
#include "../../header/checker/PairChecker.h"

HandRank PairChecker::check(const Hand& hand) {
    if (hand.isPair) {
        std::cout << "Detected Pair\n";
        return HandRank::PAIR;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
