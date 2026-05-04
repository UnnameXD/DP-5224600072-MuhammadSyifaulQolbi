#include <iostream>
#include <algorithm>
#include <random>
#include "../header/HandGenerator.h"
#include "../header/Hand.h"

HandGenerator::HandGenerator() {
    initializeDeck();
}

void HandGenerator::initializeDeck() {
    deck.clear();

    char suits[] = {'H', 'D', 'C', 'S'};

    for (char suit : suits) {
        for (int rank = 2; rank <= 14; rank++) {
            deck.push_back(Card(rank, suit));
        }
    }
}

void HandGenerator::shuffleDeck() {
    static std::mt19937 rng(std::random_device{}()); // seeded once
    std::shuffle(deck.begin(), deck.end(), rng);
}

Card HandGenerator::drawCard() {
    Card c = deck.back();
    deck.pop_back();
    return c;
}

Hand HandGenerator::generateHand() {
    shuffleDeck();

    Hand hand;

    for (int i = 0; i < 8; i++) { // Balatro hand size
        hand.cards.push_back(drawCard());
    }

    return hand;
}
