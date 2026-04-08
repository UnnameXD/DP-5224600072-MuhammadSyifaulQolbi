#include <iostream>
#include "../../header/checker/ThreeOfAKindChecker.h"

HandRank ThreeOfAKindChecker::check(const Hand& hand) {
    if (hand.isThreeOfAKind) {
        std::cout << "Detected Three of a Kind\n";
        return HandRank::THREE_OF_A_KIND;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
