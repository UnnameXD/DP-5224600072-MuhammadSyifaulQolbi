# Shop System
The Shop System handles the economy and metagame progression loops between blinds. Following the principles of Runtime State Separation, the shop does not deal with local in-round scoring data (like chips, multipliers, or hands). Instead, it uses a transactional model that interacts strictly with the Persistent State container, as items purchased in the shop must survive across future blinds.

## Architectural Responsibility Boundaries
The shop functions as a decoupled subsystem that requires explicit dependencies to manipulate the game state safely:
┌────────────────────────────────────────────────────────────────┐
│                         ShopManager                            │
│  - Tracks local session data (e.g., current reroll cost)       │
│  - Generates item slots using weighted pool generation tables  │
└──────────────────────────────┬─────────────────────────────────┘
                               │
            ┌──────────────────┴──────────────────┐
            ▼                                     ▼
┌──────────────────────┐               ┌──────────────────────┐
│  RunPersistentState  │               │   UI Presentation    │
│  - Modifies money    │               │  - Recieves data to  │
│  - Stores inventory  │               │    render visual slot │
└──────────────────────┘               └──────────────────────┘
- **ShopManager**: A transient or state-controlled manager that orchestrates generation, reroll pricing, and transaction validation. It holds temporary shop-specific state, such as how many times the player has clicked "Reroll" during this single visit.
- **RunPersistentState Compatibility**: The shop reads persistent.money to authorize purchases and writes directly to inventories like persistent.jokers or persistent.pendingCommands when transactions succeed.  

## Structural Shop Layout & Data Representations
When a shop session is initialized, it builds an inventory array of available purchase choices grouped by category types:

C++
``` cpp
enum class ShopSlotType {
    JokerItem,      // Standard card inventory slot
    VoucherItem,    // Permanent run-wide upgrades
    BoosterPack     // Blind pack containing choices
};

struct ShopItemSlot {
    int uniqueSlotId;
    ShopSlotType type;
    int baseCost;
    bool isPurchased = false;
    
    // Abstract base content pointer (can represent a Joker, a Voucher, or a Pack)
    std::unique_ptr<PurchasableContent> content; 
};
```

## The Transaction Lifecycle Pipeline
Every purchase or reroll choice flows through a strict operational sequence to prevent state manipulation or negative balancing bugs:
```
[Player Clicks Purchase Item]
              │
              ▼
Verify Item Availability (isPurchased == false)
              │
              ▼
Check Financial Constraints (persistent.money >= slot.baseCost)
              │
              ├───► [Passes] ──► Deduct Money -> Extract Content -> Add to Target Inventory
              │                                                             │
              │                                                             ▼
              │                                                    Set isPurchased = true
              │
              └───► [Fails]  ──► Reject Action -> Error Notification
```

### 1. Purchase Transaction Execution
``` cpp
bool purchaseItem(RunSessionState& session, ShopItemSlot& slot) {
    if (slot.isPurchased) return false;
    if (session.persistent.money < slot.baseCost) return false;

    // Deduct persistent currency asset
    session.persistent.money -= slot.baseCost;
    slot.isPurchased = true;

    // Route content payload based on its target structural type
    if (slot.type == ShopSlotType::JokerItem) {
        // Cast directly and append to persistent collections
        auto jokerPtr = unique_cast<Joker>(std::move(slot.content));
        session.persistent.jokers.push_back(std::move(jokerPtr));
    } 
    else if (slot.type == ShopSlotType::VoucherItem) {
        // Vouchers immediately activate their logic hooks and append to modifiers
        slot.content->activateVoucherEffects(session);
    }
    
    return true;
}
```

### 2. The Dynamic Reroll Engine
The cost of rolling a new batch of items scales continuously with each consecutive deployment inside the current shop window.
- **Mathematical Model**:
  $$Cost_{Reroll} = Base + (Num_{Rerolls} \times ScalingFactor)$$
- **Lifecycle Boundary**: The variable tracking $Num_{Rerolls}$ lives purely inside the local runtime wrapper of the active shop session. The moment the player advances to the next blind encounter, this count resets to 0, ensuring the next shop starts fresh at the base price.