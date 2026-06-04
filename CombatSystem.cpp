#include "GameCore.hpp"

CombatSystem::CombatSystem(Player& p, Monster& m) 
    : hero(p), enemy(m), isPlayerTurn(true), battleLog("A dangerous beast stands before you!") {
    // Trước khi combat bắt đầu, xóa hết mọi Debuff kỹ năng của trận trước
    hero.resetCombatModifiers();
    enemy.resetCombatModifiers();
}

void CombatSystem::playerAttack() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    
    battleLog = hero.attack(enemy);
    isPlayerTurn = false; // Khóa lượt người chơi ngay lập tức
}

void CombatSystem::playerSkill1() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    
    std::string result = hero.useSkill1(enemy);
    battleLog = result;
    
    // Nếu dùng chiêu thành công (không bị trả về chuỗi báo thiếu mana) thì mới mất lượt
    if (result != "Not enough Mana!") {
        isPlayerTurn = false;
    }
}

void CombatSystem::playerSkill2() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    
    std::string result = hero.useSkill2(enemy);
    battleLog = result;
    
    if (result != "Too weak to offer Blood Sacrifice!" && result != "Not enough Mana!") {
        isPlayerTurn = false; 
    }
}

void CombatSystem::playerUseItem(size_t index) {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    
    if (index < hero.getInventory().size()) {
        battleLog = hero.useBagItem(index);
        isPlayerTurn = false; // SỬ DỤNG VẬT PHẨM TRONG TRẬN: TỐN 1 LƯỢT ĐÁNH CHÍNH XÁC THEO LUẬT RPG!
    } else {
        battleLog = "Invalid item slot!";
    }
}

void CombatSystem::executeEnemyTurn() {
    if (isPlayerTurn || !hero.isAlive()) return;

    // Nếu quái vật chết do đòn đánh vừa rồi của người chơi, chặn phản công từ cõi chết
    if (!enemy.isAlive()) {
        battleLog += "\n> The monster crumbles into dust!";
        return; 
    }

    // Quái vật cào người chơi
    battleLog += "\n" + enemy.attack(hero);
    
    // Trả lại quyền điều khiển cho người chơi ở turn kế tiếp
    isPlayerTurn = true; 
}

bool CombatSystem::isCombatOver() const {
    return (!hero.isAlive() || !enemy.isAlive());
}

bool CombatSystem::isHeroVictorious() const {
    return (hero.isAlive() && !enemy.isAlive());
}