#include <iostream>
#include <cstdlib>
#include "shop/ShopManager.h"
#include "joker/ConcreteJokers.h"

struct JokerEntry { std::string name; int cost; };

static const std::vector<JokerEntry> jokerPool = {
    {"Joker",            3}, {"Jolly Joker",      4}, {"Zany Joker",       4},
    {"Greedy Joker",     5}, {"Lusty Joker",       5}, {"Wrathful Joker",   5},
    {"Gluttonous Joker", 5}, {"Half Joker",        5}, {"Fibonacci",        6},
    {"Scary Face",       5}, {"Bloodstone",        6}, {"Blueprint",        7},
};

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

// ~20% chance of a random edition on a shop joker
static JokerEdition rollEdition() {
    int r = std::rand() % 20;
    if (r == 0)  return JokerEdition::FOIL;
    if (r == 1)  return JokerEdition::HOLOGRAPHIC;
    if (r == 2)  return JokerEdition::POLYCHROME;
    if (r == 3)  return JokerEdition::NEGATIVE;
    return JokerEdition::NONE;
}

// Edition cost surcharge
static int editionCost(JokerEdition ed) {
    switch (ed) {
        case JokerEdition::FOIL:        return 2;
        case JokerEdition::HOLOGRAPHIC: return 3;
        case JokerEdition::POLYCHROME:  return 4;
        case JokerEdition::NEGATIVE:    return 5;
        default: return 0;
    }
}

static std::string jokerSlotLabel(int idx, JokerEdition edition) {
    auto j = makeJokerByIndex(idx);
    j->edition = edition;
    return j->getDisplayName() + " -- " + j->getDescription()
         + j->getEditionDescription();
}

void ShopManager::openShop(RunSessionState& session) {
    rerollCount = 0;
    generateSlots(session);
    std::cout << "\n========== SHOP ==========\n";
    std::cout << "Money: $" << session.persistent.money << "\n";
    printShop();
    printJokersWithSell(session);
}

void ShopManager::generateSlots(RunSessionState& session) {
    slots.clear();

    for (int i = 0; i < 2; i++) {
        int idx = std::rand() % 12;
        ShopItemSlot slot;
        slot.slotId   = i;
        slot.type     = ShopSlotType::JokerItem;
        slot.jokerIdx = idx;

        if (session.freeNextJoker && i == 0) {
            if (session.freeNextJokerRare) {
                idx = (std::rand() % 2 == 0) ? 11 : 10;
                slot.jokerIdx = idx;
            }
            slot.edition  = session.freeNextJokerEdition;
            slot.baseCost = 0;
            slot.isFree   = true;
        } else {
            slot.edition  = rollEdition();
            slot.baseCost = jokerPool[idx].cost + editionCost(slot.edition);
            slot.isFree   = false;
        }
        slot.label = jokerSlotLabel(idx, slot.edition);
        slots.push_back(slot);
    }

    // Voucher
    ShopItemSlot voucher;
    voucher.slotId = 2; voucher.type = ShopSlotType::VoucherItem; voucher.jokerIdx = -1;
    if (session.persistent.ante % 2 == 0) {
        voucher.label = "Overstock -- Allows 1 extra card in shop"; voucher.baseCost = 10;
    } else {
        voucher.label = "Wasteful -- +1 Discard each round"; voucher.baseCost = 10;
    }
    slots.push_back(voucher);

    // Booster
    ShopItemSlot pack;
    pack.slotId = 3; pack.type = ShopSlotType::BoosterPack; pack.jokerIdx = -1;
    pack.label = "Standard Buffoon Pack -- Choose 1 of 2 Jokers"; pack.baseCost = 4;
    slots.push_back(pack);
}

void ShopManager::printShop() const {
    std::cout << "\n";
    for (int i = 0; i < (int)slots.size(); i++) {
        const auto& s = slots[i];
        std::string price = s.isPurchased ? "[SOLD]" : (s.isFree ? "FREE" : "$" + std::to_string(s.baseCost));
        std::cout << "  [" << i << "] " << s.label << "  " << price << "\n";
    }
    std::cout << "  [r] Reroll ($" << rerollCost() << ")\n";
    std::cout << "  [s] Sell a Joker\n";
    std::cout << "  [x] Leave Shop\n";
}

void ShopManager::printJokersWithSell(const RunSessionState& session) const {
    const auto& jokers = session.persistent.jokerManager.getJokers();
    if (jokers.empty()) return;
    std::cout << "\nYour Jokers:\n";
    for (int i = 0; i < (int)jokers.size(); i++) {
        std::cout << "  [" << i << "] " << jokers[i]->getDisplayName()
                  << " -- " << jokers[i]->getDescription()
                  << jokers[i]->getEditionDescription()
                  << "  (sell: $" << jokers[i]->getSellValue() << ")\n";
    }
}

bool ShopManager::purchaseItem(RunSessionState& session, int idx) {
    if (idx < 0 || idx >= (int)slots.size()) { std::cout << "Invalid slot.\n"; return false; }
    auto& slot = slots[idx];
    if (slot.isPurchased) { std::cout << "Already purchased.\n"; return false; }
    if (!slot.isFree && session.persistent.money < slot.baseCost) {
        std::cout << "Not enough money! Need $" << slot.baseCost << "\n"; return false;
    }
    if (!slot.isFree) session.persistent.money -= slot.baseCost;
    slot.isPurchased = true;
    if (slot.type == ShopSlotType::JokerItem)       addJokerToSession(session, slot);
    else if (slot.type == ShopSlotType::VoucherItem) addVoucherToSession(session, slot);
    else if (slot.type == ShopSlotType::BoosterPack) openBoosterPack(session);
    return true;
}

void ShopManager::addJokerToSession(RunSessionState& session, ShopItemSlot& slot) {
    auto j = makeJokerByIndex(slot.jokerIdx);
    j->edition = slot.edition;
    if (slot.isFree) {
        std::cout << "[Tag used] " << j->getDisplayName() << " acquired for free!\n";
        session.freeNextJoker = false;
        session.freeNextJokerEdition = JokerEdition::NONE;
        session.freeNextJokerRare    = false;
    }
    session.persistent.jokerManager.addJoker(std::move(j));
}

void ShopManager::addVoucherToSession(RunSessionState& session, ShopItemSlot& slot) {
    if (slot.label.find("Overstock") != std::string::npos) {
        session.persistent.hasHandSizeVoucher = true;
        std::cout << "Voucher: Hand size +1!\n";
    } else {
        session.persistent.hasExtraDiscardVoucher = true;
        std::cout << "Voucher: +1 Discard per blind!\n";
    }
}

void ShopManager::openBoosterPack(RunSessionState& session) {
    std::cout << "\n--- Standard Buffoon Pack ---\n";
    std::cout << "Choose 1 of 2 Jokers:\n";

    int idxA = std::rand() % 12, idxB;
    do { idxB = std::rand() % 12; } while (idxB == idxA);

    auto jA = makeJokerByIndex(idxA); jA->edition = rollEdition();
    auto jB = makeJokerByIndex(idxB); jB->edition = rollEdition();

    std::cout << "  [0] " << jA->getDisplayName() << " -- " << jA->getDescription()
              << jA->getEditionDescription() << "\n";
    std::cout << "  [1] " << jB->getDisplayName() << " -- " << jB->getDescription()
              << jB->getEditionDescription() << "\n";
    std::cout << "  [-1] Skip\n";

    int pick = -99;
    while (pick != 0 && pick != 1 && pick != -1) {
        std::cout << "Choice (0, 1, or -1 to skip): ";
        std::string line; std::getline(std::cin >> std::ws, line);
        try { pick = std::stoi(line); } catch (...) { pick = -99; }
        if (pick != 0 && pick != 1 && pick != -1)
            std::cout << "Invalid. Enter 0, 1, or -1.\n";
    }
    if (pick == 0)      session.persistent.jokerManager.addJoker(std::move(jA));
    else if (pick == 1) session.persistent.jokerManager.addJoker(std::move(jB));
    else                std::cout << "Pack skipped.\n";
}

bool ShopManager::reroll(RunSessionState& session) {
    int cost = rerollCost();
    if (session.persistent.money < cost) {
        std::cout << "Not enough money to reroll! ($" << cost << " needed)\n"; return false;
    }
    session.persistent.money -= cost;
    rerollCount++;
    generateSlots(session);
    std::cout << "Rerolled! ($" << cost << " spent)\n";
    printShop();
    printJokersWithSell(session);
    return true;
}

void ShopManager::closeShop() { std::cout << "Leaving shop.\n"; }
