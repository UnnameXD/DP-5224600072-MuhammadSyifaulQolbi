#include <iostream>
#include "../header/GameManager.h"
#include "../header/Hand.h"
#include "../header/HandGenerator.h"

void GameManager::runSession() {
    std::cout << "\n=== Run Started ===\n";

    Hand hand = handGenerator.generateHand(); // generate once
    handPlayer.playHand(hand);                // pass same hand to player
    Hand chosen = handPlayer.getChosenHand();
    int score = scoringRule.scoreHand(chosen);  // score the same hand
    bool win = blindRule.checkBlind(score);
    int reward = rewardRule.earnMoney(win, score);

    std::cout << "Money gained: " << reward << "\n";
    std::cout << "=== Run Ended ===\n\n";
}
