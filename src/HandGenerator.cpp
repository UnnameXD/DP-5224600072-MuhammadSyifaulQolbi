#include <iostream>
#include <cstdlib>
#include "../header/HandGenerator.h"
#include "../header/Hand.h"

Hand HandGenerator::generateHand() {
    std::cout << "Generating cards...\n";

    Hand hand;
    int roll = rand() % 13; // 0-12, one per hand type

    switch (roll) {
        case 0:  hand.isHighCard      = true; break;
        case 1:  hand.isPair          = true; break;
        case 2:  hand.isTwoPair       = true; break;
        case 3:  hand.isThreeOfAKind  = true; break;
        case 4:  hand.isStraight      = true; break;
        case 5:  hand.isFlush         = true; break;
        case 6:  hand.isFullHouse     = true; break;
        case 7:  hand.isFourOfAKind   = true; break;
        case 8:  hand.isStraightFlush = true; break;
        case 9:  hand.isRoyalFlush    = true; break;
        case 10: hand.isFiveOfAKind   = true; break;
        case 11: hand.isFlushHouse    = true; break;
        case 12: hand.isFlushFive     = true; break;
    }

    return hand;
}
