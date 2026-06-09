#pragma once
#include "PokerHandChecker.h"

// Mutable scoring container passed through the Joker pipeline.
// Invariant: baseChips and baseMult are set once from ScoringRule and never changed.
// Modifiers only touch chips and mult.
struct ScoreContext {
    int baseChips   = 0;  // invariant - set from hand type
    int baseMult    = 0;  // invariant - set from hand type
    int chips       = 0;  // mutable - starts equal to baseChips + card chips
    int mult        = 0;  // mutable - starts equal to baseMult
    HandRank handRank = HandRank::HIGH_CARD;
};
