#pragma once
#include <iostream>
#include <string>
#include "../ScoreContext.h"
#include "../Hand.h"

enum class JokerEdition { NONE, FOIL, HOLOGRAPHIC, POLYCHROME, NEGATIVE };

// Observer-pattern Joker interface.
// Modifiers may only mutate context.chips and context.mult.
// They MUST NOT change context.baseChips or context.baseMult (invariants).
class Joker {
public:
    virtual ~Joker() = default;

    JokerEdition edition = JokerEdition::NONE;

    // Called during additive pass
    virtual void applyAdditive(ScoreContext& context, const Hand& hand) {}
    // Called during multiplicative pass
    virtual void applyMultiplicative(ScoreContext& context, const Hand& hand) {}

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getSellValue() const { return 2; }

    // Short display label: "Blueprint [f]", "Joker [h]", etc.
    std::string getDisplayName() const {
        std::string base = getName();
        switch (edition) {
            case JokerEdition::FOIL:        return base + " [f]";
            case JokerEdition::HOLOGRAPHIC: return base + " [h]";
            case JokerEdition::POLYCHROME:  return base + " [p]";
            case JokerEdition::NEGATIVE:    return base + " [n]";
            default:                        return base;
        }
    }

    // Edition bonus applied after concrete joker passes
    void applyEditionAdditive(ScoreContext& ctx) {
        if (edition == JokerEdition::FOIL) {
            std::cout << "  [Foil] +50 Chips\n";
            ctx.chips += 50;
        } else if (edition == JokerEdition::HOLOGRAPHIC) {
            std::cout << "  [Holo] +10 Mult\n";
            ctx.mult += 10;
        }
    }
    void applyEditionMultiplicative(ScoreContext& ctx) {
        if (edition == JokerEdition::POLYCHROME) {
            std::cout << "  [Poly] x1.5 Mult\n";
            ctx.mult = (int)(ctx.mult * 1.5);
        }
    }

    std::string getEditionDescription() const {
        switch (edition) {
            case JokerEdition::FOIL:        return " [f: +50 Chips]";
            case JokerEdition::HOLOGRAPHIC: return " [h: +10 Mult]";
            case JokerEdition::POLYCHROME:  return " [p: x1.5 Mult]";
            case JokerEdition::NEGATIVE:    return " [n: +1 Joker slot]";
            default:                        return "";
        }
    }

    // Apply Negative edition: grants +1 joker slot (tracked in JokerManager)
    bool isNegative() const { return edition == JokerEdition::NEGATIVE; }
};
