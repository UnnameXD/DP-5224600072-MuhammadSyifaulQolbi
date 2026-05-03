#pragma once
#include <vector>
#include "Hand.h"
#include "Card.h"

class HandGenerator {
private:
    std::vector<Card> deck;

public:
    HandGenerator();

    void initializeDeck();
    void shuffleDeck();
    Card drawCard();

    Hand generateHand(); // always 8 cards
};