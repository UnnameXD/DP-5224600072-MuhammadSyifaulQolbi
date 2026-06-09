#pragma once
#include "Hand.h"
#include "PokerHandChecker.h"
#include "ScoreContext.h"
#include "./checker/HighCardChecker.h"
#include "./checker/PairChecker.h"
#include "./checker/TwoPairChecker.h"
#include "./checker/ThreeOfAKindChecker.h"
#include "./checker/StraightChecker.h"
#include "./checker/FlushChecker.h"
#include "./checker/FullHouseChecker.h"
#include "./checker/FourOfAKindChecker.h"
#include "./checker/StraightFlushChecker.h"
#include "./checker/RoyalFlushChecker.h"
#include "./checker/FiveOfAKindChecker.h"
#include "./checker/FlushHouseChecker.h"
#include "./checker/FlushFiveChecker.h"

class ScoringRule {
public:
    ScoringRule();
    ScoreContext createContext(const Hand& hand) const;

private:
    FlushFiveChecker     FlushFive;
    FlushHouseChecker    FlushHouse;
    FiveOfAKindChecker   FiveOfAKind;
    RoyalFlushChecker    RoyalFlush;
    StraightFlushChecker StraightFlush;
    FourOfAKindChecker   FourOfAKind;
    FullHouseChecker     FullHouse;
    FlushChecker         Flush;
    StraightChecker      Straight;
    ThreeOfAKindChecker  ThreeOfAKind;
    TwoPairChecker       TwoPair;
    PairChecker          Pair;
    HighCardChecker      HighCard;

    struct BaseScore { int chips; int mult; };
    BaseScore getBaseScore(HandRank rank) const;
    int calcCardChips(const Hand& hand) const;
};
