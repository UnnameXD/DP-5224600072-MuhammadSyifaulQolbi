# Game Manager System Architecture
The GameManager serves as the central orchestration hub for the entire application. It functions as a state machine that manages the high-level game loop, tracks session-wide resources, and coordinates data flow between isolated subsystems like the deck, player hand, and scoring engine.

## Core State Tracking and Resources
To maintain the integrity of a run, the GameManager encapsulates and mutates several key state variables tracking round-specific limits and requirements:  
- Current Score Accumulator: A running total tracking the cumulative points earned from played hands within the active round.  
- Target Score (Blind Requirement): The static point threshold required to pass the active round.
- Remaining Hands Counter: A strictly bounded integer tracking how many scoring attempts the player has left before failing the round.  
- Remaining Discards Counter: A strictly bounded integer tracking how many card cycling actions are left available to the player. 

## High Level Architectural Coordination
The GameManager acts as an intermediary, ensuring structural components never communicate directly with each other, keeping the codebase decoupled.
```
                     ┌──────────────────────┐
                     │     GameManager      │
                     └─▲──────┬────────▲────┬─┘
                       │      │        │    │
         ┌─────────────┘      │        │    └──────────────┐
         │                    ▼        │                   ▼
┌────────┴──────────┐   ┌─────────────┴──────┐   ┌───────────────────┐
│ Deck & HandState  │   │ PokerHandEvaluator │   │   JokerManager    │
│ (Card Management) │   │ (Pattern Matching) │   │ (Score Mutation)  │
└───────────────────┘   └────────────────────┘   └───────────────────┘
```
- **Card Distribution**: It instructs the Deck to shuffle and populates the HandState collection during initialization or after hand consumption.  
- **Evaluation Routing**: Upon a play action, it extracts the ChosenHand array and passes it directly to the PokerHandEvaluator.  
- **Score Pipeline Ingestion**: It takes the evaluated hand type, fetches the base scoring rules, instantiates the mutable ScoreContext, and hands it over to the JokerManager for modification loops. 

## Turn Resolution and State Transition Logic
When an action is processed, the GameManager runs a check to determine if the game state should transition, wrap up, or end the session:
[Hand Evaluation Complete] ──► Accumulate Score ──► Decrement Hands Remaining
```
                                                          │
                                                          ▼
                                            ┌───────────────────────────┐
                                            │   Is Current Score >=     │
                                            │      Target Score?    │
                                            └─────────────┬─────────────┘
                                                          │
                            ┌─────────────────────────────┴─────────────────────────────┐
                            │                                                           │
                         ( Yes )                                                     ( No )
                            │                                                           │
                            ▼                                                           ▼
             ┌─────────────────────────────┐                             ┌───────────────────────────┐
             │    Trigger Round Win        │                             │  Is Hands Remaining == 0? │
             │  (Advance to Next Blind)    │                             └──────────────┬────────────┘
             └─────────────────────────────┘                                            │
                                                          ┌─────────────────────────────┴─────────────┐
                                                          │                                           │
                                                       ( Yes )                                     ( No )
                                                          │                                           │
                                                          ▼                                           ▼
                                           ┌─────────────────────────────┐             ┌─────────────────────────────┐
                                           │     Trigger Game Over       │             │     Refill Player Hand      │
                                           │      (Reset Session)        │             │  (Await Next Action Input)  │
                                           └─────────────────────────────┘             └─────────────────────────────┘
```
1. **Score Validation**: The calculated score payload is added to the running total, and the hands counter is decremented by 1.  
2. **Win Condition Check**: If the cumulative score meets or exceeds the target threshold, the round immediately terminates successfully, advancing the player to the next setup phase.
3. **Loss Condition Check**: If the score is below the target and the remaining hands counter hits 0, the manager halts the loop, bypasses further input requests, and triggers the game over sequence.  
4. **Loop Continuation**: If neither condition is met, the manager invokes the draw service to replenish the player's hand back to full capacity and returns to the input gathering phase, waiting for the next move.