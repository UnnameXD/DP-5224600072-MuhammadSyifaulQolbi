#pragma once
#include "Joker.h"
#include "../PokerHandChecker.h"

// -------------------------------------------------------
// Joker: Base Joker - +4 Mult unconditionally
// -------------------------------------------------------
class BaseJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        std::cout << "[Joker] +4 Mult\n";
        ctx.mult += 4;
    }
    std::string getName()        const override { return "Joker"; }
    std::string getDescription() const override { return "+4 Mult"; }
    int getSellValue()           const override { return 2; }
};

// -------------------------------------------------------
// Joker: Jolly Joker - +8 Mult if played hand contains a Pair
// -------------------------------------------------------
class JollyJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        if (ctx.handRank == HandRank::PAIR ||
            ctx.handRank == HandRank::TWO_PAIR ||
            ctx.handRank == HandRank::FULL_HOUSE) {
            std::cout << "[Jolly Joker] +8 Mult (pair present)\n";
            ctx.mult += 8;
        }
    }
    std::string getName()        const override { return "Jolly Joker"; }
    std::string getDescription() const override { return "+8 Mult if hand contains a Pair"; }
    int getSellValue()           const override { return 2; }
};

// -------------------------------------------------------
// Joker: Zany Joker - +12 Mult if played hand contains Three of a Kind
// -------------------------------------------------------
class ZanyJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        if (ctx.handRank == HandRank::THREE_OF_A_KIND ||
            ctx.handRank == HandRank::FULL_HOUSE) {
            std::cout << "[Zany Joker] +12 Mult (three of a kind present)\n";
            ctx.mult += 12;
        }
    }
    std::string getName()        const override { return "Zany Joker"; }
    std::string getDescription() const override { return "+12 Mult if hand contains Three of a Kind"; }
    int getSellValue()           const override { return 2; }
};

// -------------------------------------------------------
// Joker: Greedy Joker - +3 Mult for each Diamond card played
// -------------------------------------------------------
class GreedyJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        int diamonds = 0;
        for (const auto& c : hand.cards)
            if (c.suit == 'D') diamonds++;
        if (diamonds > 0) {
            std::cout << "[Greedy Joker] +" << (diamonds * 3) << " Mult (" << diamonds << " Diamonds)\n";
            ctx.mult += diamonds * 3;
        }
    }
    std::string getName()        const override { return "Greedy Joker"; }
    std::string getDescription() const override { return "+3 Mult per Diamond card played"; }
    int getSellValue()           const override { return 2; }
};

// -------------------------------------------------------
// Joker: Lusty Joker - +3 Mult for each Heart card played
// -------------------------------------------------------
class LustyJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        int hearts = 0;
        for (const auto& c : hand.cards)
            if (c.suit == 'H') hearts++;
        if (hearts > 0) {
            std::cout << "[Lusty Joker] +" << (hearts * 3) << " Mult (" << hearts << " Hearts)\n";
            ctx.mult += hearts * 3;
        }
    }
    std::string getName()        const override { return "Lusty Joker"; }
    std::string getDescription() const override { return "+3 Mult per Heart card played"; }
    int getSellValue()           const override { return 2; }
};

// -------------------------------------------------------
// Joker: Wrathful Joker - +3 Mult for each Spade card played
// -------------------------------------------------------
class WrathfulJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        int spades = 0;
        for (const auto& c : hand.cards)
            if (c.suit == 'S') spades++;
        if (spades > 0) {
            std::cout << "[Wrathful Joker] +" << (spades * 3) << " Mult (" << spades << " Spades)\n";
            ctx.mult += spades * 3;
        }
    }
    std::string getName()        const override { return "Wrathful Joker"; }
    std::string getDescription() const override { return "+3 Mult per Spade card played"; }
    int getSellValue()           const override { return 2; }
};

// -------------------------------------------------------
// Joker: Gluttonous Joker - +3 Mult for each Club card played
// -------------------------------------------------------
class GluttonousJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        int clubs = 0;
        for (const auto& c : hand.cards)
            if (c.suit == 'C') clubs++;
        if (clubs > 0) {
            std::cout << "[Gluttonous Joker] +" << (clubs * 3) << " Mult (" << clubs << " Clubs)\n";
            ctx.mult += clubs * 3;
        }
    }
    std::string getName()        const override { return "Gluttonous Joker"; }
    std::string getDescription() const override { return "+3 Mult per Club card played"; }
    int getSellValue()           const override { return 2; }
};

// -------------------------------------------------------
// Joker: Half Joker - +20 Mult if played hand has 3 or fewer cards
// -------------------------------------------------------
class HalfJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        if ((int)hand.cards.size() <= 3) {
            std::cout << "[Half Joker] +20 Mult (3 or fewer cards played)\n";
            ctx.mult += 20;
        }
    }
    std::string getName()        const override { return "Half Joker"; }
    std::string getDescription() const override { return "+20 Mult if you play 3 or fewer cards"; }
    int getSellValue()           const override { return 3; }
};

// -------------------------------------------------------
// Joker: Fibonacci - +8 Mult per card with a Fibonacci rank (2,3,5,8,A)
// -------------------------------------------------------
class FibonacciJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        for (const auto& c : hand.cards) {
            if (c.rank==2 || c.rank==3 || c.rank==5 || c.rank==8 || c.rank==14) {
                std::cout << "[Fibonacci] +8 Mult (" << c.toString() << " is Fibonacci rank)\n";
                ctx.mult += 8;
            }
        }
    }
    std::string getName()        const override { return "Fibonacci"; }
    std::string getDescription() const override { return "+8 Mult per played card with rank 2, 3, 5, 8, or A"; }
    int getSellValue()           const override { return 4; }
};

// -------------------------------------------------------
// Joker: Scary Face - +30 Chips per face card played (J, Q, K)
// -------------------------------------------------------
class ScaryFaceJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        int faces = 0;
        for (const auto& c : hand.cards)
            if (c.rank >= 11 && c.rank <= 13) faces++;
        if (faces > 0) {
            std::cout << "[Scary Face] +" << (faces * 30) << " Chips (" << faces << " face cards)\n";
            ctx.chips += faces * 30;
        }
    }
    std::string getName()        const override { return "Scary Face"; }
    std::string getDescription() const override { return "+30 Chips per face card (J, Q, K) played"; }
    int getSellValue()           const override { return 3; }
};

// -------------------------------------------------------
// Joker: Bloodstone - x2 Mult if played hand contains any Heart card
// -------------------------------------------------------
class BloodstoneJoker : public Joker {
public:
    void applyMultiplicative(ScoreContext& ctx, const Hand& hand) override {
        for (const auto& c : hand.cards) {
            if (c.suit == 'H') {
                std::cout << "[Bloodstone] x2 Mult (Heart card present)\n";
                ctx.mult *= 2;
                return;
            }
        }
    }
    std::string getName()        const override { return "Bloodstone"; }
    std::string getDescription() const override { return "x2 Mult if played hand contains a Heart"; }
    int getSellValue()           const override { return 4; }
};

// -------------------------------------------------------
// Joker: Blueprint - +10 Chips and +5 Mult every hand
// -------------------------------------------------------
class BlueprintJoker : public Joker {
public:
    void applyAdditive(ScoreContext& ctx, const Hand& hand) override {
        std::cout << "[Blueprint] +10 Chips +5 Mult\n";
        ctx.chips += 10;
        ctx.mult  += 5;
    }
    std::string getName()        const override { return "Blueprint"; }
    std::string getDescription() const override { return "+10 Chips and +5 Mult every hand"; }
    int getSellValue()           const override { return 5; }
};
