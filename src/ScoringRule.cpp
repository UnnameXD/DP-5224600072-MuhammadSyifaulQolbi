#include <iostream>
#include "ScoringRule.h"

ScoringRule::ScoringRule() {
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

ScoreContext ScoringRule::createContext(const Hand& hand) const {
    HandRank rank  = const_cast<FlushFiveChecker&>(FlushFive).check(hand);
    BaseScore base = getBaseScore(rank);
    int cardChips  = calcCardChips(hand);

    ScoreContext ctx;
    ctx.handRank  = rank;
    ctx.baseChips = base.chips;
    ctx.baseMult  = base.mult;
    ctx.chips     = base.chips + cardChips;
    ctx.mult      = base.mult;
    return ctx;
}

ScoringRule::BaseScore ScoringRule::getBaseScore(HandRank rank) const {
    switch (rank) {
        case HandRank::FLUSH_FIVE:      return {160, 16};
        case HandRank::FLUSH_HOUSE:     return {140, 14};
        case HandRank::FIVE_OF_A_KIND:  return {120, 12};
        case HandRank::ROYAL_FLUSH:     return {100,  8};
        case HandRank::STRAIGHT_FLUSH:  return {100,  8};
        case HandRank::FOUR_OF_A_KIND:  return { 60,  7};
        case HandRank::FULL_HOUSE:      return { 40,  4};
        case HandRank::FLUSH:           return { 35,  4};
        case HandRank::STRAIGHT:        return { 30,  4};
        case HandRank::THREE_OF_A_KIND: return { 30,  3};
        case HandRank::TWO_PAIR:        return { 20,  2};
        case HandRank::PAIR:            return { 10,  2};
        case HandRank::HIGH_CARD:       return {  5,  1};
        default:                        return {  5,  1};
    }
}

int ScoringRule::calcCardChips(const Hand& hand) const {
    int total = 0;
    for (const auto& c : hand.cards)
        total += c.getChipValue();
    return total;
}
