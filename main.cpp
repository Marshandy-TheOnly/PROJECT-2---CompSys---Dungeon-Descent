#include "GameCore.hpp"
#include <memory>

int main() {
    while (true) {
        // 0=New Game  1=Continue  2=Quit
        int menuChoice = UIManager::showMainMenu();
        if (menuChoice == 2) return 0;

        unique_ptr<Player> player;
        int startStage = 0;

        if (menuChoice == 1) {
            // --- CONTINUE: try loading each class save ---
            bool loaded = false;
            for (int c = 1; c <= 4; c++) {
                string filename = SaveManager::saveFileName(c);
                if (SaveManager::saveExists(filename)) {
                    try {
                        player     = SaveManager::load(filename);
                        startStage = player->getCurrentStage();
                        loaded     = true;
                        break;
                    } catch (CorruptSaveException&) {
                        UIManager::showOpportunityEvent(
                            "Corrupt Save",
                            "Save file was corrupt — returning to menu.");
                    }
                }
            }
            if (!loaded) {
                UIManager::showOpportunityEvent(
                    "No Save Found",
                    "No save file found — returning to menu.");
                continue; // back to main menu, do NOT start a new game
            }
        }

        if (menuChoice == 0) {
            // --- NEW GAME: pick class ---
            int classChoice = UIManager::showClassSelect();
            switch (classChoice) {
                case 1:  player = make_unique<Prince>("Prince");       break;
                case 2:  player = make_unique<Priest>("Priest");       break;
                case 3:  player = make_unique<Berserker>("Berserker"); break;
                default: player = make_unique<Mage>("Mage");           break;
            }
            startStage = 0;
        }

        StageManager manager;
        manager.generateStages();
        manager.runGame(*player, startStage);
        // When runGame returns (win or lose), loop back to main menu
    }
}
