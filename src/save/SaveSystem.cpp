#include "../entities/Player.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <exception>

using namespace std;

// ============================================================================
// FILE I/O IMPLEMENTATION
// Format: CSV for compact storage and easy spreadsheet import
// Location: "save.csv" in current working directory
// ============================================================================

const string SAVE_FILE = "save.csv";

// ────────────────────────────────────────────────────────────────────────────
// SAVE: Write hero state to CSV file
// Format: name,classType,hp,maxHp,attackPower,defense,skillLevel0-3,currentStage
// ────────────────────────────────────────────────────────────────────────────
void saveGame(const Player& hero) {
    ofstream file(SAVE_FILE);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open save file for writing.\n";
        return;
    }

    // Header row
    file << "name,classType,hp,maxHp,attackPower,defense,skillLevel0,skillLevel1,skillLevel2,skillLevel3,currentStage\n";

    // Data row
    file << hero.getName() << ","
         << hero.getClassType() << ","
         << hero.getHP() << ","
         << hero.getMaxHP() << ","
         << hero.getAttackPower() << ","
         << hero.getDefense() << ",";

    for (int i = 0; i < 4; i++) {
        file << hero.getSkillLevel(i);
        if (i < 3) file << ",";
    }
    file << ","
         << hero.getCurrentStage() << "\n";

    file.close();
    cout << "Game saved successfully.\n";
}

// ────────────────────────────────────────────────────────────────────────────
// LOAD: Read hero state from CSV file and restore into hero object
// CSV format: name,classType,hp,maxHp,attackPower,defense,skillLevels[4],currentStage
// ────────────────────────────────────────────────────────────────────────────
bool loadGame(Player& hero) {
    ifstream file(SAVE_FILE);
    if (!file.is_open()) {
        return false;  // Save file doesn't exist
    }

    string header, data;
    getline(file, header);  // Skip header row
    getline(file, data);    // Read data row

    if (data.empty()) {
        file.close();
        return false;  // File is empty
    }

    // Parse CSV: split by commas
    stringstream ss(data);
    string token;
    vector<string> fields;

    while (getline(ss, token, ',')) {
        fields.push_back(token);
    }

    // Validate we have all fields (11 total)
    if (fields.size() < 11) {
        file.close();
        return false;
    }

    // Extract and convert fields (robust against corrupt saves)
    try {
        const int classType = stoi(fields[1]);
        const int hp        = stoi(fields[2]);
        const int maxHp     = stoi(fields[3]);
        const int atk       = stoi(fields[4]);
        const int def       = stoi(fields[5]);
        const int stage     = stoi(fields[10]);

        const int savedSkillLevels[4] = {
            stoi(fields[6]), stoi(fields[7]), stoi(fields[8]), stoi(fields[9])
        };

        // Prevent loading a save into the wrong hero subclass.
        if (hero.getClassType() != classType) return false;

        hero.setMaxHP(maxHp);
        hero.setHP(hp);
        hero.setAttackPower(atk);
        hero.setDefense(def);
        hero.setCurrentStage(stage);

        // Player has no direct skill-level setter; bring skills up to the saved level.
        for (int i = 0; i < 4; ++i) {
            if (savedSkillLevels[i] < 1 || savedSkillLevels[i] > 5) return false;
            while (hero.getSkillLevel(i) < savedSkillLevels[i]) hero.levelUpSkill(i);
            if (hero.getSkillLevel(i) != savedSkillLevels[i]) return false;
        }

        cout << "Game loaded successfully.\n";
        return true;
    } catch (const exception&) {
        return false;
    }
}

// ────────────────────────────────────────────────────────────────────────────
// UTILITY: Check if save exists
// ────────────────────────────────────────────────────────────────────────────
bool saveExists() {
    ifstream file(SAVE_FILE);
    return file.is_open();
}

// ────────────────────────────────────────────────────────────────────────────
// UTILITY: Delete save file (new game)
// ────────────────────────────────────────────────────────────────────────────
void deleteSave() {
    if (remove(SAVE_FILE.c_str()) == 0) {
        cout << "Save file deleted.\n";
    }
}
