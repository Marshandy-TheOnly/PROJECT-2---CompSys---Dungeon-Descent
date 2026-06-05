
#ifndef OPPORTUNITY_HPP
#define OPPORTUNITY_HPP

#include "Enemy.hpp"
#include <random>

// Structural Item Definition for inventory storage arrays
struct Item {
    std::string name;
    std::string modifierType; // "HEAL", "ATK_BUFF", "MANA_RESTORE"
    int effectValue;
};

// Abstract Stage Base Interface to allow polymorphism inside the StageManager vector
class Stage {
public:
    virtual ~Stage() = default;
    virtual void execute(Character& player, std::vector<Item>& inventory) = 0;
};

// Concrete Opportunity Stage Module (Item Cache, Fountains, Risks)
class OpportunityStage : public Stage {
private:
    int localizedEventType;

public:
    OpportunityStage() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 2);
        localizedEventType = distr(gen); // Random selection between 3 distinct events
    }

    void execute(Character& player, std::vector<Item>& inventory) override {
        std::cout << "\n==================================================\n";
        std::cout << "✨ OPPORTUNITY EVENT: A safe sanctuary zone appears...\n";
        std::cout << "==================================================\n";

        switch (localizedEventType) {
            case 0: // Event Variant A: The Healing Pool
                std::cout << "You encounter a sacred Healing Fountain nestled in the stone floor.\n";
                std::cout << "Pure energy completely cleanses all active negative status ailments!\n";
                player.healFull();
                player.resetCombatModifiers();
                std::cout << "❤️ HP and Mana completely restored. Current HP: " << player.getHP() << "/" << player.getMaxHP() << "\n";
                break;

            case 1: // Event Variant B: The High-Risk Shrine Gamble
                {
                    std::cout << "A pulsating, volatile Forbidden Altar beckons you.\n";
                    std::cout << "Do you offer your life force to gain permanent combat power? (1: Yes, 2: No): ";
                    int selection = 2;
                    std::cin >> selection;

                    if (selection == 1) {
                        std::random_device rd;
                        std::mt19937 engine(rd());
                        std::uniform_real_distribution<> probability(0.0, 1.0);

                        if (probability(engine) > 0.35) { // 65% Chance of success
                            std::cout << "🔥 SUCCESS! The altar empowers you. You gain permanent combat stats (+15 HP, +4 ATK, +2 DEF)!\n";
                            player.permanentlyBuff(15, 4, 2);
                        } else { // 35% Failure penalty
                            std::cout << "💀 BACKFIRE! The altar rejects your essence. You take catastrophic damage (-30 HP)!\n";
                            player.setHP(player.getHP() - 30);
                            std::cout << "Current Player HP: " << player.getHP() << "/" << player.getMaxHP() << "\n";
                        }
                    } else {
                        std::cout << "You prudently ignore the dark whispers and move forward safely.\n";
                    }
                }
                break;

            case 2: // Event Variant C: Ancient Cache Loot drop
                std::cout << "You stumble upon the lost satchel of a veteran dungeon explorer.\n";
                Item foundPotion{"High-Grade Elixir", "HEAL", 50};
                inventory.push_back(foundPotion);
                std::cout << "🎒 Item Acquired: [" << foundPotion.name << "] added directly to your inventory vector!\n";
                break;
        }
        std::cout << "==================================================\n";
    }
};

#endif
