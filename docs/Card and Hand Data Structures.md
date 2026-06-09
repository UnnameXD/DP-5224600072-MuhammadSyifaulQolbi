# Card and Hand Data Structures
The system relies on lightweight, encapsulated data structures to maintain state integrity across card operations, hand selections, and inventory adjustments. These structures separate raw card identities from their dynamic groupings within game scenes.

## Core Structural Layout
```
┌────────────────────────────────────────┐
                  │                 Card                   │
                  │ - Rank (2-Ace)                         │
                  │ - Suit (Hearts, Spades, etc.)          │
                  └───────────────────┬────────────────────┘
                                      │
              ┌───────────────────────┼───────────────────────┐
              │                       │                       │
              ▼                       ▼                       ▼
   ┌────────────────────┐  ┌────────────────────┐  ┌────────────────────┐
   │        Deck        │  │     HandState      │  │     ChosenHand     │
   │ [Ordered Contiguous│  │ [Dynamic Boundless │  │ [Strictly Bounded  │
   │       Array]       │  │    Collection]     │  │   Array: Max 5]    │
   └────────────────────┘  └────────────────────┘  └────────────────────┘
```

### 1. The Card Component
The fundamental element of the system is the Card structure, which defines an immutable atomic identity for an individual card asset.  
- **Rank Field**: An enumeration value representing the numerical face value (e.g., Two through Ace). This value maps directly to an underlying integer weight used by the mathematical evaluation checkers to resolve straights and high-card tiebreakers.  
- **Suit Field**: An enumeration value defining the suit classification (Hearts, Diamonds, Clubs, Spades). This field is read by the FlushChecker to evaluate uniform suit arrays and by specific card-targeted Jokers. 

### 2. The Deck Container
The Deck manages the state of unspent cards available to enter play.  
- Data Representation: It is implemented as a contiguous linear array or list containing up to 52 default Card instances.  
- State Behavior:
  - Shuffling: Implements an in-place shuffle algorithm (such as a Fisher-Yates randomization pipeline) that reorders the underlying indexes before drawing actions occur.  
  - Drawing: Acts as a stack structure where drawing a card extracts the top element, shrinking the available array size by one and transferring ownership of that instance to the player's active hand. 
  
### 3. The Hand State Container
The HandState represents the total collection of cards currently held by the player.  
- Data Representation: An unbounded dynamic collection or list that tracks all card instances currently available for player interaction.  
- State Behavior:
  - This structure expands when receiving new cards from the Deck draw pipeline and shrinks when chosen items are spent during scoring plays or discarded cycles.  
  - It maintains reference synchronization with the scene's rendering layer to update visible cards in the player's interface.  

### 4. The ChosenHand Container
The ChosenHand is a specialized, strictly bounded selection buffer representing the staging zone for immediate player actions.  
- Data Representation: A fixed-capacity linear array constrained to hold between 0 and 5 elements.  
- State Behavior:
  - **Selection Isolation**: When a player selects a card from their UI hand layout, that card instance is registered inside the ChosenHand container without being stripped from the primary HandState pool.  
  - **Capacity Guarding**: The array implements size checking. If a player attempts to add a 6th card to the selection, the system rejects the operation and blocks action execution until elements are unselected.  
  - **Action Consumption**: When a Play or Discard action passes system validation, the contents of ChosenHand dictate exactly which card references are passed to the evaluation system or wiped from HandState completely.