#pragma once
#include "Stage.hpp"
#include "../ui/UIManager.hpp"
#include "../save/SaveManager.hpp"
#include <random>

// ============================================================================
// OPPORTUNITY STAGE — 3 random safe-zone events
// Events are selected randomly using mt19937 on construction.
//
// Event 0 — Healing Pool:
//   Fully restores HP.
//
// Event 1 — Forbidden Altar (Gamble):
//   65% success: permanently +15 HP, +4 ATK, +2 DEF
//   35% failure: take 30 damage
//
// Event 2 — Ancient Cache:
//   Discover a hidden cache. Permanently gain +10 HP (stages 1-4)
//   or +20 HP (stages 5+) — reward scales with how deep you are.
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
        switch (eventType) {

            case 0: { // Healing Pool — full restore
                player.healFull();
                UIManager::showOpportunityEvent(
                    "Healing Pool",
                    "Pure energy washes over you.\n  HP fully restored: "
                    + to_string(player.getHP()) + "/" + to_string(player.getMaxHP()));
                break;
            }

            case 1: { // Forbidden Altar — 65/35 gamble
                bool offered = UIManager::showConfirm(
                    "Forbidden Altar: Dark power pulses before you.\nOffer your life force for a chance at power?");

                if (offered) {
                    uniform_real_distribution<float> roll(0.0f, 1.0f);
                    if (roll(rng) > 0.35f) {
                        player.permanentlyBuff(15, 4, 2);
                        UIManager::showOpportunityEvent(
                            "Forbidden Altar — SUCCESS!",
                            "The altar empowers you.\n  Gained: +15 Max HP, +4 ATK, +2 DEF");
                    } else {
                        player.setHP(player.getHP() - 30);
                        string hpStatus = player.isAlive()
                            ? "Current HP: " + to_string(player.getHP()) + "/" + to_string(player.getMaxHP())
                            : "The altar consumed your last life force.";
                        UIManager::showOpportunityEvent(
                            "Forbidden Altar — BACKFIRE!",
                            "The altar rejects your essence.\n  Lost 30 HP. " + hpStatus);
                        if (!player.isAlive()) {
                            SaveManager::deleteSave(SaveManager::saveFileName(player.getClassType()));
                            UIManager::showGameOver();
                        }
                    }
                } else {
                    UIManager::showOpportunityEvent(
                        "Forbidden Altar",
                        "You step back from the altar.");
                }
                break;
            }

            case 2: { // Ancient Cache — permanent HP boost (scales with stage)
                int bonus = (player.getCurrentStage() >= 5) ? 20 : 10;
                player.permanentlyBuff(bonus, 0, 0);
                UIManager::showOpportunityEvent(
                    "Ancient Cache",
                    "A veteran explorer's hidden stash!\n  Discovered supplies. Max HP permanently +"
                    + to_string(bonus) + ".\n  Current HP: "
                    + to_string(player.getHP()) + "/" + to_string(player.getMaxHP()));
                break;
            }
        }
    }
};
