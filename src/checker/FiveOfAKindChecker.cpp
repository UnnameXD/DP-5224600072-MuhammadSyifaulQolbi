#include <iostream>
#include "../../header/checker/FiveOfAKindChecker.h"

HandRank FiveOfAKindChecker::check(const Hand& hand) {
    if (hand.isFiveOfAKind) {
        std::cout << "Detected Five of a Kind\n";
        return HandRank::FIVE_OF_A_KIND;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
