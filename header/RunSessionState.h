#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include "joker/JokerManager.h"
#include "blind/SkipRewardCommand.h"

class BlindState;

// Per-ante tag assignments: rolled once when ante begins, shown all run.
// One tag for Small Blind skip, one for Big Blind skip.
struct AnteTagAssignment {
    int  smallTagType = 0;  // index into small-blind pool
    int  bigTagType   = 0;  // index into big-blind pool
};

// Persistent state - survives across blinds and shops
struct RunPersistentState {
    int ante  = 1;
    int money = 4;
    JokerManager jokerManager;
    std::vector<std::unique_ptr<SkipRewardCommand>> pendingCommands;

    bool hasHandSizeVoucher     = false;
    bool hasExtraDiscardVoucher = false;

    // Tag rolled at the start of each ante
    AnteTagAssignment currentAnteTag;
    bool antePrepared = false;  // false until first rollAnteTags() call

    // Boss blind order rolled once at run start: indices 0-7 map ante 1-8 to boss type 0-7
    // 0=TheHook 1=TheOx 2=TheHouse 3=TheWall 4=TheWheel 5=TheArm 6=TheClub 7=TheTooth
    std::vector<int> bossBlindOrder;  // size 8, shuffled at run start
};

// Runtime state - resets every blind
struct BlindRuntimeState {
    int blindScore        = 0;
    int remainingHands    = 4;
    int remainingDiscards = 3;
    int scoreTarget       = 0;
};

// Combined session state
struct RunSessionState {
    RunPersistentState persistent;
    BlindRuntimeState  runtime;

    // Set by Foil/Holo/Rare tag: next joker in shop is free with a specific edition
    bool         freeNextJoker        = false;
    JokerEdition freeNextJokerEdition = JokerEdition::NONE;
    bool         freeNextJokerRare    = false; // RareTag: offer a rare joker
};
