#include <iostream>
#include "../header/GameManager.h"
#include "../header/Hand.h"

void GameManager::runSession() {
    std::cout << "\n=== Run Started ===\n";

    Hand hand = handGenerator.generateHand(); // generate once
    handPlayer.playHand(hand);                // pass same hand to player

    int score = scoringRule.scoreHand(hand);  // score the same hand
    bool win = blindRule.checkBlind(score);
    int reward = rewardRule.earnMoney(win, score);

    std::cout << "Money gained: " << reward << "\n";
    std::cout << "=== Run Ended ===\n\n";
}
