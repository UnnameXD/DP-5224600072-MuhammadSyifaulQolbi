#include <iostream>
#include <algorithm>
#include "blind/SkipRewardCommand.h"
#include "RunSessionState.h"
#include "joker/ConcreteJokers.h"

static std::unique_ptr<Joker> makeJokerByIndex(int i) {
    switch (i % 12) {
        case 0:  return std::make_unique<BaseJoker>();
        case 1:  return std::make_unique<JollyJoker>();
        case 2:  return std::make_unique<ZanyJoker>();
        case 3:  return std::make_unique<GreedyJoker>();
        case 4:  return std::make_unique<LustyJoker>();
        case 5:  return std::make_unique<WrathfulJoker>();
        case 6:  return std::make_unique<GluttonousJoker>();
        case 7:  return std::make_unique<HalfJoker>();
        case 8:  return std::make_unique<FibonacciJoker>();
        case 9:  return std::make_unique<ScaryFaceJoker>();
        case 10: return std::make_unique<BloodstoneJoker>();
        case 11: return std::make_unique<BlueprintJoker>();
        default: return std::make_unique<BaseJoker>();
    }
}

static int pickJokerFromPack(const std::string& packName, int poolSize = 12) {
    int idxA = std::rand() % poolSize;
    int idxB; do { idxB = std::rand() % poolSize; } while (idxB == idxA);

    auto jA = makeJokerByIndex(idxA);
    auto jB = makeJokerByIndex(idxB);

    std::cout << "\n[" << packName << "]\n";
    std::cout << "  [0] " << jA->getDisplayName() << " -- " << jA->getDescription() << "\n";
    std::cout << "  [1] " << jB->getDisplayName() << " -- " << jB->getDescription() << "\n";
    std::cout << "  [-1] Skip\n";

    int pick = -99;
    while (pick != 0 && pick != 1 && pick != -1) {
        std::cout << "Choice (0, 1, or -1 to skip): ";
        std::string line;
        std::getline(std::cin >> std::ws, line);
        try { pick = std::stoi(line); }
        catch (...) { pick = -99; }
        if (pick != 0 && pick != 1 && pick != -1)
            std::cout << "Invalid. Enter 0, 1, or -1.\n";
    }
    return pick;  // -1 means skipped, 0=jA index, 1=jB index (caller rebuilds)
}

// -------------------------------------------------------
// Double Tag: double money, cap at $40
void DoubleTag::execute(RunSessionState& session) {
    int before = session.persistent.money;
    int doubled = std::min(before * 2, 40);
    session.persistent.money = doubled;
    std::cout << "[Double Tag] $" << before << " x2 = $" << (before * 2)
              << (before * 2 > 40 ? " (capped at $40)" : "")
              << " -> Money: $" << session.persistent.money << "\n";
}

// -------------------------------------------------------
// Foil Tag: next joker in shop is FREE with Foil edition
void FoilTag::execute(RunSessionState& session) {
    session.freeNextJoker        = true;
    session.freeNextJokerEdition = JokerEdition::FOIL;
    session.freeNextJokerRare    = false;
    std::cout << "[Foil Tag] Next Joker in shop is FREE with Foil (+50 Chips)!\n";
}

// -------------------------------------------------------
// Holographic Tag: next joker in shop is FREE with Holographic edition
void HolographicTag::execute(RunSessionState& session) {
    session.freeNextJoker        = true;
    session.freeNextJokerEdition = JokerEdition::HOLOGRAPHIC;
    session.freeNextJokerRare    = false;
    std::cout << "[Holographic Tag] Next Joker in shop is FREE with Holo (+10 Mult)!\n";
}

// -------------------------------------------------------
// Buffoon Tag: open Mega Buffoon Pack immediately (2 random jokers, pick 1)
void BuffoonTag::execute(RunSessionState& session) {
    int idxA = std::rand() % 12;
    int idxB; do { idxB = std::rand() % 12; } while (idxB == idxA);

    auto jA = makeJokerByIndex(idxA);
    auto jB = makeJokerByIndex(idxB);

    std::cout << "\n[Buffoon Tag] Mega Buffoon Pack opened!\n";
    std::cout << "  [0] " << jA->getDisplayName() << " -- " << jA->getDescription() << "\n";
    std::cout << "  [1] " << jB->getDisplayName() << " -- " << jB->getDescription() << "\n";
    std::cout << "  [-1] Skip\n";

    int pick = -99;
    while (pick != 0 && pick != 1 && pick != -1) {
        std::cout << "Choice (0, 1, or -1 to skip): ";
        std::string line;
        std::getline(std::cin >> std::ws, line);
        try { pick = std::stoi(line); }
        catch (...) { pick = -99; }
        if (pick != 0 && pick != 1 && pick != -1)
            std::cout << "Invalid. Enter 0, 1, or -1.\n";
    }
    if (pick == 0) session.persistent.jokerManager.addJoker(std::move(jA));
    else if (pick == 1) session.persistent.jokerManager.addJoker(std::move(jB));
    else std::cout << "Pack skipped.\n";
}

// -------------------------------------------------------
// Negative Tag: open a pack, chosen joker receives Negative edition (+1 joker slot)
void NegativeTag::execute(RunSessionState& session) {
    session.freeNextJoker        = true;
    session.freeNextJokerEdition = JokerEdition::NEGATIVE;
    session.freeNextJokerRare    = false;
    std::cout << "[Negative Tag] Next Joker in shop is FREE with Negative edition (+1 Joker slot)!\n";
}

// -------------------------------------------------------
// Bounty Tag: +$25 after the boss blind of this ante is defeated
void BountyTag::execute(RunSessionState& session) {
    session.persistent.money += 25;
    std::cout << "[Bounty Tag] Boss blind defeated! +$25. Money: $"
              << session.persistent.money << "\n";
}
