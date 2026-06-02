# Overview
This repository is a from-scratch recreation of Balatro, built entirely in C++. Balatro is a hypnotically addictive, poker-themed roguelike deck-builder. Instead of playing against traditional opponents, players face off against increasingly difficult point thresholds called "Blinds." The goal is to play valid poker hands to generate a score, which is calculated by multiplying Chips by a Multiplier ($\text{Chips} \times \text{Mult}$). The true core of the game lies in collecting game-changing Joker cards that trigger complex, cascading math formulas and outrageous point combos, turning standard card game rules completely on their head.

## Engine Capabilities
This project functions as a standalone simulation of those core mechanics, designed to:
- **Evaluate Poker Hands**: Accurately identify and grade standard, modified, and illegal poker hands.
- **Calculate Scores**: Compute the dynamic scaling and interactions of Chips and Multipliers.
- **Apply Joker Effects**: Process a modular system for unique Joker triggers, conditional behaviors, and stacking modifiers.
- **Manage Player Actions**: Seamlessly handle fundamental game states like PLAY and DISCARD.

## Architecture
The system utilizes a highly modular architecture. Every mechanic—from base card properties to volatile Joker synergies—operates with distinct boundaries. This decoupled structure allows developers to easily extend the engine, experiment with new custom jokers, or adjust scoring logic independently without destabilizing the core codebase.

# Technical Design Documents
- [High Level Architecture](/docs/High%20Level%20Architecture.md)
- [Runtime Flow](/docs/Runtime%20Flow.md)
- [How Evaluation (Checkers) Works](/docs/How%20Evaluation%20(Checkers)%20Works.md)
- [Scoring System](/docs/Scoring%20System.md)
- [Game Manager Architecture](/docs/Game%20Manager%20System.md)
- [Card and Hand Data Structures](/docs/Card%20and%20Hand%20Data%20Structures.md)
- [Joker System](/docs/Joker%20System.md)
- [Shop System](/docs/Shop%20System.md)
- [Blind State System and Skip Reward Command System](/docs/Blind%20State%20System%20and%20Skip%20Reward%20Command%20System.md)

# Author
- Name : Muhammad Syifaul Qolbi
- Class: GT 12 C
- NRP  : 5224600072
