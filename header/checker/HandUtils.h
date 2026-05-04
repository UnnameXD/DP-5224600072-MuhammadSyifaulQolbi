#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include "../Hand.h"

// Returns a map of rank -> count for all cards in the hand
inline std::map<int, int> getRankCounts(const Hand& hand) {
    std::map<int, int> counts;
    for (const auto& c : hand.cards) counts[c.rank]++;
    return counts;
}

// Returns true if all cards share the same suit
inline bool allSameSuit(const Hand& hand) {
    if (hand.cards.empty()) return false;
    char s = hand.cards[0].suit;
    for (const auto& c : hand.cards)
        if (c.suit != s) return false;
    return true;
}

// Returns true if the 5-card hand forms a straight (consecutive ranks)
// Also handles Ace-low straight: A-2-3-4-5
inline bool isConsecutiveRanks(const Hand& hand) {
    if (hand.cards.size() != 5) return false;

    std::vector<int> ranks;
    for (const auto& c : hand.cards) ranks.push_back(c.rank);
    std::sort(ranks.begin(), ranks.end());

    // No duplicates allowed in a straight
    for (int i = 1; i < (int)ranks.size(); i++)
        if (ranks[i] == ranks[i - 1]) return false;

    // Normal straight: span of exactly 4
    if (ranks[4] - ranks[0] == 4) return true;

    // Ace-low straight: A-2-3-4-5 stored as {2,3,4,5,14}
    if (ranks[0] == 2 && ranks[3] == 5 && ranks[4] == 14) return true;

    return false;
}
