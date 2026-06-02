# Blind State System & Skip Reward Command System
Applying the principle of Runtime State Separation, the macro systems of a run are explicitly segregated by their lifecycles to prevent state leakage and side effects during testing and execution. The Blind State System represents mutable runtime state, whereas the Skip Reward Command System relies on deferred command objects stored safely inside the persistent state container.

## Macro Architecture & Lifecycle Boundary
The system manages data across strict architectural boundaries using the recommended structural wrapper:
``` cpp
struct RunSessionState {
    RunPersistentState persistent;
    BlindRuntimeState runtime;
};
```
This ensures a clear division between what survives across multiple blinds and what resets immediately when entering a new stage.  
```
RUN START
   │
   ▼
[Create RunPersistentState] ──► Tracks: ante, money, jokers, pendingCommands
   │
   ├──► ENTER BLIND
   │       │
   │       ▼
   │   [Reset BlindRuntimeState] ──► Instantiates: blindScore = 0, hands = 4, discards = 3
   │       │
   │       ├──► PLAYER ACTION
   │       │       │
   │       │       ▼
   │       │   [Create Temporary State] ──► ScoreContext instantiated, processed, and destroyed
   │       │
   │       └──► BLIND END ──► BlindRuntimeState destroyed
   │
   ▼
RUN END
```

## 1. Blind State System (Runtime State)
The BlindRuntimeState operates strictly within the boundaries of a single blind lifecycle. It represents highly mutable data that governs the immediate win/loss loop of the active combat layer.

**Structural Definition:**
``` cpp
struct BlindRuntimeState {
    int blindScore = 0;
    int remainingHands = 4;
    int remainingDiscards = 3;
    int scoreTarget = 0; // Configured by the active Blind Type
};
```
**Lifecycle Rules:**
- **Initialization (Enter Blind):** Every time the player selects or enters a blind, a fresh BlindRuntimeState is instantiated or reset. Progression variables from RunPersistentState (such as the current Ante level) are read only to calculate the scoreTarget for this isolated session.  
- **Mutation Scope:** Actions like playing a hand or discarding cards mutate fields strictly inside this struct (e.g., blindScore += finalScore, remainingHands--).  
- **Destruction (Blind End):** Once blindScore >= scoreTarget (Win) or remainingHands == 0 (Loss), the state is finalized. Upon exiting to the shop or run-over sequence, this entire structural instance is discarded, guaranteeing no calculation relics bleed into subsequent blinds. 

## Skip Reward Command System (Persistent & Command Pattern)
Skipping a blind acts as a macro-progression choice that rewards the player with a deferred benefit. Because rewards may execute at different times (e.g., immediate cash, free items in the next shop, or modifications to a future booster pack), they cannot live in temporary or runtime states. They are modeled via the Command Pattern and tracked inside the persistent layer.

**Architectural Integration:**
``` cpp
// Command Interface for deferred actions
class SkipRewardCommand {
public:
    virtual ~SkipRewardCommand() = default;
    virtual void execute(RunSessionState& sessionState) = 0;
    virtual bool shouldTriggerOn(enum class TriggerTiming timing) = 0;
};

// Persistent container capturing the command array
struct RunPersistentState {
    int ante = 1;
    int money = 0;
    std::vector<std::unique_ptr<Joker>> jokers;
    std::vector<std::unique_ptr<SkipRewardCommand>> pendingCommands; // Deferred rewards
};
```

## Operational Execution Flow
```
[Player Skips Blind]
          │
          ▼
Instantiate Concrete Command (e.g., ShopJokerUpgradeCommand)
          │
          ▼
Push to sessionState.persistent.pendingCommands list[cite: 4]
          │
          ▼
Trigger Global State Transition: Advance Ante / Load Shop
          │
          ▼
[Shop Is Loaded] ──► Query commands matching TriggerTiming::OnShopEnter
          │
          ├───► True  ──► Command.execute(sessionState) ──► Mutates persistent state, removes from list
          └───► False ──► Retained in list for later evaluation
```

## Structural Command Types & Execution Timings
Commands use strict boundary rules to isolate processing logic:
|**Concrete Command**|**Execution Timing**|**Targeted Mutation Scope**|
|---|---|---|
|`InstantCashReward`|`TriggerTiming::Immediate`|Adds directly to `persistent.money`.|
|`ShopJokerUpgrade`|`TriggerTiming::OnShopEnter`|Intercepts shop generation to force a specific holographic/foil modifier on an item.|
|`MegaPackVoucher`|`TriggerTiming::OnPackOpen`|Mutates the generation parameters of a booster session to increase choices.|

## 