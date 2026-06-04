#ifndef GAME_CORE_HPP
#define GAME_CORE_HPP

#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

class Player;

// ============================================================================
// STEP 1: HỆ THỐNG VẬT PHẨM ĐA HÌNH & PHÂN CẤP ĐỘ (ITEM SYSTEM)
// ============================================================================
class Item {
protected:
    std::string name;
    std::string description;
    int floorLevel; // Cấp độ tầng (Tầng càng cao đồ càng mạnh)
public:
    Item(std::string n, std::string d, int lvl) : name(n), description(d), floorLevel(lvl) {}
    virtual ~Item() = default;

    // Hàm ảo thuần túy để phân loại vật phẩm
    virtual bool isInstantBoost() const = 0; 
    virtual void use(Player& player) = 0;

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
};

// --- NHÓM 1: Vật phẩm dùng TRONG TRẬN (Tốn lượt) ---
class HealthPotion : public Item {
public:
    HealthPotion(int lvl) : Item("Health Potion Mk-" + std::to_string(lvl), "Restores " + std::to_string(30 + lvl * 15) + " HP during battle.", lvl) {}
    bool isInstantBoost() const override { return false; }
    void use(Player& player) override; // Định nghĩa chi tiết ở dưới
};

class ManaPotion : public Item {
public:
    ManaPotion(int lvl) : Item("Mana Potion Mk-" + std::to_string(lvl), "Restores " + std::to_string(20 + lvl * 10) + " Mana during battle.", lvl) {}
    bool isInstantBoost() const override { return false; }
    void use(Player& player) override;
};

// --- NHÓM 2: Vật phẩm tăng THẲNG CHỈ SỐ khi chọn (Instant Boost) ---
class StatScroll : public Item {
private:
    int statType; // 1: HP, 2: ATK, 3: DEF
public:
    StatScroll(int lvl, int type) : Item("", "", lvl), statType(type) {
        if (type == 1) {
            name = "Ancient Vit Scroll (Lvl " + std::to_string(lvl) + ")";
            description = "Permanently increases Max HP by +" + std::to_string(lvl * 15) + ".";
        } else if (type == 2) {
            name = "Sharpening Stone (Lvl " + std::to_string(lvl) + ")";
            description = "Permanently increases Attack Power by +" + std::to_string(lvl * 3) + ".";
        } else {
            name = "Iron Core (Lvl " + std::to_string(lvl) + ")";
            description = "Permanently increases Defense by +" + std::to_string(lvl * 2) + ".";
        }
    }
    bool isInstantBoost() const override { return true; }
    void use(Player& player) override;
};

// ============================================================================
// STEP 2: LỚP CƠ SỞ NHÂN VẬT (CHARACTER BASE)
// ============================================================================
class Character {
protected:
    std::string name;
    int hp, maxHp, mana, maxMana;
    int attackPower, defense, intelligence, agility;
    
    // Chỉ số giảm tạm thời trong trận đấu (dùng cho các chiêu thức hi sinh chỉ số)
    int tempDefDebuff;
    int tempAgiDebuff;

public:
    Character(std::string n, int h, int m, int atk, int def, int intel, int agi) 
        : name(n), hp(h), maxHp(h), mana(m), maxMana(m), attackPower(atk), defense(def), intelligence(intel), agility(agi),
          tempDefDebuff(0), tempAgiDebuff(0) {}
    virtual ~Character() = default;

    virtual std::string attack(Character& target) = 0;

    // Getters & Setters
    std::string getName() const { return name; }
    int getHP() const { return hp; }
    int getMaxHP() const { return maxHp; }
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
    int getAttackPower() const { return attackPower; }
    
    // Tính chỉ số thực tế sau khi trừ đi Debuff trong trận
    int getDefense() const { return (defense - tempDefDebuff < 0) ? 0 : (defense - tempDefDebuff); }
    int getAgility() const { return (agility - tempAgiDebuff < 0) ? 0 : (agility - tempAgiDebuff); }
    int getIntelligence() const { return intelligence; }
    bool isAlive() const { return hp > 0; }

    void setHP(int newHp) { hp = (newHp > maxHp) ? maxHp : (newHp < 0 ? 0 : newHp); }
    void setMana(int newMana) { mana = (newMana > maxMana) ? maxMana : (newMana < 0 ? 0 : newMana); }
    
    void applyDefDebuff(int amount) { tempDefDebuff += amount; }
    void applyAgiDebuff(int amount) { tempAgiDebuff += amount; }
    
    void resetCombatModifiers() {
        tempDefDebuff = 0;
        tempAgiDebuff = 0;
    }

    // Logic Nhận sát thương, né tránh và chí mạng
    std::string takeDamage(int incomingDmg, int attackerInt, const std::string& attackerName) {
        // Tỷ lệ né = Agility thực tế % (Tối đa 80%)
        int dodgeChance = getAgility();
        if (dodgeChance > 80) dodgeChance = 80;
        
        if ((std::rand() % 100) < dodgeChance) {
            return " -> " + name + " DODGED the attack!";
        }

        // Tỷ lệ chí mạng = Intelligence % (Gấp đôi sát thương)
        bool isCrit = ((std::rand() % 100) < attackerInt);
        if (isCrit) incomingDmg *= 2;

        // Trừ phòng thủ thực tế
        int finalDmg = incomingDmg - getDefense();
        if (finalDmg < 1) finalDmg = 1; // Đòn đánh trúng tối thiểu mất 1 máu

        hp -= finalDmg;
        if (hp < 0) hp = 0;

        std::string res = " -> " + name + " took " + std::to_string(finalDmg) + " dmg" + (isCrit ? " (CRITICAL!)" : ".");
        if (getDefense() > 0) res += " (Blocked " + std::to_string(getDefense()) + ")";
        return res;
    }

    void permanentlyBuff(int h, int a, int d) {
        maxHp += h; hp += h; attackPower += a; defense += d;
    }
    
    void healFull() { 
        hp = maxHp; 
        mana = maxMana;
    }
};

// ============================================================================
// STEP 3: LỚP NGƯỜI CHƠI (PLAYER CLASS)
// ============================================================================
class Player : public Character {
protected:
    std::vector<std::unique_ptr<Item>> inventory; // Túi đồ chứa vật phẩm dùng trong trận
public:
    Player(std::string n, int h, int m, int atk, int def, int intel, int agi) 
        : Character(n, h, m, atk, def, intel, agi) {}

    // Mỗi nhân vật có đúng 2 kỹ năng chủ động riêng biệt
    virtual std::string useSkill1(Character& target) = 0;
    virtual std::string useSkill2(Character& target) = 0;
    virtual int getClassType() const = 0; 

    void addItemToBag(std::unique_ptr<Item> item) { inventory.push_back(std::move(item)); }
    const std::vector<std::unique_ptr<Item>>& getInventory() const { return inventory; }
    
    std::string useBagItem(size_t index) {
        if (index < inventory.size()) {
            std::string msg = "Used " + inventory[index]->getName();
            inventory[index]->use(*this);
            inventory.erase(inventory.begin() + index); // Dùng xong xóa khỏi túi
            return msg;
        }
        return "Invalid item!";
    }
};

// Hiện thực hóa hàm sử dụng vật phẩm sau khi có class Player công khai
inline void HealthPotion::use(Player& player) { player.setHP(player.getHP() + (30 + floorLevel * 15)); }
inline void ManaPotion::use(Player& player) { player.setMana(player.getMana() + (20 + floorLevel * 10)); }
inline void StatScroll::use(Player& player) {
    if (statType == 1) playerpermanentlyBuff(floorLevel * 15, 0, 0);
    else if (statType == 2) playerpermanentlyBuff(0, floorLevel * 3, 0);
    else playerpermanentlyBuff(0, 0, floorLevel * 2);
}

// ============================================================================
// STEP 4: 3 LỚP NHÂN VẬT VỚI CƠ CHẾ TIÊU TỐN TÀI NGUYÊN ĐỘC ĐÁO
// ============================================================================

// 1. WARRIOR: Hi sinh Thủ để đổi Sát thương cực đại
class Warrior : public Player {
public:
    Warrior(std::string n) : Player(n, 160, 40, 15, 8, 5, 10) {}
    std::string attack(Character& target) override {
        return "[Attack] Heavy Slash! " + target.takeDamage(attackPower, intelligence, name);
    }
    // Skill 1: Tốn ít Mana thường
    std::string useSkill1(Character& target) override {
        if (mana < 15) return "Not enough Mana!";
        mana -= 15;
        return "[Skill 1: Shield Bash] " + target.takeDamage(attackPower + 10, intelligence, name);
    }
    // Skill 2: KHÔNG tốn mana, nhưng tự GIẢM VĨNH VIỄN 3 THỦ trong trận này để chém x3 sát thương
    std::string useSkill2(Character& target) override {
        applyDefDebuff(3);
        int recklessDmg = attackPower * 3;
        return "[Skill 2: Reckless Break] Sacrificed 3 DEF for this battle! " + target.takeDamage(recklessDmg, intelligence, name);
    }
    int getClassType() const override { return 1; }
};

// 2. MAGE: Dùng máu làm tế phẩm đúc phép bùng nổ
class Mage : public Player {
public:
    Mage(std::string n) : Player(n, 90, 100, 8, 3, 25, 12) {}
    std::string attack(Character& target) override {
        return "[Attack] Magic Spark! " + target.takeDamage(attackPower, intelligence, name);
    }
    // Skill 1: Phép tiêu tốn lượng Mana lớn
    std::string useSkill1(Character& target) override {
        if (mana < 35) return "Not enough Mana!";
        mana -= 35;
        return "[Skill 1: Comet Fireball] " + target.takeDamage(attackPower * 2, intelligence, name);
    }
    // Skill 2: TỐN 25 MÁU của bản thân để kích hoạt cấm thuật Sát thương x3.5
    std::string useSkill2(Character& target) override {
        if (hp <= 25) return "Too weak to offer Blood Sacrifice!";
        hp -= 25; // Trừ thẳng vào máu hiện tại
        int bloodDmg = static_cast<int>(attackPower * 3.5);
        return "[Skill 2: Blood Rupture] Sacrificed 25 HP! " + target.takeDamage(bloodDmg, intelligence, name);
    }
    int getClassType() const override { return 2; }
};

// 3. ROGUE: Hi sinh Sự cơ động (Né tránh) để ám sát chắc chắn Crit
class Rogue : public Player {
public:
    Rogue(std::string n) : Player(n, 110, 50, 12, 4, 15, 25) {}
    std::string attack(Character& target) override {
        return "[Attack] Twin Daggers! " + target.takeDamage(attackPower, intelligence, name);
    }
    // Skill 1: Tốn ít Mana
    std::string useSkill1(Character& target) override {
        if (mana < 12) return "Not enough Mana!";
        mana -= 12;
        return "[Skill 1: Poison Poison] " + target.takeDamage(attackPower + 12, intelligence, name);
    }
    // Skill 2: Tốn 15 Mana, Giảm vĩnh viễn 6 NÉ (Agility) trong trận này để dồn lực bạo kích chí mạng 100%
    std::string useSkill2(Character& target) override {
        if (mana < 15) return "Not enough Mana!";
        mana -= 15;
        applyAgiDebuff(6); // Giảm né trong trận
        int shadowDmg = attackPower * 2;
        // Truyền chỉ số INT bằng 100 để ép hệ thống ép buộc nổ Chí mạng (Critical)
        return "[Skill 2: Shadow Dance] Sacrificed 6 AGI for guaranteed Critical! " + target.takeDamage(shadowDmg, 100, name);
    }
    int getClassType() const override { return 3; }
};

// ============================================================================
// STEP 5: LỚP QUÁI VẬT TĂNG THEO TẦNG
// ============================================================================
class Monster : public Character {
public:
    Monster(int floor) : Character("Floor " + std::to_string(floor) + " Fiend", 0, 0, 0, 0, 0, 0) {
        this->maxHp = 50 + (floor * 22);
        this->hp = this->maxHp;
        this->mana = 0; this->maxMana = 0;
        this->attackPower = 12 + (floor * 3);
        this->defense = 1 + (floor * 2);
        this->intelligence = 5 + floor; 
        this->agility = 4 + floor;      
    }
    std::string attack(Character& target) override {
        return "[" + name + " Claws] " + target.takeDamage(attackPower, intelligence, name);
    }
};

// ============================================================================
// STEP 6: HÀM TẠO 3 PHẦN THƯỞNG NGẪU NHIÊN CHO BẠN LÀM STAGE MÀN HÌNH
// ============================================================================
inline std::vector<std::unique_ptr<Item>> generateThreeRewards(int floor) {
    std::vector<std::unique_ptr<Item>> choices;
    for (int i = 0; i < 3; i++) {
        int roll = std::rand() % 3;
        if (roll == 0) {
            choices.push_back(std::make_unique<HealthPotion>(floor));
        } else if (roll == 1) {
            choices.push_back(std::make_unique<ManaPotion>(floor));
        } else {
            int statType = (std::rand() % 3) + 1; // Thưởng ngẫu nhiên cuộn tăng HP, ATK hoặc DEF
            choices.push_back(std::make_unique<StatScroll>(floor, statType));
        }
    }
    return choices; // Trả về danh sách 3 món đồ tỷ lệ thuận theo cấp độ tầng
}

// ============================================================================
// STEP 7: TIÊU ĐỀ HỆ THỐNG CHIẾN ĐẤU (COMBAT SYSTEM)
// ============================================================================
class CombatSystem {
private:
    Player& hero;
    Monster& enemy;
    std::string battleLog;
    bool isPlayerTurn;

public:
    CombatSystem(Player& p, Monster& m);

    void playerAttack();
    void playerSkill1();
    void playerSkill2();
    void playerUseItem(size_t index);
    
    void executeEnemyTurn(); 

    bool isCombatOver() const;
    bool isHeroVictorious() const;
    bool getIsPlayerTurn() const { return isPlayerTurn; }
    std::string getBattleLog() const { return battleLog; }
};

#endif // GAME_CORE_HPP