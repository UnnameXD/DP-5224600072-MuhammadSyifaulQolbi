#pragma once
#include "Hand.h"

class HandPlayer {
private:
    Hand chosenHand; // store selected cards

public:
    void playHand(Hand& hand); // DO NOT CHANGE

    Hand getChosenHand() const; // getter
};