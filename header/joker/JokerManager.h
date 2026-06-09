#pragma once
#include <vector>
#include <memory>
#include "Joker.h"
#include "../ScoreContext.h"
#include "../Hand.h"

// Manages equipped Jokers and runs the two-pass modification pipeline.
class JokerManager {
public:
    static constexpr int MAX_JOKERS = 5;

    bool addJoker(std::unique_ptr<Joker> joker);
    bool removeJoker(int index);
    int  jokerCount() const;
    void printJokers() const;
    const std::vector<std::unique_ptr<Joker>>& getJokers() const;

    // Two-pass pipeline: additive then multiplicative
    void applyJokers(ScoreContext& context, const Hand& hand);

private:
    std::vector<std::unique_ptr<Joker>> jokers;
};
