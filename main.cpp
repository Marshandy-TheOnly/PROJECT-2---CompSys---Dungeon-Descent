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
            // --- CONTINUE: single save file ---
            string filename = SaveManager::saveFileName(0);
            if (!SaveManager::saveExists(filename)) {
                UIManager::showOpportunityEvent(
                    "No Save Found",
                    "No save file found — returning to menu.");
                continue;
            }
            try {
                player     = SaveManager::load(filename);
                startStage = player->getCurrentStage();
            } catch (CorruptSaveException&) {
                UIManager::showOpportunityEvent(
                    "Corrupt Save",
                    "Save file was corrupt — returning to menu.");
                continue;
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

        if (menuChoice == 1) {
            // Continue: restore the exact stage layout that was saved
            string filename = SaveManager::saveFileName(0);
            vector<int> savedTypes = SaveManager::loadStageTypes(filename);
            // Sanity-check the boss slots — if a tampered/corrupt save would
            // skip a boss, fall back to a fresh layout instead.
            bool layoutValid =
                savedTypes.size() == 10 &&
                savedTypes[4] == static_cast<int>(StageType::MINI_BOSS) &&
                savedTypes[9] == static_cast<int>(StageType::FINAL_BOSS);
            if (layoutValid)
                manager.generateStagesFromTypes(savedTypes);
            else
                manager.generateStages(); // old-format or corrupt — fresh layout
        } else {
            manager.generateStages();
        }

        manager.runGame(*player, startStage);
        // When runGame returns (win or lose), loop back to main menu
    }
}
