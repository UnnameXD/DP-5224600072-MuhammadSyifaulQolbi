#include <iostream>
#include <sstream>
#include "../header/HandPlayer.h"

void HandPlayer::playHand(Hand& hand) {
    chosenHand.cards.clear();

    std::cout << "\nYour Hand:\n";
    for (int i = 0; i < hand.cards.size(); i++) {
        std::cout << i << ": " << hand.cards[i].toString() << "\n";
    }

    while (true) {
        std::cout << "\nChoose up to 5 cards (example: 1 3 4 6)\n";
        std::cout << "Input: ";

        std::string line;
        std::getline(std::cin >> std::ws, line);  

        std::stringstream ss(line);
        std::vector<Card> tempChosen;
        bool invalidInput = false;
        std::string token;

        while (ss >> token) {
            try {
                int index = std::stoi(token);

                if (index < 0 || index >= hand.cards.size()) {
                    std::cout << "Invalid index: " << index << "\n";
                    invalidInput = true;
                    break;
                }

                // duplicate check
                bool duplicate = false;
                for (const auto& c : tempChosen) {
                    if (c.rank == hand.cards[index].rank &&
                        c.suit == hand.cards[index].suit) {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate) {
                    std::cout << "Duplicate card: " << index << "\n";
                    invalidInput = true;
                    break;
                }

                if (tempChosen.size() >= 5) {
                    std::cout << "Max 5 cards only!\n";
                    break;
                }

                tempChosen.push_back(hand.cards[index]);

            } catch (...) {
                // non-number input (like "a")
                std::cout << "Invalid input: " << token << "\n";
                invalidInput = true;
                break;
            }
        }

        // restart if any error
        if (invalidInput || tempChosen.empty()) {
            std::cout << "Please re-enter your selection.\n";
            continue;
        }

        // valid → save result
        chosenHand.cards = tempChosen;
        break;
    }

    std::cout << "\nFinal Chosen Cards:\n";
    for (const auto& c : chosenHand.cards) {
        std::cout << c.toString() << " ";
    }
    std::cout << "\n";
}

Hand HandPlayer::getChosenHand() const {
    return chosenHand;
}