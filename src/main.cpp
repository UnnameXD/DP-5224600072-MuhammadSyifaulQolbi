#include <cstdlib>
#include <ctime>
#include "../header/GameManager.h"

int main() {
    srand(time(nullptr)); // seed randomizer
    GameManager gameManager;
    gameManager.runSession();
    return 0;
}
