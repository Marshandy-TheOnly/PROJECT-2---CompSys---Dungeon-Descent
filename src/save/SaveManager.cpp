#include "SaveManager.hpp"
#include "../entities/Prince.hpp"
#include "../entities/Priest.hpp"
#include "../entities/Berserker.hpp"
#include "../entities/Mage.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>

using namespace std;

// ============================================================================
// SAVE — Write hero state + stage layout to CSV
// Format: name,classType,hp,maxHp,attackPower,defense,
//         skill0,skill1,skill2,skill3,currentStage,
//         stageType0,...,stageType9
// ============================================================================
void SaveManager::save(const Player& player, const string& filename,
                       const vector<int>& stageTypes) {
    ofstream file(filename);
    if (!file.is_open())
        throw runtime_error("Could not open save file for writing: " + filename);

    // Header row
    file << "name,classType,hp,maxHp,attackPower,defense,"
         << "skillLevel0,skillLevel1,skillLevel2,skillLevel3,currentStage,"
         << "type0,type1,type2,type3,type4,type5,type6,type7,type8,type9\n";

    // Player data
    file << player.getName()        << ","
         << player.getClassType()   << ","
         << player.getHP()          << ","
         << player.getMaxHP()       << ","
         << player.getAttackPower() << ","
         << player.getDefense()     << ",";

    for (int i = 0; i < 4; i++) {
        file << player.getSkillLevel(i);
        if (i < 3) file << ",";
    }

    file << "," << player.getCurrentStage();

    // Stage layout (10 type integers)
    for (int i = 0; i < 10 && i < (int)stageTypes.size(); i++)
        file << "," << stageTypes[i];

    file << "\n";
}

// ============================================================================
// LOAD — Read CSV, construct the correct subclass, restore all stats
// ============================================================================
unique_ptr<Player> SaveManager::load(const string& filename) {
    ifstream file(filename);
    if (!file.is_open())
        throw SaveNotFoundException("No save file found: " + filename);

    string header, data;
    getline(file, header);
    getline(file, data);

    if (data.empty())
        throw CorruptSaveException("Save file is empty: " + filename);

    // Parse CSV fields
    stringstream ss(data);
    string token;
    vector<string> fields;
    while (getline(ss, token, ','))
        fields.push_back(token);

    if (fields.size() < 11)
        throw CorruptSaveException("Save file has missing fields: " + filename);

    try {
        const string name      = fields[0];
        const int classType    = stoi(fields[1]);
        const int hp           = stoi(fields[2]);
        const int maxHp        = stoi(fields[3]);
        const int atk          = stoi(fields[4]);
        const int def          = stoi(fields[5]);
        const int stage        = stoi(fields[10]);

        const int savedSkills[4] = {
            stoi(fields[6]), stoi(fields[7]), stoi(fields[8]), stoi(fields[9])
        };

        // Validate skill levels
        for (int i = 0; i < 4; i++)
            if (savedSkills[i] < 1 || savedSkills[i] > 5)
                throw CorruptSaveException("Corrupt skill level in save: " + filename);

        // Construct the correct subclass from classType
        unique_ptr<Player> player;
        switch (classType) {
            case 1:  player = make_unique<Prince>(name);     break;
            case 2:  player = make_unique<Priest>(name);     break;
            case 3:  player = make_unique<Berserker>(name);  break;
            case 4:  player = make_unique<Mage>(name);       break;
            default: throw CorruptSaveException("Unknown class type in save: " + filename);
        }

        // Restore stats
        player->setMaxHP(maxHp);
        player->setHP(hp);
        player->setAttackPower(atk);
        player->setDefense(def);
        player->setCurrentStage(stage);

        // Bring skills up to saved level (each starts at 1)
        for (int i = 0; i < 4; i++)
            while (player->getSkillLevel(i) < savedSkills[i])
                player->levelUpSkill(i);

        return player;

    } catch (const CorruptSaveException&) {
        throw;
    } catch (const exception&) {
        throw CorruptSaveException("Failed to parse save file: " + filename);
    }
}

// ============================================================================
// HELPERS
// ============================================================================
bool SaveManager::saveExists(const string& filename) {
    ifstream file(filename);
    return file.is_open();
}

void SaveManager::deleteSave(const string& filename) {
    remove(filename.c_str()); // std::remove from <cstdio> — already included
}

// Returns the 10 stage-type integers from fields[11..20].
// Returns an empty vector if the save predates layout storage (old format).
vector<int> SaveManager::loadStageTypes(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return {};

    string header, data;
    getline(file, header);
    getline(file, data);
    if (data.empty()) return {};

    stringstream ss(data);
    string token;
    vector<string> fields;
    while (getline(ss, token, ','))
        fields.push_back(token);

    if (fields.size() < 21) return {};   // old format — no layout stored

    vector<int> types;
    try {
        for (int i = 11; i < 21; i++)
            types.push_back(stoi(fields[i]));
    } catch (...) {
        return {};
    }
    return types;
}

string SaveManager::saveFileName(int /*classType*/) {
    // Single save file regardless of class — eliminates multi-file collision bugs.
    return "save.csv";
}
