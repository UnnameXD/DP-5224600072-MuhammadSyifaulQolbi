#pragma once
#include <string>

// Forward declare
struct RunSessionState;

// State Pattern: each blind type is a concrete state
class BlindState {
public:
    virtual ~BlindState() = default;

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual double getScoreMultiplier() const = 0;
    virtual int getBaseTarget(int ante) const = 0;

    // Called on entry
    virtual void onEnter(RunSessionState& session) {}
    // Called on win
    virtual void onWin(RunSessionState& session) {}
};
