#pragma once
#include "HandGenerator.h"
#include "HandPlayer.h"
#include "ScoringRule.h"
#include "RunSessionState.h"
#include "blind/BlindManager.h"
#include "blind/ConcreteBlindStates.h"
#include "shop/ShopManager.h"

class GameManager {
public:
    GameManager();
    void runSession();

private:
    HandGenerator  handGenerator;
    HandPlayer     handPlayer;
    ScoringRule    scoringRule;
    RunSessionState session;
    BlindManager   blindManager;
    ShopManager    shopManager;

    bool playBlind();
    void runShopPhase();
    void printStatus() const;
};
