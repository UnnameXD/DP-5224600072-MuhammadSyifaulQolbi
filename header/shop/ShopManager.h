#pragma once
#include <vector>
#include <memory>
#include <string>
#include "../joker/Joker.h"
#include "../RunSessionState.h"

enum class ShopSlotType { JokerItem, VoucherItem, BoosterPack };

struct ShopItemSlot {
    int          slotId      = 0;
    ShopSlotType type        = ShopSlotType::JokerItem;
    int          baseCost    = 0;
    bool         isPurchased = false;
    bool         isFree      = false;       // set by Foil Tag
    JokerEdition edition     = JokerEdition::NONE;
    int          jokerIdx    = -1;          // index into jokerPool
    std::string  label;
};

class ShopManager {
public:
    static constexpr int BASE_REROLL_COST = 5;

    void openShop(RunSessionState& session);
    void printShop() const;
    bool purchaseItem(RunSessionState& session, int slotIndex);
    bool reroll(RunSessionState& session);
    void closeShop();
    void printJokersWithSell(const RunSessionState& session) const;

private:
    std::vector<ShopItemSlot> slots;
    int rerollCount = 0;

    void generateSlots(RunSessionState& session);
    void addJokerToSession(RunSessionState& session, ShopItemSlot& slot);
    void addVoucherToSession(RunSessionState& session, ShopItemSlot& slot);
    void openBoosterPack(RunSessionState& session);

    int rerollCost() const { return BASE_REROLL_COST + rerollCount; }
};
