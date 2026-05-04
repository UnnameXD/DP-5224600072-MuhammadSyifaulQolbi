#include <iostream>
#include "../../header/checker/FlushChecker.h"
#include "../../header/checker/HandUtils.h"

// Flush: all 5 cards share the same suit (not necessarily consecutive)
HandRank FlushChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5 && allSameSuit(hand)) {
        std::cout << "Detected Flush\n";
        return HandRank::FLUSH;
    }
    if (nextChecker) return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
