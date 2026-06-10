#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <random>
#include "GameManager.h"

GameManager::GameManager() {
    std::srand((unsigned)std::time(nullptr));
}

void GameManager::runSession() {
    std::cout << "\n================================\n";
    std::cout << "|     BALATRO in C++        |\n";
    std::cout << "================================\n";
    std::cout << "Starting run! Ante 1, Money: $" << session.persistent.money << "\n";

    // Roll and shuffle boss blind order once for the whole run
    {
        auto& order = session.persistent.bossBlindOrder;
        order.resize(8);
        std::iota(order.begin(), order.end(), 0);
        std::shuffle(order.begin(), order.end(), std::mt19937(std::random_device{}()));
        const char* bossNames[] = {"Hook","Ox","House","Wall","Wheel","Arm","Club","Tooth"};
        std::cout << "Boss order: ";
        for (int i = 0; i < 8; i++)
            std::cout << "A" << (i+1) << "=" << bossNames[order[i]] << " ";
        std::cout << "\n";
    }
    blindManager.rollAnteTags(session);

    while (session.persistent.ante <= 8) {
        for (int b = 0; b < 3; b++) {
            blindManager.loadBlindForPhase_public(session);
            bool isBoss = blindManager.isBossPhase();

            std::cout << "\n==========================================\n";
            if (isBoss)
                std::cout << "*** BOSS BLIND - cannot be skipped ***\n";
            blindManager.printBlindInfo(session);

            if (!isBoss) {
                std::cout << "\n[s] Skip  (" << blindManager.currentTagName()
                          << ": " << blindManager.currentTagDescribe() << ")\n";
                std::cout << "[p] Play Blind\n";
                char c = 0;
                while (c != 's' && c != 'p') {
                    std::cout << "Choice (s/p): ";
                    std::string line; std::cin >> line;
                    if (!line.empty()) c = line[0];
                    if (c != 's' && c != 'p')
                        std::cout << "Invalid. Enter 's' to skip or 'p' to play.\n";
                }
                if (c == 's') {
                    blindManager.skipBlind(session);
                    runShopPhase();
                    continue;
                }
            } else {
                std::cout << "\n[p] Play Blind (Boss - cannot skip)\n";
                char c = 0;
                while (c != 'p') {
                    std::cout << "Choice (p): ";
                    std::string line; std::cin >> line;
                    if (!line.empty()) c = line[0];
                    if (c != 'p')
                        std::cout << "Invalid. Enter 'p' to play the Boss Blind.\n";
                }
            }

            blindManager.enterBlind(session);
            bool survived = playBlind();
            if (!survived) {
                std::cout << "\n=== GAME OVER ===\n";
                std::cout << "You reached Ante " << session.persistent.ante << "\n";
                return;
            }
            blindManager.winBlind(session);
            runShopPhase();
        }
    }

    std::cout << "\n================================\n";
    std::cout << "|    YOU WIN! Run Complete!    |\n";
    std::cout << "================================\n";
}

static void printHand(const Hand& hand, bool firstHidden = false,
                      bool wheelHidden = false, bool clubDebuff = false) {
    std::cout << "\nYour Hand:\n";
    for (int i = 0; i < (int)hand.cards.size(); i++) {
        bool hidden = firstHidden || (wheelHidden && (i % 7 == 0));
        if (hidden) {
            std::cout << "  [" << i << "] [face-down]\n";
        } else {
            const Card& c = hand.cards[i];
            bool debuffed = clubDebuff && c.suit == 'C';
            std::string suffix = debuffed
                ? " (0 chips [debuffed])"
                : " (+" + std::to_string(c.getChipValue()) + " chips)";
            std::cout << "  [" << i << "] " << c.toString() << suffix << "\n";
        }
    }
}

static void refillHand(Hand& hand, HandGenerator& gen, int targetSize) {
    while ((int)hand.cards.size() < targetSize) {
        Hand fresh = gen.generateHand();
        if (fresh.cards.empty()) break;
        for (const auto& c : fresh.cards) {
            hand.cards.push_back(c);
            if ((int)hand.cards.size() >= targetSize) break;
        }
    }
}

static void sellJoker(RunSessionState& session) {
    auto& jm = session.persistent.jokerManager;
    if (jm.jokerCount() == 0) {
        std::cout << "No jokers to sell.\n";
        return;
    }
    std::cout << "\nSell which joker? (index or [x] to cancel)\n";
    const auto& jokers = jm.getJokers();
    for (int i = 0; i < (int)jokers.size(); i++) {
        std::cout << "  [" << i << "] " << jokers[i]->getDisplayName()
                  << " -- " << jokers[i]->getDescription()
                  << jokers[i]->getEditionDescription()
                  << "  ($" << jokers[i]->getSellValue() << ")\n";
    }
    while (true) {
        std::cout << "Choice: ";
        std::string line; std::getline(std::cin >> std::ws, line);
        if (line == "x" || line == "X") {
            std::cout << "Sell cancelled.\n";
            return;
        }
        int idx = -1;
        try { idx = std::stoi(line); } catch (...) {}
        if (idx < 0 || idx >= jm.jokerCount()) {
            std::cout << "Invalid. Enter a joker index or x to cancel.\n";
            continue;
        }
        int value = jokers[idx]->getSellValue();
        jm.removeJoker(idx);
        session.persistent.money += value;
        std::cout << "Sold for $" << value << ". Money: $"
                  << session.persistent.money << "\n";
        return;
    }
}

bool GameManager::playBlind() {
    bool isBoss  = blindManager.isBossPhase();
    bool isHook  = isBoss && dynamic_cast<TheHook*> (blindManager.currentBlind()) != nullptr;
    bool isOx    = isBoss && dynamic_cast<TheOx*>   (blindManager.currentBlind()) != nullptr;
    bool isHouse = blindManager.isHouseActive();
    bool isWheel = blindManager.isWheelActive();
    bool isClub  = blindManager.isClubDebuffActive();
    bool isArm   = isBoss && dynamic_cast<TheArm*>  (blindManager.currentBlind()) != nullptr;
    bool isTooth = isBoss && dynamic_cast<TheTooth*>(blindManager.currentBlind()) != nullptr;

    handGenerator = HandGenerator();
    int handSize = 8 + (session.persistent.hasHandSizeVoucher ? 1 : 0);
    Hand currentHand = handGenerator.generateHand();
    while ((int)currentHand.cards.size() > handSize) currentHand.cards.pop_back();

    bool firstHand = true;

    while (true) {
        std::cout << "\n==========================================\n";
        std::cout << "Score: " << session.runtime.blindScore
                  << " / " << session.runtime.scoreTarget << "\n";
        std::cout << "Hands: " << session.runtime.remainingHands
                  << "  Discards: " << session.runtime.remainingDiscards
                  << "  Money: $" << session.persistent.money << "\n";

        if (isBoss) {
            std::cout << "[BOSS] " << blindManager.currentBlind()->getName()
                      << ": " << blindManager.currentBlind()->getDescription() << "\n";
        }

        std::cout << "Jokers:\n";
        session.persistent.jokerManager.printJokers();

        bool showFirstHidden = isHouse && firstHand;
        printHand(currentHand, showFirstHidden, isWheel, isClub);

        // Action prompt — only p, d, s accepted
        char action = 0;
        while (action != 'p' && action != 'd') {
            std::cout << "\n[p] Play hand | [d] Discard | [s] Sell Joker\n";
            std::cout << "Choice: ";
            std::string line; std::getline(std::cin >> std::ws, line);
            if (!line.empty()) action = line[0];
            if (action == 's') {
                sellJoker(session);
                std::cout << "Jokers:\n";
                session.persistent.jokerManager.printJokers();
                action = 0;
                continue;
            }
            if (action != 'p' && action != 'd')
                std::cout << "Please enter 'p' to play, 'd' to discard, or 's' to sell.\n";
        }

        if (action == 'd') {
            if (session.runtime.remainingDiscards <= 0) {
                std::cout << "No discards left!\n";
                continue;
            }
            handPlayer.playHand(currentHand);
            Hand chosen = handPlayer.getChosenHand();
            if (chosen.cards.empty()) {
                std::cout << "Discard cancelled.\n";
                continue;
            }
            for (const auto& d : chosen.cards) {
                for (auto it = currentHand.cards.begin(); it != currentHand.cards.end(); ++it) {
                    if (it->rank == d.rank && it->suit == d.suit) {
                        currentHand.cards.erase(it); break;
                    }
                }
            }
            session.runtime.remainingDiscards--;
            refillHand(currentHand, handGenerator, handSize);
            std::cout << "Discarded " << chosen.cards.size() << " card(s). "
                      << "Discards left: " << session.runtime.remainingDiscards << "\n";
            printHand(currentHand, false, isWheel, isClub);
            continue;
        }

        // Play hand
        if (session.runtime.remainingHands <= 0) {
            std::cout << "Out of hands! Blind failed.\n";
            return false;
        }

        handPlayer.playHand(currentHand);
        Hand chosen = handPlayer.getChosenHand();
        if (chosen.cards.empty()) {
            std::cout << "Play cancelled.\n";
            continue;
        }

        // Scoring
        ScoreContext ctx = scoringRule.createContext(chosen);

        if (isClub) {
            for (const auto& c : chosen.cards)
                if (c.suit == 'C') ctx.chips -= c.getChipValue();
            std::cout << "[The Club] Club cards debuffed (0 chips each).\n";
        }

        std::cout << "\nHand: ";
        for (auto& c : chosen.cards) std::cout << c.toString() << " ";
        std::cout << "\n";
        std::cout << "Base: " << ctx.baseChips << " chips x " << ctx.baseMult << " mult\n";
        std::cout << "Cards add: " << (ctx.chips - ctx.baseChips) << " chips\n";

        session.persistent.jokerManager.applyJokers(ctx, chosen);

        if (isArm) {
            ctx.chips = std::max(0, ctx.chips - 5);
            std::cout << "[The Arm] -5 chips.\n";
        }

        int handScore = ctx.chips * ctx.mult;
        std::cout << "Final: " << ctx.chips << " chips x " << ctx.mult
                  << " mult = " << handScore << "\n";

        session.runtime.blindScore += handScore;
        session.runtime.remainingHands--;
        firstHand = false;

        blindManager.applyBossChallengePostPlay(session, chosen);

        std::cout << "Score: " << session.runtime.blindScore
                  << " / " << session.runtime.scoreTarget << "\n";

        if (session.runtime.blindScore >= session.runtime.scoreTarget)
            return true;

        if (session.runtime.remainingHands == 0) {
            std::cout << "Out of hands! Blind failed.\n";
            return false;
        }

        for (const auto& p : chosen.cards) {
            for (auto it = currentHand.cards.begin(); it != currentHand.cards.end(); ++it) {
                if (it->rank == p.rank && it->suit == p.suit) {
                    currentHand.cards.erase(it); break;
                }
            }
        }
        if (isHook) blindManager.applyHookDiscard(currentHand);
        refillHand(currentHand, handGenerator, handSize);
        printHand(currentHand, false, isWheel, isClub);
    }
    return false;
}

void GameManager::runShopPhase() {
    shopManager.openShop(session);
    while (true) {
        std::cout << "\nMoney: $" << session.persistent.money << "\n";
        std::cout << "Buy [slot #], [r]eroll, [s]ell joker, or [x] leave: ";
        std::string input; std::getline(std::cin >> std::ws, input);
        if (input == "x" || input == "X") break;
        if (input == "r") { shopManager.reroll(session); continue; }
        if (input == "s") {
            sellJoker(session);
            shopManager.printShop();
            shopManager.printJokersWithSell(session);
            continue;
        }
        int idx = -1;
        try { idx = std::stoi(input); } catch (...) {}
        if (idx < 0) { std::cout << "Invalid input.\n"; continue; }
        shopManager.purchaseItem(session, idx);
        shopManager.printShop();
        shopManager.printJokersWithSell(session);
    }
    shopManager.closeShop();
}

void GameManager::printStatus() const {
    std::cout << "Ante: " << session.persistent.ante
              << " | Money: $" << session.persistent.money << "\n";
}
