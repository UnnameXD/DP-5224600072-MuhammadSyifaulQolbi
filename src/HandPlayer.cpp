#include <iostream>
#include <sstream>
#include "HandPlayer.h"

void HandPlayer::playHand(Hand& hand) {
    chosenHand.cards.clear();

    // NOTE: Hand display is handled by GameManager before calling this.
    // This function only handles card index selection.

    while (true) {
        std::cout << "Choose up to 5 cards (example: 1 3 4) or [x] to cancel: ";

        std::string line;
        std::getline(std::cin >> std::ws, line);

        // Cancel: typing x returns empty chosenHand
        if (line == "x" || line == "X" || line == "cancel") {
            chosenHand.cards.clear();
            return;
        }

        std::stringstream ss(line);
        std::vector<Card> tempChosen;
        bool invalidInput = false;
        std::string token;

        while (ss >> token) {
            // Catch any non-numeric token (including x typed mid-line)
            int index = -1;
            try {
                index = std::stoi(token);
            } catch (...) {
                std::cout << "Invalid input: '" << token << "'. Use card numbers or x to cancel.\n";
                invalidInput = true;
                break;
            }

            if (index < 0 || index >= (int)hand.cards.size()) {
                std::cout << "Invalid index: " << index << ". Valid range: 0-"
                          << (int)hand.cards.size() - 1 << "\n";
                invalidInput = true;
                break;
            }

            bool duplicate = false;
            for (const auto& c : tempChosen) {
                if (c.rank == hand.cards[index].rank &&
                    c.suit == hand.cards[index].suit) {
                    duplicate = true; break;
                }
            }
            if (duplicate) {
                std::cout << "Duplicate: card " << index << " already selected.\n";
                invalidInput = true;
                break;
            }

            if ((int)tempChosen.size() >= 5) {
                std::cout << "Max 5 cards. Extra ignored.\n";
                break;
            }

            tempChosen.push_back(hand.cards[index]);
        }

        if (invalidInput || tempChosen.empty()) {
            std::cout << "Please re-enter your selection.\n";
            continue;
        }

        chosenHand.cards = tempChosen;
        break;
    }

    std::cout << "Chosen: ";
    for (const auto& c : chosenHand.cards) std::cout << c.toString() << " ";
    std::cout << "\n";
}

Hand HandPlayer::getChosenHand() const {
    return chosenHand;
}
