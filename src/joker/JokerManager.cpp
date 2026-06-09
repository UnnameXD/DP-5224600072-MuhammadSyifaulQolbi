#include <iostream>
#include "joker/JokerManager.h"

bool JokerManager::addJoker(std::unique_ptr<Joker> joker) {
    // Negative jokers each add +1 slot
    int negativeCount = 0;
    for (const auto& j : jokers)
        if (j->isNegative()) negativeCount++;
    int effectiveMax = MAX_JOKERS + negativeCount;
    if ((int)jokers.size() >= effectiveMax) {
        std::cout << "Joker slots full! Max " << effectiveMax << " jokers.\n";
        return false;
    }
    std::cout << "Equipped: " << joker->getDisplayName()
              << " -- " << joker->getDescription()
              << joker->getEditionDescription() << "\n";
    jokers.push_back(std::move(joker));
    return true;
}

bool JokerManager::removeJoker(int index) {
    if (index < 0 || index >= (int)jokers.size()) return false;
    std::cout << "Sold: " << jokers[index]->getDisplayName() << "\n";
    jokers.erase(jokers.begin() + index);
    return true;
}

int JokerManager::jokerCount() const { return (int)jokers.size(); }

void JokerManager::printJokers() const {
    if (jokers.empty()) { std::cout << "  (no jokers)\n"; return; }
    for (int i = 0; i < (int)jokers.size(); i++) {
        std::cout << "  [" << i << "] "
                  << jokers[i]->getDisplayName()
                  << " -- " << jokers[i]->getDescription()
                  << jokers[i]->getEditionDescription() << "\n";
    }
}

const std::vector<std::unique_ptr<Joker>>& JokerManager::getJokers() const {
    return jokers;
}

void JokerManager::applyJokers(ScoreContext& context, const Hand& hand) {
    // Pass 1: additive (+chips, +mult) + foil/holo editions
    for (auto& j : jokers) {
        j->applyAdditive(context, hand);
        j->applyEditionAdditive(context);
    }
    // Pass 2: multiplicative (xmult) + polychrome editions
    for (auto& j : jokers) {
        j->applyMultiplicative(context, hand);
        j->applyEditionMultiplicative(context);
    }
}
