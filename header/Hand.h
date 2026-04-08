#pragma once

struct Hand {
    int value = 0;

    // Exactly one of these will be true, set by HandGenerator
    bool isFlushFive     = false;
    bool isFlushHouse    = false;
    bool isFiveOfAKind   = false;
    bool isRoyalFlush    = false;
    bool isStraightFlush = false;
    bool isFourOfAKind   = false;
    bool isFullHouse     = false;
    bool isFlush         = false;
    bool isStraight      = false;
    bool isThreeOfAKind  = false;
    bool isTwoPair       = false;
    bool isPair          = false;
    bool isHighCard      = false;
};
