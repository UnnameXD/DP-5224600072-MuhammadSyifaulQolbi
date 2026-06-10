#include <iostream>
#include <algorithm>
#include <random>
#include "HandGenerator.h"
#include "Hand.h"

static std::mt19937 rng(std::random_device{}());

HandGenerator::HandGenerator() { initializeDeck(); }

void HandGenerator::initializeDeck() {
    deck.clear();
    char suits[] = {'H', 'D', 'C', 'S'};
    for (char suit : suits)
        for (int rank = 2; rank <= 14; rank++)
            deck.push_back(Card(rank, suit));
}

void HandGenerator::shuffleDeck() {
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
    for (int i = 0; i < 8 && !deck.empty(); i++)
        hand.cards.push_back(drawCard());
    return hand;
}
