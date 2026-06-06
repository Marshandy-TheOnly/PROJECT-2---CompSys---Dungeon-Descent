#include "UIManager.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;
using namespace std;

// ============================================================================
// HELPERS
// ============================================================================

static Color hpColor(int hp, int maxHp) {
    float r = (float)hp / maxHp;
    if (r > 0.6f) return Color::Green;
    if (r > 0.3f) return Color::Yellow;
    return Color::Red;
}

static Element hpBar(const string& label, int hp, int maxHp) {
    float ratio = max(0.0f, (float)hp / maxHp);
    return hbox({
        text(label + ": ") | bold,
        gauge(ratio) | color(hpColor(hp, maxHp)) | flex,
        text(" " + to_string(hp) + "/" + to_string(maxHp)) | bold,
    });
}

// ============================================================================
// MAIN MENU
// ============================================================================

int UIManager::showMainMenu() {
    auto screen = ScreenInteractive::Fullscreen();
    vector<string> entries = { "New Game", "Continue", "Quit" };
    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto component = CatchEvent(menu, [&](Event e) {
        if (e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
            text("DUNGEON DESCENT") | bold | color(Color::Red) | center,
            text("A turn-based dungeon crawler") | dim | center,
            separator(),
            component->Render(),
        }) | border | center;
    });

    screen.Loop(renderer);
    return selected; // 0=New Game  1=Continue  2=Quit
}

// ============================================================================
// CLASS SELECT
// ============================================================================

int UIManager::showClassSelect() {
    auto screen = ScreenInteractive::Fullscreen();
    vector<string> classes = { "Prince", "Priest", "Berserker", "Mage" };
    int selected = 0;
    auto menu = Menu(&classes, &selected);

    vector<vector<string>> info = {
        { "HP: 150  ATK: 18  DEF: 10",
          "Balanced all-rounder.",
          "Best base block (15).",
          "Ultimate: 3x ATK — reliable." },
        { "HP: 120  ATK: 12  DEF: 6",
          "Sustain specialist.",
          "Defend also heals 15 HP.",
          "Ultimate: Heal 30 HP + 2x ATK." },
        { "HP: 180  ATK: 22  DEF: 5",
          "Highest HP and ATK.",
          "Ultimate sacrifices 40 HP.",
          "Ultimate: 4x ATK — high risk." },
        { "HP: 100  ATK: 10  DEF: 4",
          "Glass cannon.",
          "Squishiest — dies fast if hit.",
          "Ultimate: 5x ATK — devastating." },
    };

    auto component = CatchEvent(menu, [&](Event e) {
        if (e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    auto renderer = Renderer(component, [&] {
        Elements details;
        for (auto& line : info[selected])
            details.push_back(text(line));

        return vbox({
            text("SELECT YOUR CLASS") | bold | color(Color::Yellow) | center,
            separator(),
            hbox({
                vbox({ component->Render() }) | border | flex,
                vbox(details) | border | flex,
            }),
            text("Press Enter to confirm") | dim | center,
        }) | border;
    });

    screen.Loop(renderer);
    return selected + 1; // 1=Prince  2=Priest  3=Berserker  4=Mage
}

// ============================================================================
// COMBAT SCREEN — called every turn
// ============================================================================

int UIManager::getPlayerAction(Player& player, Monster& enemy,
                               const vector<string>& log) {
    auto screen = ScreenInteractive::Fullscreen();
    vector<string> actions = {
        "Attack",
        "Defend",
        player.canDodge() ? "Dodge" : "Dodge  [on cooldown]",
        "Ultimate",
    };
    int selected = 0;
    auto menu = Menu(&actions, &selected);

    auto component = CatchEvent(menu, [&](Event e) {
        if (e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    auto renderer = Renderer(component, [&] {
        // Last 5 log lines
        Elements logLines;
        int start = max(0, (int)log.size() - 5);
        for (int i = start; i < (int)log.size(); i++)
            logLines.push_back(text(log[i]));
        if (logLines.empty())
            logLines.push_back(text("Combat begins!") | dim);

        // Player sprite
        Elements playerSpriteEl;
        for (auto& line : getPlayerSprite(player.getClassType()))
            playerSpriteEl.push_back(text(line) | color(Color::Green));

        // Enemy sprite
        Elements enemySpriteEl;
        for (auto& line : getMonsterSprite(enemy.getName()))
            enemySpriteEl.push_back(text(line) | color(Color::Red));

        return vbox({
            text("DUNGEON DESCENT") | bold | color(Color::Red) | center,
            separator(),
            hbox({
                // Player panel
                vbox({
                    vbox(playerSpriteEl) | center,
                    separator(),
                    text(player.getName()) | bold | color(Color::Green),
                    hpBar("HP", player.getHP(), player.getMaxHP()),
                    text("ATK Lv"   + to_string(player.getSkillLevel(0)) +
                         "  DEF Lv"   + to_string(player.getSkillLevel(1)) +
                         "  DODGE Lv" + to_string(player.getSkillLevel(2)) +
                         "  ULT Lv"   + to_string(player.getSkillLevel(3))) | dim,
                }) | border | flex,
                // Enemy panel
                vbox({
                    vbox(enemySpriteEl) | center,
                    separator(),
                    text(enemy.getName()) | bold | color(Color::Red),
                    hpBar("HP", enemy.getHP(), enemy.getMaxHP()),
                }) | border | flex,
            }),
            // Battle log
            vbox(logLines) | border,
            // Action menu
            text("Choose your action:") | bold,
            component->Render() | border,
        }) | border;
    });

    screen.Loop(renderer);
    return selected + 1; // 1=Attack  2=Defend  3=Dodge  4=Ultimate
}

// ============================================================================
// SKILL LEVEL UP
// ============================================================================

int UIManager::showSkillLevelUp(Player& player, const string& title) {
    auto screen = ScreenInteractive::Fullscreen();

    auto lvStr = [&](int idx) { return to_string(player.getSkillLevel(idx)); };
    auto nxStr = [&](int idx) { return to_string(player.getSkillLevel(idx) + 1); };

    vector<string> skills = {
        "ATK    Lv " + lvStr(0) + " -> Lv " + nxStr(0) + "   (+3 dmg per level)",
        "DEF    Lv " + lvStr(1) + " -> Lv " + nxStr(1) + "   (+4 block per level)",
        "DODGE  Lv " + lvStr(2) + " -> Lv " + nxStr(2) + "   (+5% success chance)",
        "ULT    Lv " + lvStr(3) + " -> Lv " + nxStr(3) + "   (-5% miss chance)",
    };
    int selected = 0;
    auto menu = Menu(&skills, &selected);

    auto component = CatchEvent(menu, [&](Event e) {
        if (e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
            text(title) | bold | color(Color::Yellow) | center,
            text("Choose a skill to level up:") | center,
            separator(),
            component->Render(),
            separator(),
            text("Press Enter to confirm") | dim | center,
        }) | border;
    });

    screen.Loop(renderer);
    return selected; // 0=ATK  1=DEF  2=DODGE  3=ULT
}

// ============================================================================
// CONFIRM PROMPT (Yes / No)
// ============================================================================

bool UIManager::showConfirm(const string& prompt) {
    auto screen = ScreenInteractive::Fullscreen();
    vector<string> choices = { "Yes", "No" };
    int selected = 1; // Default: No
    auto menu = Menu(&choices, &selected);

    auto component = CatchEvent(menu, [&](Event e) {
        if (e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
            text(prompt) | bold | center,
            separator(),
            component->Render(),
        }) | border | center;
    });

    screen.Loop(renderer);
    return selected == 0; // true = Yes
}

// ============================================================================
// OPPORTUNITY EVENT DISPLAY
// ============================================================================

void UIManager::showOpportunityEvent(const string& eventName, const string& desc) {
    auto screen = ScreenInteractive::Fullscreen();
    bool ready = false;

    auto component = CatchEvent(Renderer([&] {
        ready = true;
        return vbox({
            text("OPPORTUNITY EVENT") | bold | color(Color::Yellow) | center,
            separator(),
            text(eventName) | bold | center,
            text(desc) | center,
            separator(),
            text("Press Enter to continue") | dim | center,
        }) | border;
    }), [&](Event e) {
        if (ready && e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    screen.Loop(component);
}

// ============================================================================
// STAGE TRANSITION BANNER
// ============================================================================

void UIManager::showStageTransition(int stageNum, StageType type) {
    auto screen = ScreenInteractive::Fullscreen();
    bool ready = false;

    string typeStr;
    Color  typeColor = Color::White;
    switch (type) {
        case StageType::MONSTER:     typeStr = "MONSTER";     typeColor = Color::Red;    break;
        case StageType::OPPORTUNITY: typeStr = "OPPORTUNITY"; typeColor = Color::Green;  break;
        case StageType::MINI_BOSS:   typeStr = "MINI-BOSS";   typeColor = Color::Yellow; break;
        case StageType::FINAL_BOSS:  typeStr = "FINAL BOSS";  typeColor = Color::Red;    break;
    }

    auto component = CatchEvent(Renderer([&] {
        ready = true;
        return vbox({
            text("STAGE " + to_string(stageNum) + " / 10") | bold | center,
            text(typeStr) | bold | color(typeColor) | center,
            separator(),
            text("Press Enter to begin") | dim | center,
        }) | border;
    }), [&](Event e) {
        if (ready && e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    screen.Loop(component);
}

// ============================================================================
// VICTORY
// ============================================================================

void UIManager::showVictory(Player& player) {
    auto screen = ScreenInteractive::Fullscreen();
    bool ready = false;

    auto component = CatchEvent(Renderer([&] {
        ready = true;
        return vbox({
            text("YOU WIN!") | bold | color(Color::Yellow) | center,
            text("The dungeon has been cleared!") | center,
            separator(),
            hpBar("HP", player.getHP(), player.getMaxHP()),
            text("Skills — ATK:" + to_string(player.getSkillLevel(0)) +
                 "  DEF:"  + to_string(player.getSkillLevel(1)) +
                 "  DODGE:" + to_string(player.getSkillLevel(2)) +
                 "  ULT:"  + to_string(player.getSkillLevel(3))) | center,
            separator(),
            text("Press Enter to exit") | dim | center,
        }) | border;
    }), [&](Event e) {
        if (ready && e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    screen.Loop(component);
}

// ============================================================================
// GAME OVER
// ============================================================================

void UIManager::showGameOver() {
    auto screen = ScreenInteractive::Fullscreen();
    bool ready = false;

    auto component = CatchEvent(Renderer([&] {
        ready = true;
        return vbox({
            text("GAME OVER") | bold | color(Color::Red) | center,
            text("You have fallen in the dungeon...") | center,
            separator(),
            text("Press Enter to exit") | dim | center,
        }) | border;
    }), [&](Event e) {
        if (ready && e == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false;
    });

    screen.Loop(component);
}
