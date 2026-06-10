#pragma once
#include <memory>
#include "../RunSessionState.h"
#include "BlindState.h"
#include "ConcreteBlindStates.h"
#include "SkipRewardCommand.h"

enum class BlindPhase { SMALL, BIG, BOSS };

class BlindManager {
public:
    BlindManager();

    // Roll and store one tag for Small Blind and one for Big Blind for this ante.
    // Call once when ante begins (first blind of ante, or new run start).
    void rollAnteTags(RunSessionState& session);

    // Load the blind state for current phase/ante (call before display)
    void loadBlindForPhase_public(RunSessionState& session);

    // Enter blind: reset runtime, set target
    void enterBlind(RunSessionState& session);

    // Skip: execute the pre-rolled tag reward, advance phase
    void skipBlind(RunSessionState& session);

    // Win: money reward, advance phase, trigger OnShopEnter commands
    void winBlind(RunSessionState& session);

    void triggerPendingCommands(RunSessionState& session, TriggerTiming timing);

    void applyBossChallengePostPlay(RunSessionState& session, const Hand& playedHand);
    void applyHookDiscard(Hand& hand) const;

    bool isClubDebuffActive() const;
    bool isWheelActive() const;
    bool isHouseActive() const;

    BlindState* currentBlind() const { return blindState.get(); }
    BlindPhase  currentPhase() const { return phase; }
    bool        isBossPhase()  const { return phase == BlindPhase::BOSS; }

    // Returns the tag name/description for the current phase (for skip preview)
    std::string currentTagName()    const;
    std::string currentTagDescribe() const;

    void printBlindInfo(const RunSessionState& session) const;

private:
    BlindPhase phase = BlindPhase::SMALL;
    std::unique_ptr<BlindState> blindState;
    bool firstHandPlayed = false;

    // Cached tag instances for current ante — rebuilt each rollAnteTags()
    std::unique_ptr<SkipRewardCommand> smallTag;
    std::unique_ptr<SkipRewardCommand> bigTag;

    void loadBlindForPhase(int ante, int bossType = 0);
    void advancePhase(RunSessionState& session);

    // Build a tag object from a pool index
    static std::unique_ptr<SkipRewardCommand> buildSmallTag(int type);
    static std::unique_ptr<SkipRewardCommand> buildBigTag(int type);
    static std::unique_ptr<SkipRewardCommand> buildTag(int type);  // unified pool
    static std::unique_ptr<BlindState> makeBossFromType(int type);
    int getBossTypeForAnte(const RunSessionState& session) const;
};
