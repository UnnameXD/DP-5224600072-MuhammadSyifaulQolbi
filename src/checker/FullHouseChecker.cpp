#include <iostream>
#include "../../header/checker/FullHouseChecker.h"

HandRank FullHouseChecker::check(const Hand& hand) {
    if (hand.isFullHouse) {
        std::cout << "Detected Full House\n";
        return HandRank::FULL_HOUSE;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
