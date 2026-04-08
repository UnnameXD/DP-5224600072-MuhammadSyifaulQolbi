#include <iostream>
#include "../../header/checker/HighCardChecker.h"

HandRank HighCardChecker::check(const Hand& hand) {
    if (hand.isHighCard) {
        std::cout << "Detected High Card\n";
        return HandRank::HIGH_CARD;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
