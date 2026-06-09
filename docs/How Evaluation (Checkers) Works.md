# Poker Hand Evaluation System (Checkers)
The evaluation system uses a deterministic, priority-ordered Chain of Responsibility pattern to identify the highest-scoring poker hand from the player's ChosenHand. Rather than utilizing a single monolithic evaluation function, the system delegates responsibility to a sequence of isolated, specialized Checker components.

## The Chain of Responsibility Pipeline
When resolveHand() is invoked, the ChosenHand payload is passed down an ordered hierarchy of individual hand checkers. Each checker focuses exclusively on a single hand pattern (e.g., Flush, Full House, Two Pair).
```
[ChosenHand Payload]
                 │
                 ▼
     ┌───────────────────────┐
     │  StraightFlushChecker │ ──► Matches? ──(Yes)──► [Return Straight Flush Type]
     └───────────┬───────────┘
                 │ (No)
                 ▼
     ┌───────────────────────┐
     │  FourOfAKindChecker   │ ──► Matches? ──(Yes)──► [Return Four of a Kind Type]
     └───────────┬───────────┘
                 │ (No)
                 ▼
                 :
        (Remaining Checkers)
                 :
                 │ (No)
                 ▼
     ┌───────────────────────┐
     │    HighCardChecker    │ ──► Matches? ──(Yes)──► [Return High Card Type]
     └───────────────────────┘
```
- **Short-Circuit Evaluation**: The evaluation loop processes checkers strictly from highest hand rank to lowest hand rank. The first checker that validates the card combination successfully aborts further execution and returns its specific hand type. This structural ordering guarantees that a Straight Flush is never mistakenly evaluated or bypassed as a simple Flush or Straight.  
- **The High Card Fallback**: The final link in the chain is a catch-all HighCardChecker. If all preceding structural pattern checkers fail, this component automatically returns a valid HighCard type, ensuring that every valid selection of cards can be scored. 

## internal Execution Flow of a Checker
Every individual checker follows a strict three-step execution lifecycle to determine whether the card array satisfies its structural requirements:
```
┌─────────────────────────────┐
│ 1. Frequency Map Analysis  │ ──► Counts card frequencies by Rank and Suit.
└──────────────┬──────────────┘
               ▼
┌─────────────────────────────┐
│    2. Metric Validation     │ ──► Evaluates structural conditions (e.g., pairs, consecutive ranks).
└──────────────┬──────────────┘
               ▼
┌─────────────────────────────┐
│   3. Structural Decision    │ ──► Returns Hand Type to short-circuit, or passes to next checker.
└─────────────────────────────┘
```

### 1. Frequency Map Analysis
Before executing matching logic, the checker counts the elements by creating two internal frequency distributions from the ChosenHand collection:
- **Rank Map**: Maps each distinct card value (e.g., Ace, King, 10) to its total occurrences in the hand.
- **Suit Map**: Maps each distinct card suit (e.g., Hearts, Spades) to its total occurrences in the hand.

### 2. Metric Validation
The checker matches these frequency maps against its specific rule criteria. For example:
- **Full House Checker**: Scans the Rank Map to verify it contains exactly one rank with a frequency count of 3, and exactly one distinct rank with a frequency count of 2.
- **Flush Checker**: Scans the Suit Map to verify that a single suit has a frequency count equal to the total number of played cards (typically 5).
- **Straight Checker**: Sorts the distinct keys of the Rank Map to verify that they form a continuous, unbroken mathematical sequence of values.

## 3. Structural Decision
- **On Match Success**: The checker constructs a hand definition object containing the identified hand type, sets the short-circuit flag to true, and hands control back to the PokerHandEvaluator.  
- **On Match Failure**: The checker returns a null or invalid result, signaling the evaluation loop to pass the unchanged ChosenHand payload to the next checker in the sequence.

