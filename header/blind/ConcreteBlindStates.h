#pragma once
#include "BlindState.h"
#include <iostream>
#include <algorithm>
#include <random>

// Balatro score targets per ante
static int getAnteBase(int ante) {
    switch (ante) {
        case 1: return 300;
        case 2: return 800;
        case 3: return 2000;
        case 4: return 5000;
        case 5: return 11000;
        case 6: return 20000;
        case 7: return 35000;
        case 8: return 50000;
        default: return 300 * ante;
    }
}

// -------------------------------------------------------
class SmallBlind : public BlindState {
public:
    std::string getName()        const override { return "Small Blind"; }
    std::string getDescription() const override { return "No effect."; }
    double getScoreMultiplier()  const override { return 1.0; }
    int getBaseTarget(int ante)  const override { return getAnteBase(ante); }
};

class BigBlind : public BlindState {
public:
    std::string getName()        const override { return "Big Blind"; }
    std::string getDescription() const override { return "No effect."; }
    double getScoreMultiplier()  const override { return 1.5; }
    int getBaseTarget(int ante)  const override { return (int)(getAnteBase(ante) * 1.5); }
};

// -------------------------------------------------------
// Boss Blind base
class BossBlind : public BlindState {
public:
    double getScoreMultiplier() const override { return 2.0; }
    int getBaseTarget(int ante) const override { return getAnteBase(ante) * 2; }
    virtual bool isDebuffActive() const { return false; }
};

// -------------------------------------------------------
// The Hook (Ante 1): Discards 2 random cards from your hand after each play
class TheHook : public BossBlind {
public:
    std::string getName()        const override { return "The Hook"; }
    std::string getDescription() const override {
        return "Discards 2 random cards from your hand after each play.";
    }
    bool isDebuffActive() const override { return true; }

    // Returns 2 indices to discard from a hand of size n
    std::vector<int> getDiscardIndices(int handSize) const {
        std::vector<int> all;
        for (int i = 0; i < handSize; i++) all.push_back(i);
        std::shuffle(all.begin(), all.end(), std::default_random_engine(std::rand()));
        return {all[0], all[1]};
    }
};

// -------------------------------------------------------
// The Ox (Ante 2): Playing your most played hand sets money to $0
class TheOx : public BossBlind {
public:
    std::string getName()        const override { return "The Ox"; }
    std::string getDescription() const override {
        return "Playing a hand sets your money to $0.";
    }
    bool isDebuffActive() const override { return true; }
};

// -------------------------------------------------------
// The House (Ante 3): First hand is drawn face-down (cards hidden)
class TheHouse : public BossBlind {
public:
    std::string getName()        const override { return "The House"; }
    std::string getDescription() const override {
        return "First hand is drawn face-down (cards hidden).";
    }
    bool isDebuffActive() const override { return true; }
};

// -------------------------------------------------------
// The Wall (Ante 4): Extra-large blind (4x base score)
class TheWall : public BossBlind {
public:
    std::string getName()        const override { return "The Wall"; }
    std::string getDescription() const override {
        return "Extra large blind — score target is doubled.";
    }
    int getBaseTarget(int ante)  const override { return getAnteBase(ante) * 4; }
};

// -------------------------------------------------------
// The Wheel (Ante 5): 1 in 7 cards are drawn face-down each hand
class TheWheel : public BossBlind {
public:
    std::string getName()        const override { return "The Wheel"; }
    std::string getDescription() const override {
        return "1 in 7 cards drawn face-down each hand.";
    }
    bool isDebuffActive() const override { return true; }
};

// -------------------------------------------------------
// The Arm (Ante 6): Decrease level of played poker hand by 1 (not yet leveled here, so -5 chips)
class TheArm : public BossBlind {
public:
    std::string getName()        const override { return "The Arm"; }
    std::string getDescription() const override {
        return "Played poker hand loses 5 base chips after scoring.";
    }
    bool isDebuffActive() const override { return true; }
};

// -------------------------------------------------------
// The Club (Ante 7): All Club cards are debuffed (worth 0 chips)
class TheClub : public BossBlind {
public:
    std::string getName()        const override { return "The Club"; }
    std::string getDescription() const override {
        return "All Club cards are debuffed — worth 0 chips.";
    }
    bool isDebuffActive() const override { return true; }
};

// -------------------------------------------------------
// The Tooth (Ante 8): Lose $1 per card played
class TheTooth : public BossBlind {
public:
    std::string getName()        const override { return "The Tooth"; }
    std::string getDescription() const override {
        return "Lose $1 for each card played.";
    }
    bool isDebuffActive() const override { return true; }
};
