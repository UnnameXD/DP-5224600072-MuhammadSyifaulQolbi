#include <iostream>
#include "./header/GameManager.h"

void GameManager::runSession() {
    std::cout << "=== Run Started ===\n";

    handGenerator.generateHand();
    handPlayer.playHand();

    int score = scoringRule.scoreHand();
    bool win = blindRule.checkBlind(score);
    int reward = rewardRule.earnMoney(win, score);

    std::cout << "Money gained: " << reward << "\n";
    std::cout << "=== Run Ended ===\n";
}