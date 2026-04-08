#include <iostream>
#include "../header/ScoringRule.h"

ScoringRule::ScoringRule() {
    // Chain from strongest to weakest poker hand
    FlushFive.setNext(&FlushHouse);
    FlushHouse.setNext(&FiveOfAKind);
    FiveOfAKind.setNext(&RoyalFlush);
    RoyalFlush.setNext(&StraightFlush);
    StraightFlush.setNext(&FourOfAKind);
    FourOfAKind.setNext(&FullHouse);
    FullHouse.setNext(&Flush);
    Flush.setNext(&Straight);
    Straight.setNext(&ThreeOfAKind);
    ThreeOfAKind.setNext(&TwoPair);
    TwoPair.setNext(&Pair);
    Pair.setNext(&HighCard);
}

int ScoringRule::scoreHand(const Hand& hand) {
    std::cout << "Calculating hand score...\n";

    HandRank rank = FlushFive.check(hand);

    int score = convertRankToScore(rank);

    std::cout << "Final score = " << score << "\n";
    return score;
}

int ScoringRule::convertRankToScore(HandRank rank) {
    switch (rank) {
        case HandRank::FLUSH_FIVE: return 100;
        case HandRank::FLUSH_HOUSE: return 75;
        case HandRank::FIVE_OF_A_KIND: return 50;
        case HandRank::ROYAL_FLUSH: return 40;
        case HandRank::STRAIGHT_FLUSH: return 30;
        case HandRank::FOUR_OF_A_KIND: return 25;
        case HandRank::FULL_HOUSE: return 20;
        case HandRank::FLUSH: return 15;
        case HandRank::STRAIGHT: return 10;
        case HandRank::THREE_OF_A_KIND: return 7;
        case HandRank::TWO_PAIR: return 5;
        case HandRank::PAIR: return 2;
        case HandRank::HIGH_CARD: return 1;
        default: return 0;
    }
}