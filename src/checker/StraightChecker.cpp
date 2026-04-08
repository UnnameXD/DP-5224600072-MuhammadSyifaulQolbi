#include <iostream>
#include "../../header/checker/StraightChecker.h"

HandRank StraightChecker::check(const Hand& hand) {
    if (hand.isStraight) {
        std::cout << "Detected Straight\n";
        return HandRank::STRAIGHT;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
