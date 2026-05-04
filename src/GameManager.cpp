#include <iostream>
#include "../header/GameManager.h"
#include "../header/Hand.h"
#include "../header/HandGenerator.h"

void GameManager::runSession() {
    std::cout << "\n=== Run Started ===\n";

    // Step 1: Generate Cards / chosen hand
    Hand hand = handGenerator.generateHand(); 
    
    // Step 2: Player plays selected hand
    handPlayer.playHand(hand);    
    
    // Step 3: Calculate base score
    Hand chosen = handPlayer.getChosenHand();

    // Step 4: Modify Joker Observer

    // Step 5: Joker modifies score

    // Step 6: Check win/lose
    int score = scoringRule.scoreHand(chosen);  
    bool win = blindRule.checkBlind(score);

    // Step 7: Calculate reward
    int reward = rewardRule.earnMoney(win, score);
    std::cout << "Money gained: " << reward << "\n";
    std::cout << "=== Run Ended ===\n\n";
}
