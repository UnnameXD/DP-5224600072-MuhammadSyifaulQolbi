# Scoring System
Once the evaluation pipeline identifies the played poker hand type, the system shifts responsibility to the scoring engine. The computation follows a strict order of operations: extracting **Base Values**, calculating **Contextual Accumulation**, and executing a two-stage **Joker Modification Pipeline.**

## The Mathematical Model
The total score for any played hand is calculated using a basic formula:

$$Total\ Score = Chips \times Multiplier$$

Both variables start with base values determined by the hand type, but they remain mutable throughout the calculation phase, allowing external modifiers (like Jokers) to alter either side of the equation.

## The Step by Step Scoring Pipeline
[Detected Hand Type]
           │
           ▼
┌──────────────────────────────┐
│  1. Base Value Extraction    │ ──► Look up default Chips and Multiplier from ScoringRule.
└──────────┬───────────────────┘
           ▼
┌──────────────────────────────┐
│ 2. ScoreContext Allocation   │ ──► Instantiate mutable wrapper holding current score state.
└──────────┬───────────────────┘
           ▼
┌──────────────────────────────┐
│  3. Flat Modifier Pipeline   │ ──► Process additive (+Chips, +Mult) Joker effects.
└──────────┬───────────────────┘
           ▼
┌──────────────────────────────┐
│ 4. Product Modifier Pipeline │ ──► Process multiplicative (xMult) Joker effects.
└──────────┬───────────────────┘
           ▼
┌──────────────────────────────┐
│     5. Final Evaluation      │ ──► Cast variables to final integers and multiply.
└──────────────────────────────┘

## Step 1: Base Value Extraction
The GameManager references the ScoringRule registry using the validated hand type returned by the checkers. Each hand type maps to an immutable set of starting numbers. For example:  
- Pair: 10 Chips $\times$ 2 Multiplier
- Flush: 35 Chips $\times$ 4 Multiplier

## Step 2: ScoreContext Allocation
The system instantiates a mutable ScoreContext payload. This object acts as a data container that travels through the modifier systems, keeping track of the running totals for: 
- context.chips (The current accumulation of chip points)
- context.multiplier (The current accumulation of the scoring multiplier)

## Step 3: Flat Modifier Pipeline (Addition)
The ScoreContext is passed to the JokerManager. The system loops through all active, equipped Jokers to apply flat arithmetic additions first.
- Example: A Joker might state: "If played hand contains a Pair, add +40 Chips and +4 Multiplier."
- Resulting Mutation:
  $$\text{chips} = \text{chips} + 40$$
  $$\text{multiplier} = \text{multiplier} + 4$$

## Step 4: Product Modifier Pipeline (Multiplication)
After all additive calculations finish, the ScoreContext passes through a second loop over the equipped Jokers to apply multiplicative scaling factors. Splitting additions and multiplications into separate steps ensures that flat bonuses are always scaled by multipliers, preventing compounding math bugs based on the arbitrary inventory order of the player's items.
- Example: A Joker might state: "If played hand is a Flush, trigger $\times 2$ Multiplier."
- Resulting Mutation:
  $$\text{multiplier} = \text{multiplier} \times 2$$

## Step 5: Final Evaluation and Round Update
With all modifier loops complete, the system extracts the final values from the ScoreContext wrapper, rounds any floating-point calculations to the nearest integer, and computes the final score product. This value is added directly to the round's cumulative score tracker, and the UI updates to show the results of the calculation.