#pragma once
#include <string>

struct RunSessionState;

enum class TriggerTiming { Immediate, OnShopEnter, OnBossWin };

// Command Pattern: reward granted for skipping a blind
class SkipRewardCommand {
public:
    virtual ~SkipRewardCommand() = default;
    virtual void execute(RunSessionState& session) = 0;
    virtual bool shouldTriggerOn(TriggerTiming timing) const = 0;
    virtual std::string describe() const = 0;
    virtual std::string tagName()   const = 0;
};

// -------------------------------------------------------
// Double Tag: doubles player money, capped at $40
class DoubleTag : public SkipRewardCommand {
public:
    void execute(RunSessionState& session) override;
    bool shouldTriggerOn(TriggerTiming t) const override {
        return t == TriggerTiming::Immediate;
    }
    std::string describe() const override { return "Double your money (max $40)"; }
    std::string tagName()   const override { return "Double Tag"; }
};

// -------------------------------------------------------
// Foil Tag: next Joker in shop is FREE with Foil edition (+50 Chips)
class FoilTag : public SkipRewardCommand {
public:
    void execute(RunSessionState& session) override;
    bool shouldTriggerOn(TriggerTiming t) const override {
        return t == TriggerTiming::OnShopEnter;
    }
    std::string describe() const override { return "Next Joker in shop is FREE with Foil (+50 Chips)"; }
    std::string tagName()   const override { return "Foil Tag"; }
};

// -------------------------------------------------------
// Holographic Tag: next Joker in shop is FREE with Holographic edition (+10 Mult)
class HolographicTag : public SkipRewardCommand {
public:
    void execute(RunSessionState& session) override;
    bool shouldTriggerOn(TriggerTiming t) const override {
        return t == TriggerTiming::OnShopEnter;
    }
    std::string describe() const override { return "Next Joker in shop is FREE with Holo (+10 Mult)"; }
    std::string tagName()   const override { return "Holographic Tag"; }
};

// -------------------------------------------------------
// Buffoon Tag: immediately opens a free Mega Buffoon Pack (pick 1 of 2 Jokers)
class BuffoonTag : public SkipRewardCommand {
public:
    void execute(RunSessionState& session) override;
    bool shouldTriggerOn(TriggerTiming t) const override {
        return t == TriggerTiming::Immediate;
    }
    std::string describe() const override { return "Free Mega Buffoon Pack — choose 1 of 2 Jokers now"; }
    std::string tagName()   const override { return "Buffoon Tag"; }
};

// -------------------------------------------------------
// Negative Tag: immediately opens a free pack; chosen Joker gets Negative edition (+1 slot)
class NegativeTag : public SkipRewardCommand {
public:
    void execute(RunSessionState& session) override;
    bool shouldTriggerOn(TriggerTiming t) const override {
        return t == TriggerTiming::Immediate;
    }
    std::string describe() const override { return "Free Joker with Negative edition (+1 Joker slot)"; }
    std::string tagName()   const override { return "Negative Tag"; }
};

// -------------------------------------------------------
// Bounty Tag: grants +$25 after defeating the Boss Blind of this ante
class BountyTag : public SkipRewardCommand {
public:
    void execute(RunSessionState& session) override;
    bool shouldTriggerOn(TriggerTiming t) const override {
        return t == TriggerTiming::OnBossWin;
    }
    std::string describe() const override { return "+$25 after defeating this ante's Boss Blind"; }
    std::string tagName()   const override { return "Bounty Tag"; }
};
