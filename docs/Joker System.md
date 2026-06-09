# Joker Modification System
The Joker System serves as the primary modifier engine for adjusting scores mid-round. It operates using a decoupled Observer Pattern, where individual Jokers act as listeners that intercept, evaluate, and mutate the ScoreContext payload before final calculations are completed.

## Structural Architecture
The system splits responsibilities across three core layers to keep gameplay logic separated from data storage:
```
┌────────────────────────────────────────────────────────┐
│                   1. Joker Interface                   │
│ Defines the contract: evaluateModifier(context, hand)  │
└───────────────────────────┬────────────────────────────┘
                            ▼
┌────────────────────────────────────────────────────────┐
│                    2. JokerManager                     │
│ Tracks equipped instances & orchestrates pipeline loops│
└───────────────────────────┬────────────────────────────┘
                            ▼
┌────────────────────────────────────────────────────────┐
│              3. Concrete Implementations               │
│ Executable scripts defining unique conditional logic   │
└────────────────────────────────────────────────────────┘
```
- **The Joker Interface**: A standardized contract requiring all Jokers to implement a core modification method (e.g., evaluateModifier(ScoreContext context, PokerHand hand)). This ensures the execution engine can process any item uniformly without needing to know its internal logic. 
- **The JokerManager**: A tracking registry that manages the player's current inventory of equipped modifiers. It acts as the central hub, receiving the scoring payload from the GameManager and passing it sequentially through each active modifier.  
- **Concrete Implementations**: Individual scripts or data objects representing specific items (e.g., Gros Michel, Ice Cream, Blueprint). Each concrete implementation holds its own validation conditions and arithmetic properties.

## The Two-Stage Execution Pipeline
To maintain mathematical consistency and prevent visual bugs where score totals change depending on inventory order, the JokerManager executes the modification process in two completely isolated passes.

```
[ScoreContext Input] ──► ( Pass 1: Additive Loop ) ──► ( Pass 2: Multiplicative Loop ) ──► [Final Mutated Context]
```
### Pass 1: The Additive Loop
The manager loops through the equipped inventory from left to right, invoking only the flat addition logic on matching items.  
- Scope: Modifies context.chips or context.multiplier via addition ($+ \text{Chips}$ or $+ \text{Mult}$).  
- Goal: Accumulates a solid scoring foundation across all items before any scaling factors apply. 

### Pass 2: Multiplicative Loop
The manager resets its index and loops through the equipped inventory a second time, invoking only the scaling multiplier logic on matching items.  
- Scope: Modifies context.multiplier via multiplication ($\times \text{Mult}$).  
- Goal: Applies scaling values to the combined sum of the base score and flat bonuses, ensuring maximum point impact without math ordering bugs. 

## Conditional Evaluation & Execution Flow
When a Joker is processed within either loop, it follows a strict local validation flow before updating the data payload:
```
┌───────────────────────────────┐
                    │  Receive Context & Hand Data  │
                    └───────────────┬───────────────┘
                                    │
                                    ▼
                    ┌───────────────────────────────┐
                    │   Does Hand Match Criteria?   │
                    └───────────────┬───────────────┘
                                    │
                    ┌───────────────┴───────────────┐
                    │                               │
                 ( Yes )                         ( No )
                    │                               │
                    ▼                               ▼
       ┌─────────────────────────┐     ┌─────────────────────────┐
       │ Mutate Score Properties │     │    Forward Payload Un-  │
       │ (Apply + / x Values)    │     │   modified to Next Link │
       └─────────────────────────┘     └─────────────────────────┘
```
1. **Context Ingestion**: The Joker reads the current state of the ScoreContext (the running chip and multiplier totals) along with the evaluated poker hand type.  
2. **Criteria Validation**: The item checks its own activation rules against the current context.
   - Type-Based Criteria: Checks if the played hand matches a specific pattern (e.g., "Is this a Flush?").  
   - Suit/Rank Criteria: Scans the individual cards in the hand for specific properties (e.g., "Contains at least one Diamond card?").
   - State-Based Criteria: Checks environmental factors (e.g., remaining discards, total money, or current round number).
3. **Payload Mutation**:
   - If validation succeeds, the Joker applies its arithmetic changes directly to the mutable properties of the ScoreContext and forwards it down the line.  
   - If validation fails, the Joker completely bypasses mutation, leaving the properties untouched, and forwards the payload immediately to the next item in the registry. 