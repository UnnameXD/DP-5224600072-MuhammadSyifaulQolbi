# High Level Architecture
The project is structured around a modular, responsibility-driven architecture to facilitate independent extension and clear separation of concerns. The system core is organized into the following components:
## Structural System Components
- **GameManager**: Acts as the central orchestrator, controlling the overall lifecycle and high-level game flow sessions.  
- **Deck**: Responsible for creating, shuffling, storing, tracking, and drawing from a standard pool of playing cards.
- **HandState**: Serves as the storage entity for the collection of cards currently held in the player's hand.  
- **ChosenHand**: A dedicated data structure that stores only the specific subset of cards selected by the player for hand scoring.

## Evaluation & Scoring Subsystems
- **PokerHandEvaluator**: Analyzes the selected cards using a priority-based chain of individual checkers to detect the valid poker hand type.  
- **ScoringRule**: Implements the core mathematical formulas to calculate base chip and multiplier values.  
- **JokerManager**: Manages the collection of active modifiers (Jokers) and handles the registration and notification process during calculation phases. 
- **ScoreContext**: A mutable data container that encapsulates the runtime score properties ($\text{chips}$, $\text{multiplier}$, hand details) for real-time manipulation by external systems.  

## Architectural Responsibility Mapping

| Component          | Responsibility                            |
| ------------------ | ----------------------------------------- |
| GameManager        | Controls the overall game flow            |
| Deck               | Stores and manages cards                  |
| HandState          | Stores cards currently held by the player |
| ChosenHand         | Stores cards selected for scoring         |
| PokerHandEvaluator | Detects poker hand types                  |
| ScoringRule        | Calculates base scores                    |
| Joker Manager      | Applies Joker Effects                     |
| ScoreContext       | Stores mutable score data                 |

```
  End-to-End System Flow DiagramThe runtime execution sequence transitions across three major architectural boundaries: Session Setup, the Main Gameplay Loop, and contextual Action Branches.  [Runtime Initialization: main()]
       │
       ▼
┌──────────────────────────────────────────────────────── ┐
│ 1. SESSION SETUP                                        │
│    - Register active effects via setupJokers()          │
│    - Build & shuffle pool via createShuffledDeck()      │
│    - Populate initial player state via drawInitialHand()│
└──────────────────┬───────────────────────────────────── ┘
                   │
                   ▼
┌────────────────────────────────────────────────────────┐
│ 2. MAIN GAMEPLAY LOOP (Repeats via runSessionLoop())   │
│    - Output runtime state to interface                 │
│    - Gather payload using readPlayerActionRequest()    │
│    - Validate constraints using canPerformAction()     │
└──────────────────┬─────────────────────────────────────┘
                   │
                   ▼
       [processPlayerAction()]
         ├─── (PLAY Action) ──────► 3A. PLAY BRANCH
         │                             - Evaluate hand pattern via resolveHand()
         │                             - Allocate mutable payload via createScoreContext()
         │                             - Pipeline modifications via applyJokers()
         │                             - Finalize evaluations & call printResult()
         │
         └─── (DISCARD Action) ───► 3B. DISCARD BRANCH
                                       - Extract selected elements via DiscardService
                                       - Draw replacement entries via DrawService
                                       - Decrement session budget tracking counters
```