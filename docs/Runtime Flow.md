# Runtime Flow and System Execution
The core runtime loop controls the lifecycle of a round, transforming raw player input into evaluated score outputs or state changes. This section breaks down the initialization sequence, the execution pipeline of the core loop, and the detailed branching logic for playing and discarding cards

## Phase 1: Initialization Sequence
Before entering the main interaction loop, the game state must be structurally initialized. This process sets up the foundation for the session.
'''
[System Boot]
        │
        ▼
 ┌──────────────┐
 │ setupJokers()│ ──► Registers active modifier/Joker effects to the JokerManager.
 └──────┬───────┘
        ▼
┌───────────────┐
│  createDeck() │ ──► Instantiates a standard deck and executes an in-place shuffle.
└──────┬────────┘
        ▼
┌───────────────┐
│ drawInitial() │ ──► Moves cards from the top of the Deck into the player's HandState.
└───────────────┘   
'''

## Phase 2: The Core Game Loop
Once initialized, the system falls into a continuous evaluation loop monitored by the GameManager. The loop handles UI updates, processes action requests, and ensures all choices are valid under current game constraints.

                  ┌─────────────────────────────────┐
                  │    Display Hand & Game UI       │◄────────────────┐
                  └────────────────┬────────────────┘                 │
                                   │                                  │
                                   ▼                                  │
                  ┌─────────────────────────────────┐                 │
                  │   readPlayerActionRequest()     │                 │
                  └────────────────┬────────────────┘                 │
                                   │                                  │
                                   ▼                                  │
                  ┌─────────────────────────────────┐                 │
                  │      canPerformAction()?        │                 │
                  └────────────────┬────────────────┘                 │
                                   │                                  │
                    ┌──────────────┴──────────────┐                   │
                    │                             │                   │
            [Valid Action]                [Invalid Action]            │
                    │                             │                   │
                    ▼                             ▼                   │
       ┌────────────────────────┐      ┌─────────────────────┐        │
       │ processPlayerAction()  │      │ Print Error Message │────────┘
       └────────────────────────┘      └─────────────────────┘

1. **State Presentation**: The system outputs the current contents of HandState, available discards, remaining hands, and current score metrics.  
2. **Input Capture (readPlayerActionRequest)**: The system waits for and captures the player's choice—identifying which cards are selected and whether they want to Play or Discard.  
3. **Validation (canPerformAction)**: The system checks constraints before modifying data. For example, it ensures the player has remaining hands for a play, remaining discards for a discard, and has selected between 1 and 5 cards.  

## Phase 3: Action Processing Branches (processPlayerAction)
Once validated, the payload is routed into one of two primary execution paths based on the action type.
### Branch A: The Play Path
When a player commits a hand to be scored, the system executes a precise evaluation pipeline:
'''
[ChosenHand Selected] 
         │
         ▼
┌───────────────────────────────┐
│    resolveHand() Evaluation   │ ──► Passes cards to PokerHandEvaluator to determine type.
└──────────────┬────────────────┘
               ▼
┌───────────────────────────────┐
│     createScoreContext()      │ ──► Fetches base Chips & Multiplier from the matching ScoringRule.
└──────────────┬────────────────┘
               ▼
┌───────────────────────────────┐
│  JokerManager.applyJokers()   │ ──► Iterates through active Jokers to modify Chips and Multiplier.
└──────────────┬────────────────┘
               ▼
┌───────────────────────────────┐
│       Final Computation       │ ──► Computes final score ($Total = \text{Chips} \times \text{Multiplier}$).
└──────────────┬────────────────┘
               ▼
┌───────────────────────────────┐
│         State Update          │ ──► Adds to cumulative score, decrements hands remaining.
└──────────────┬────────────────┘
               ▼
┌───────────────────────────────┐
│     Draw Replacement Cards    │ ──► Draws back up to maximum hand size, cleaning ChosenHand.
└───────────────────────────────┘
'''

### Branch B: The Discard Path
When a player opts to cycle cards to fish for a better hand, the system bypasses evaluation and updates the state directly:
- **Card Destructuring**: The specific card instances stored in ChosenHand are permanently removed from HandState and placed into a discard pile.
- **Resource Cost**: The remaining discard counter tracked by the GameManager is decremented by 1.
- **Refill Pipeline**: The system immediately triggers the draw service, moving new cards from the top of the Deck into HandState until the hand size limit is restored.
- **Reset**: The ChosenHand collection is cleared to prepare for the next player action.
