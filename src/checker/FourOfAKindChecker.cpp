#include <iostream>
#include "../../header/checker/FourOfAKindChecker.h"

HandRank FourOfAKindChecker::check(const Hand& hand) {
    if (hand.isFourOfAKind) {
        std::cout << "Detected Four of a Kind\n";
        return HandRank::FOUR_OF_A_KIND;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
