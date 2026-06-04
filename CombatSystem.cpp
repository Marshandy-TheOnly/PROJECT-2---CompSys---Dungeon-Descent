#include "GameCore.hpp"

using namespace std; // Đã thêm đồng bộ

CombatSystem::CombatSystem(Player& p, Monster& m) 
    : hero(p), enemy(m), isPlayerTurn(true), battleLog("A new conflict begins!") {
    hero.resetCombatModifiers();
    enemy.resetCombatModifiers();
}

void CombatSystem::playerAttack() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    battleLog = hero.attack(enemy);
    isPlayerTurn = false; 
}

void CombatSystem::playerSkill1() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    string result = hero.useSkill1(enemy);
    battleLog = result;
    if (result != "Not enough Mana!") isPlayerTurn = false;
}

void CombatSystem::playerSkill2() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    string result = hero.useSkill2(enemy);
    battleLog = result;
    if (result != "Not enough Mana!" && result != "Too weak to offer Blood Sacrifice!") {
        isPlayerTurn = false; 
    }
}

void CombatSystem::playerUseItem(size_t index) {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    battleLog = hero.useBagItem(index, enemy);
    if (battleLog != "Invalid item!") isPlayerTurn = false; 
}

void CombatSystem::executeEnemyTurn() {
    if (isPlayerTurn || !hero.isAlive()) return;

    if (!enemy.isAlive()) {
        battleLog += "\n> The enemy has collapsed!";
        return;
    }

    // KIỂM TRA ĐỊCH MẤT LƯỢT (Bị đóng băng do Mage gọi chiêu 2)
    if (enemy.getStunTurns() > 0) {
        battleLog = "[TURN SKIP] " + enemy.getName() + " is frozen and skips its action!";
        
        // Vẫn chạy hàm update tick để tính sát thương độc/bỏng và trừ lượt debuff
        battleLog += enemy.updateDurationTicks();
        battleLog += hero.updateDurationTicks();
        isPlayerTurn = true; 
        return;
    }

    // GỌI BỌC SKILL SET CỦA ĐỊCH (Chừa cổng cho thành viên khác thiết kế)
    battleLog += "\n" + enemy.executeSkillSet(hero);

    // ========================================================================
    // ĐIỂM CHỐT LOGIC: CUỐI HIỆP ĐẤU
    // Lúc này quái vật ĐÃ ĐÁNH XONG. Hàm updateDurationTicks() của Người chơi 
    // chạy -> Trừ lượt debuff Agility -> Rogue nhận lại chỉ số Agility gốc an toàn!
    // ========================================================================
    battleLog += enemy.updateDurationTicks();
    battleLog += hero.updateDurationTicks();

    isPlayerTurn = true; 
}

bool CombatSystem::isCombatOver() const { return (!hero.isAlive() || !enemy.isAlive()); }
bool CombatSystem::isHeroVictorious() const { return (hero.isAlive() && !enemy.isAlive()); }