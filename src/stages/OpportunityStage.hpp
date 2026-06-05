#pragma once
#include "Stage.hpp"
#include <iostream>
#include <random>

// ============================================================================
// OPPORTUNITY STAGE — 3 random safe-zone events
// Events are selected randomly using mt19937 on construction.
//
// Event 0 — Healing Pool:
//   Fully restores HP and resets all combat modifiers.
//
// Event 1 — Forbidden Altar (Gamble):
//   65% success: permanently +15 HP, +4 ATK, +2 DEF
//   35% failure: take 30 damage
//
// Event 2 — Ancient Cache:
//   Discover a hidden cache. Permanently gain +10 HP.
// ============================================================================
class OpportunityStage : public Stage {
private:
    int eventType;
    inline static mt19937 rng{ random_device{}() };

public:
    OpportunityStage() {
        uniform_int_distribution<int> pick(0, 2);
        eventType = pick(rng);
    }

    StageType getType() const override { return StageType::OPPORTUNITY; }

    void execute(Player& player) override {
        cout << "\n==================================================\n";
        cout << "  OPPORTUNITY EVENT: A safe sanctuary appears...\n";
        cout << "==================================================\n";

        switch (eventType) {

            case 0: { // Healing Pool — full restore
                player.healFull();
                cout << "  Healing Pool: Pure energy washes over you.\n";
                cout << "  HP fully restored: "
                     << player.getHP() << "/" << player.getMaxHP() << "\n";
                break;
            }

            case 1: { // Forbidden Altar — 65/35 gamble
                cout << "  Forbidden Altar: Dark power pulses before you.\n";
                cout << "  Offer your life force for a chance at power? (1=Yes / 2=No): ";
                int choice = 2;
                cin >> choice;

                if (choice == 1) {
                    uniform_real_distribution<float> roll(0.0f, 1.0f);
                    if (roll(rng) > 0.35f) {
                        player.permanentlyBuff(15, 4, 2);
                        cout << "  SUCCESS! The altar empowers you.\n";
                        cout << "  Gained: +15 Max HP, +4 ATK, +2 DEF\n";
                    } else {
                        player.setHP(player.getHP() - 30);
                        cout << "  BACKFIRE! The altar rejects your essence.\n";
                        cout << "  Lost 30 HP. Current HP: "
                             << player.getHP() << "/" << player.getMaxHP() << "\n";
                    }
                } else {
                    cout << "  You step back from the altar.\n";
                }
                break;
            }

            case 2: { // Ancient Cache — permanent HP boost
                player.permanentlyBuff(10, 0, 0);
                cout << "  Ancient Cache: A veteran explorer's hidden stash!\n";
                cout << "  Discovered supplies. Max HP permanently +10.\n";
                cout << "  Current HP: "
                     << player.getHP() << "/" << player.getMaxHP() << "\n";
                break;
            }
        }
        cout << "==================================================\n";
    }
};
