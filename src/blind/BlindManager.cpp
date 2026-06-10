#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "../../header/blind/BlindManager.h"

// Unified tag pool (6 options) — both Small and Big Blind draw from same pool
//   0 = FoilTag        : next joker FREE + Foil
//   1 = HolographicTag : next joker FREE + Holo
//   2 = BuffoonTag     : free Mega Buffoon Pack now
//   3 = NegativeTag    : free Joker with Negative edition
//   4 = DoubleTag      : double money (cap $40)
//   5 = BountyTag      : +$25 after boss blind
std::unique_ptr<SkipRewardCommand> BlindManager::buildSmallTag(int type) {
    return buildTag(type);
}

std::unique_ptr<SkipRewardCommand> BlindManager::buildBigTag(int type) {
    return buildTag(type);
}

std::unique_ptr<SkipRewardCommand> BlindManager::buildTag(int type) {
    switch (type % 6) {
        case 0: return std::make_unique<FoilTag>();
        case 1: return std::make_unique<HolographicTag>();
        case 2: return std::make_unique<BuffoonTag>();
        case 3: return std::make_unique<NegativeTag>();
        case 4: return std::make_unique<DoubleTag>();
        case 5: return std::make_unique<BountyTag>();
        default:return std::make_unique<FoilTag>();
    }
}

// Build a boss blind from a type index 0-7
std::unique_ptr<BlindState> BlindManager::makeBossFromType(int type) {
    switch (type % 8) {
        case 0: return std::make_unique<TheHook>();
        case 1: return std::make_unique<TheOx>();
        case 2: return std::make_unique<TheHouse>();
        case 3: return std::make_unique<TheWall>();
        case 4: return std::make_unique<TheWheel>();
        case 5: return std::make_unique<TheArm>();
        case 6: return std::make_unique<TheClub>();
        case 7: return std::make_unique<TheTooth>();
        default:return std::make_unique<TheHook>();
    }
}

// Get boss type for current ante from the shuffled run order
int BlindManager::getBossTypeForAnte(const RunSessionState& session) const {
    const auto& order = session.persistent.bossBlindOrder;
    if (order.empty()) return 0;
    int idx = (session.persistent.ante - 1) % (int)order.size();
    return order[idx];
}

BlindManager::BlindManager() {
    phase = BlindPhase::SMALL;
    blindState = std::make_unique<SmallBlind>();
    // Default tags until rollAnteTags is called
    smallTag = buildSmallTag(0);
    bigTag   = buildBigTag(0);
}

// Called once at the start of each ante to set fixed tags for both skippable blinds
void BlindManager::rollAnteTags(RunSessionState& session) {
    int sType = std::rand() % 6;
    int bType = std::rand() % 6;
    session.persistent.currentAnteTag = { sType, bType };
    session.persistent.antePrepared = true;
    smallTag = buildSmallTag(sType);
    bigTag   = buildBigTag(bType);
    std::cout << "\n[Ante " << session.persistent.ante << " Tags rolled]\n";
    std::cout << "  Small Blind skip tag: " << smallTag->tagName() << "\n";
    std::cout << "  Big Blind skip tag:   " << bigTag->tagName()   << "\n";
}

void BlindManager::loadBlindForPhase(int ante, int bossType) {
    int bossTypeForAnte = bossType;
    switch (phase) {
        case BlindPhase::SMALL: blindState = std::make_unique<SmallBlind>(); break;
        case BlindPhase::BIG:   blindState = std::make_unique<BigBlind>();   break;
        case BlindPhase::BOSS:
            blindState = makeBossFromType(bossTypeForAnte);
            break;
    }
    firstHandPlayed = false;
}

void BlindManager::loadBlindForPhase_public(RunSessionState& session) {
    int bossType = getBossTypeForAnte(session);
    loadBlindForPhase(session.persistent.ante, bossType);
    session.runtime.scoreTarget = blindState->getBaseTarget(session.persistent.ante);
}

void BlindManager::enterBlind(RunSessionState& session) {
    session.runtime = BlindRuntimeState();
    if (session.persistent.hasExtraDiscardVoucher)
        session.runtime.remainingDiscards += 1;
    session.runtime.scoreTarget = blindState->getBaseTarget(session.persistent.ante);
    firstHandPlayed = false;
    blindState->onEnter(session);
    printBlindInfo(session);
}

// Use the pre-rolled cached tag for this phase
void BlindManager::skipBlind(RunSessionState& session) {
    std::cout << "\nSkipping " << blindState->getName() << "...\n";

    SkipRewardCommand* reward = (phase == BlindPhase::SMALL) ? smallTag.get() : bigTag.get();
    std::cout << "[" << reward->tagName() << "] " << reward->describe() << "\n";

    if (reward->shouldTriggerOn(TriggerTiming::Immediate)) {
        reward->execute(session);
    } else {
        // Clone into persistent commands: rebuild from stored type
        int type = (phase == BlindPhase::SMALL)
            ? session.persistent.currentAnteTag.smallTagType
            : session.persistent.currentAnteTag.bigTagType;
        auto clone = (phase == BlindPhase::SMALL) ? buildSmallTag(type) : buildBigTag(type);
        session.persistent.pendingCommands.push_back(std::move(clone));
    }

    advancePhase(session);
    // Fire any OnShopEnter commands immediately (e.g. FoilTag sets freeNextJoker)
    triggerPendingCommands(session, TriggerTiming::OnShopEnter);
}

std::string BlindManager::currentTagName() const {
    if (phase == BlindPhase::SMALL) return smallTag->tagName();
    if (phase == BlindPhase::BIG)   return bigTag->tagName();
    return "";
}

std::string BlindManager::currentTagDescribe() const {
    if (phase == BlindPhase::SMALL) return smallTag->describe();
    if (phase == BlindPhase::BIG)   return bigTag->describe();
    return "";
}

void BlindManager::winBlind(RunSessionState& session) {
    std::cout << "\n[WIN] " << blindState->getName() << " cleared!\n";
    blindState->onWin(session);
    int reward = 3 + session.persistent.ante;
    session.persistent.money += reward;
    std::cout << "Earned $" << reward << " for winning. Money: $"
              << session.persistent.money << "\n";
    // Fire BountyTag if boss blind was just beaten
    if (phase == BlindPhase::BOSS) {
        triggerPendingCommands(session, TriggerTiming::OnBossWin);
    }
    advancePhase(session);
    triggerPendingCommands(session, TriggerTiming::OnShopEnter);
}

void BlindManager::triggerPendingCommands(RunSessionState& session, TriggerTiming timing) {
    auto& cmds = session.persistent.pendingCommands;
    for (auto it = cmds.begin(); it != cmds.end(); ) {
        if ((*it)->shouldTriggerOn(timing)) {
            (*it)->execute(session);
            it = cmds.erase(it);
        } else { ++it; }
    }
}

void BlindManager::advancePhase(RunSessionState& session) {
    if (phase == BlindPhase::SMALL) {
        phase = BlindPhase::BIG;
    } else if (phase == BlindPhase::BIG) {
        phase = BlindPhase::BOSS;
    } else {
        phase = BlindPhase::SMALL;
        session.persistent.ante++;
        std::cout << "\n=== Ante " << session.persistent.ante << " begins! ===\n";
        // Roll new tags for the new ante
        rollAnteTags(session);
    }
    loadBlindForPhase(session.persistent.ante, getBossTypeForAnte(session));
}

void BlindManager::applyBossChallengePostPlay(RunSessionState& session, const Hand& playedHand) {
    if (phase != BlindPhase::BOSS) return;
    if (dynamic_cast<TheOx*>(blindState.get())) {
        std::cout << "[The Ox] Your money is set to $0!\n";
        session.persistent.money = 0;
    }
    if (dynamic_cast<TheTooth*>(blindState.get())) {
        int cost = (int)playedHand.cards.size();
        session.persistent.money = std::max(0, session.persistent.money - cost);
        std::cout << "[The Tooth] Lost $" << cost << " ($1 per card). Money: $"
                  << session.persistent.money << "\n";
    }
    firstHandPlayed = true;
}

void BlindManager::applyHookDiscard(Hand& hand) const {
    if ((int)hand.cards.size() < 2) return;
    int n = (int)hand.cards.size();
    int i1 = std::rand() % n;
    std::swap(hand.cards[i1], hand.cards[n - 1]);
    hand.cards.pop_back(); n--;
    int i2 = std::rand() % n;
    std::swap(hand.cards[i2], hand.cards[n - 1]);
    hand.cards.pop_back();
    std::cout << "[The Hook] 2 random cards discarded from your hand!\n";
}

bool BlindManager::isClubDebuffActive() const {
    return phase == BlindPhase::BOSS && dynamic_cast<TheClub*>(blindState.get()) != nullptr;
}
bool BlindManager::isWheelActive() const {
    return phase == BlindPhase::BOSS && dynamic_cast<TheWheel*>(blindState.get()) != nullptr;
}
bool BlindManager::isHouseActive() const {
    return phase == BlindPhase::BOSS && dynamic_cast<TheHouse*>(blindState.get()) != nullptr;
}

void BlindManager::printBlindInfo(const RunSessionState& session) const {
    std::string bossTag = (phase == BlindPhase::BOSS) ? " [BOSS]" : "";
    std::cout << "\n--- " << blindState->getName() << bossTag << " ---\n";
    std::cout << "  Effect: " << blindState->getDescription() << "\n";
    std::cout << "  Score Target: " << session.runtime.scoreTarget << "\n";
    std::cout << "  Hands: " << session.runtime.remainingHands
              << "  Discards: " << session.runtime.remainingDiscards << "\n";
    std::cout << "  Money: $" << session.persistent.money
              << "  Ante: " << session.persistent.ante << "\n";
}
