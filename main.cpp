#include "GameCore.hpp"
#include <iostream>
#include <memory>

// ----------------------------------------------------------------------------
// Run one combat encounter to completion, auto-cycling turns.
// In the full game the UI hands control to the player each turn.
// ----------------------------------------------------------------------------
static void runBattle(Player& hero, Monster& enemy) {
    CombatSystem battle(hero, enemy);
    cout << "\n--- Battle: " << hero.getName()
         << " vs " << enemy.getName() << " ---\n";

    while (!battle.isCombatOver()) {
        if (battle.getIsPlayerTurn()) battle.playerAttack();
        else                          battle.executeEnemyTurn();
    }

    cout << battle.getBattleLog();
    cout << (battle.isHeroVictorious() ? "\n>>> Victory!\n" : "\n>>> Defeated.\n");
}

int main() {
    cout << "==================================================\n";
    cout << "              DUNGEON DESCENT\n";
    cout << "==================================================\n\n";

    cout << "Choose your class:\n";
    cout << "  1. Prince    (HP:150  ATK:18  DEF:10)  Balanced, reliable ultimate\n";
    cout << "  2. Priest    (HP:120  ATK:12  DEF:6 )  Sustain — defend heals\n";
    cout << "  3. Berserker (HP:180  ATK:22  DEF:5 )  HP sacrifice ultimate\n";
    cout << "  4. Mage      (HP:100  ATK:10  DEF:4 )  Glass cannon, 5x ultimate\n";
    cout << "Your choice: ";

    int classChoice = 1;
    cin >> classChoice;

    unique_ptr<Player> hero;
    switch (classChoice) {
        case 1:  hero = make_unique<Prince>("Prince");           break;
        case 2:  hero = make_unique<Priest>("Priest");           break;
        case 3:  hero = make_unique<Berserker>("Berserker");     break;
        default: hero = make_unique<Mage>("Mage");               break;
    }

    cout << "\n>>> " << hero->getName() << " enters the dungeon.\n";

    // Stage 1: opportunity event
    OpportunityStage opp;
    opp.execute(*hero);
    if (!hero->isAlive()) { cout << "\nGame over.\n"; return 0; }

    // Stage 2: regular monster fight
    Monster goblin("Goblin", 2);
    runBattle(*hero, goblin);
    if (!hero->isAlive()) { cout << "\nGame over.\n"; return 0; }
    hero->levelUpSkill(0);

    // Stage 3: boss
    Boss warden = makeWarden();
    runBattle(*hero, warden);

    cout << "\n==================================================\n";
    cout << "  Run complete. Final HP: "
         << hero->getHP() << "/" << hero->getMaxHP() << "\n";
    cout << "==================================================\n";
    return 0;
}
