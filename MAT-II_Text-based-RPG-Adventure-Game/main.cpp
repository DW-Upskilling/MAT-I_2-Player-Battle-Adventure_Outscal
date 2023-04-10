#include <iostream>
#include <memory>
#include <time.h>
#include <vector>
using namespace std;

enum SpecialAbilitiesType {
  CriticalHitsAbility = 2,
  BlockerAbility = 3,
  LifeStealAbility = 4,
  RangedAttackAbility = 5
};
enum SpecialBossAbilitiesType {
  GroundSlashAbility = 6,
  SpeedDashAbility = 7,
  RockAbility = 8
};
enum SpecialItemsType {
  MapItem = 1,
  SwordItem = 2,
  ShieldItem = 3,
  ArmourItem = 4,
  BowItem = 5
};

class Constants {
public:
  // Player default attributes
  static const int DEFAULT_HEALTH = 100, DEFAULT_MELEE = 25;
  static const int DEFAULT_DEFENCE = 100, DEFAULT_HEALTH_REGENERATION = 10;
  // Enemy default attributes
  static const int DEFAULT_ENEMY_HEALTH = 60, DEFAULT_ENEMY_MELEE = 35;
  static const int DEFAULT_ENEMY_DEFENCE = 100;
  // Boss Enemy default attributes
  static const int DEFAULT_BOSS_HEALTH = 500, DEFAULT_BOSS_MELEE = 75;
  static const int DEFAULT_BOSS_DEFENCE = 300;
  static constexpr const char *BOSS_NAME = "Murlocs";
  // Level default attributes
  static const int DEFAULT_LEVEL = 1, DEFAULT_ENEMIES = 1,
                   DEFAULT_MAX_LEVEL = 6;
  static const int DEFAULT_STAT_BOOST = 25;
  // Special Items default attributes
  static const int DEFAULT_SWORD = 25, DEFAULT_SHIELD = 10;
  static const int DEFAULT_ARMOUR = 50, DEFAULT_BOW = 30;
  // Special Abilities default attributes
  static const int DEFAULT_CRITICAL_HITS = 2, DEFAULT_BLOCKER = 25;
  static const int DEFAULT_LIFE_STEAL = 18, DEFAULT_RANGED_ATTACK = 10;
  static const int DEFAULT_GROUND_SLASH = 3, DEFAULT_SPEED_DASH = 4;
  static const int DEFAULT_ROCK = 100;

  static const int DEFAULT_PROBABILITY = 10, DEFAULT_BOSS_PROBABILITY = 75;

  static string getEnemyName(int level, int sequenceNumber) {
    return "(Level " + to_string(level) + ") Orc" +
           to_string(sequenceNumber + 1);
  }

  static void debugger(string prefix, string message) {
    cout << prefix << ":: " << message << endl;
  }
};

class PlayerEntity {
private:
  int health, melee, defence, health_regeneration;
  int current_health;
  string name;

protected:
  void setHealth(int health) {
    this->health = health;
    this->setCurrentHealth(health);
  }
  void setCurrentHealth(int current_health) {
    this->current_health = current_health;
  }
  void setMelee(int melee) { this->melee = melee; }
  void setDefence(int defence) { this->defence = defence; }
  void setHealthRegeneration(int health_regeneration) {
    this->health_regeneration = health_regeneration;
  }

public:
  PlayerEntity(string name) { this->name = name; }
  virtual ~PlayerEntity() {}

  string getName() { return name; }

  int getHealth() { return health; }
  int getCurrentHealth() { return current_health; }
  int getMelee() { return melee; }
  int getDefence() { return defence; }
  int getHealthRegeneration() { return health_regeneration; }

  bool isAlive() { return current_health > 0; }

  void show() {
    cout << "\nSTATS OF " << getName() << endl;
    cout << "Health: " << getCurrentHealth() << "/" << getHealth() << endl;
    cout << "Defence: " << getDefence() << endl;
    cout << "Melee: " << getMelee() << endl;
    cout << "Regeneration: " << getHealthRegeneration() << endl;
  }

  virtual int getDamage() { return getMelee(); }
  virtual int getDamageReduction() { return getDefence(); }

  virtual void attack(PlayerEntity *oppositePlayer) = 0;
  virtual void heal() = 0;
  virtual void takeDamage(int damage) = 0;
  virtual int getEntityMove(PlayerEntity *oppositePlayer) = 0;
};

class Abilities {
private:
  vector<SpecialAbilitiesType> specialAbilities, specialAttackAbilities,
      specialDefenceAbilities;
  vector<SpecialBossAbilitiesType> specialBossAbilities,
      specialBossAttackAbilities, specialBossDefenceAbilities;
  int criticalHits(int damage) {
    int criticalHitsDamage = damage * (Constants::DEFAULT_CRITICAL_HITS - 1);
    cout << "[Enchanted Purse]:: Critical hits rolled! "
         << "Providing extra damage of " << criticalHitsDamage << "!" << endl;
    return criticalHitsDamage;
  }
  int blocker() {
    cout << "[Enchanted Purse]:: Blocker rolled! "
         << "Providing extra defence of " << Constants::DEFAULT_BLOCKER << "!"
         << endl;
    return Constants::DEFAULT_BLOCKER;
  }
  void lifeSteal(int damage) {
    int lifeStealHealed =
        (int)((damage / 100.0) * Constants::DEFAULT_LIFE_STEAL);
    cout << "[Enchanted Purse]:: Life Steal rolled! "
         << "Providing extra healing of " << lifeStealHealed << "!" << endl;
    heal(lifeStealHealed);
  }
  int rangedAttack(int damage) {
    int rangedAttackDamage =
        (int)((damage / 100.0) * Constants::DEFAULT_RANGED_ATTACK);
    cout << "[Enchanted Purse]:: Ranged Attack rolled! "
         << "Providing extra damage of " << rangedAttackDamage << "!" << endl;
    return rangedAttackDamage;
  }
  int groundSlash(int damage) {
    int groundSlashDamage =
        (int)((damage / 100.0) * Constants::DEFAULT_GROUND_SLASH);
    cout << "[Enchanted Purse]:: Ground Slash rolled! "
         << "Providing extra damage of " << groundSlashDamage << "!" << endl;
    return groundSlashDamage;
  }
  int speedDash(int damage) {
    int speedDashDamage =
        (int)((damage / 100.0) * Constants::DEFAULT_SPEED_DASH);
    cout << "[Enchanted Purse]:: Speed Dash rolled! "
         << "Providing extra damage of " << speedDashDamage << "!" << endl;
    return speedDashDamage;
  }
  int rockAbility() {
    cout << "[Enchanted Purse]:: Rock rolled! "
         << "Providing extra defence and healing of " << Constants::DEFAULT_ROCK
         << "!" << endl;
    heal(Constants::DEFAULT_ROCK);
    return Constants::DEFAULT_ROCK;
  }

public:
  // CriticalHits, Blocker, LifeSteal, RangedAttack
  // GroundSlash, SpeedDash, HealthRegeneration
  static string getAbilityName(SpecialAbilitiesType ability) {
    switch (ability) {
    case SpecialAbilitiesType::CriticalHitsAbility:
      return "Critical Hits";
    case SpecialAbilitiesType::BlockerAbility:
      return "Blocker";
    case SpecialAbilitiesType::LifeStealAbility:
      return "Life Steal";
    case SpecialAbilitiesType::RangedAttackAbility:
      return "Ranged Attack";
    }
    return "";
  }
  static string getAbilityName(SpecialBossAbilitiesType ability) {
    switch (ability) {
    case SpecialBossAbilitiesType::GroundSlashAbility:
      return "Ground Slash";
    case SpecialBossAbilitiesType::SpeedDashAbility:
      return "Speed Dash";
    case SpecialBossAbilitiesType::RockAbility:
      return "Rock";
    }
    return "";
  }
  string getAbilitiesString() {
    string items = "[";

    for (int i = 0; i < specialAbilities.size(); i++)
      items += getAbilityName(specialAbilities[i]) + ",";
    for (int i = 0; i < specialAttackAbilities.size(); i++)
      items += getAbilityName(specialAttackAbilities[i]) + ",";
    for (int i = 0; i < specialDefenceAbilities.size(); i++)
      items += getAbilityName(specialDefenceAbilities[i]) + ",";

    for (int i = 0; i < specialBossAbilities.size(); i++)
      items += getAbilityName(specialBossAbilities[i]) + ",";
    for (int i = 0; i < specialBossAttackAbilities.size(); i++)
      items += getAbilityName(specialBossAttackAbilities[i]) + ",";
    for (int i = 0; i < specialBossDefenceAbilities.size(); i++)
      items += getAbilityName(specialBossDefenceAbilities[i]) + ",";
    return items + "]";
  }

  void unlockAbility(SpecialAbilitiesType ability) {
    switch (ability) {
    case SpecialAbilitiesType::CriticalHitsAbility:
    case SpecialAbilitiesType::RangedAttackAbility:
    case SpecialAbilitiesType::LifeStealAbility:
      specialAttackAbilities.push_back(ability);
      cout << "[Heisenberg]:: Unlocked the attacking ability "
           << getAbilityName(ability) << "!" << endl;
      break;
    case SpecialAbilitiesType::BlockerAbility:
      specialDefenceAbilities.push_back(ability);
      cout << "[Heisenberg]:: Unlocked the defence ability "
           << getAbilityName(ability) << "!" << endl;
      break;
    }
  }
  void unlockAbility(SpecialBossAbilitiesType ability) {
    switch (ability) {
    case SpecialBossAbilitiesType::GroundSlashAbility:
    case SpecialBossAbilitiesType::SpeedDashAbility:
      specialBossAttackAbilities.push_back(ability);
      cout << "[Heisenberg]:: Unlocked the attacking ability "
           << getAbilityName(ability) << "!" << endl;
      break;
    case SpecialBossAbilitiesType::RockAbility:
      specialBossDefenceAbilities.push_back(ability);
      cout << "[Heisenberg]:: Unlocked the defence ability "
           << getAbilityName(ability) << "!" << endl;
      break;
    }
  }

  bool isRolled() {
    srand(time(0));
    int current = 1 + (rand() % 100);
    return current <= Constants::DEFAULT_PROBABILITY;
  }

  bool isBossRolled() {
    srand(time(0));
    int current = 1 + (rand() % 100);
    return current <= Constants::DEFAULT_BOSS_PROBABILITY;
  }

  int triggerAttack(int _damage) {
    int damage = 0;
    for (int i = 0; i < specialAttackAbilities.size(); i++) {
      if (isRolled())
        switch (specialAttackAbilities[i]) {
        case SpecialAbilitiesType::CriticalHitsAbility:
          damage += criticalHits(_damage);
          break;
        case SpecialAbilitiesType::RangedAttackAbility:
          damage += rangedAttack(_damage);
          break;
        case SpecialAbilitiesType::LifeStealAbility:
          lifeSteal(_damage);
          break;
        default:
          break;
        }
    }
    for (int i = 0; i < specialBossAttackAbilities.size(); i++) {
      if (isBossRolled())
        switch (specialBossAttackAbilities[i]) {
        case SpecialBossAbilitiesType::GroundSlashAbility:
          damage += groundSlash(_damage);
          break;
        case SpecialBossAbilitiesType::SpeedDashAbility:
          damage += speedDash(_damage);
          break;
        default:
          break;
        }
    }
    return damage;
  }
  int triggerDefence() {
    int defence = 0;
    for (int i = 0; i < specialDefenceAbilities.size(); i++) {
      if (isRolled())
        switch (specialDefenceAbilities[i]) {
        case SpecialAbilitiesType::BlockerAbility:
          defence += blocker();
          break;
        default:
          break;
        }
    }
    for (int i = 0; i < specialBossDefenceAbilities.size(); i++) {
      if (isBossRolled())
        switch (specialBossDefenceAbilities[i]) {
        case SpecialBossAbilitiesType::RockAbility:
          defence += rockAbility();
          break;
        default:
          break;
        }
    }
    return defence;
  }

  virtual void heal(int healed) = 0;
};
class Items {
private:
  vector<SpecialItemsType> specialItems;
  vector<SpecialItemsType> specialAttackItems;
  vector<SpecialItemsType> specialDefenceItems;
  int sword() {
    cout << "[Enchanted Backpack]:: With a mighty swing, the adventurer "
         << "unleashes the full force of the enchanted sword, dealing "
         << Constants::DEFAULT_SWORD << " damage!" << endl;
    return Constants::DEFAULT_SWORD;
  }
  int shield() {
    cout << "[Enchanted Backpack]:: A shield fit for a champion! With a "
         << "formidable defense of " << Constants::DEFAULT_SHIELD
         << ", I'm ready to take on any enemy that dares to cross my path. "
         << "Bring it on!" << endl;
    return Constants::DEFAULT_SHIELD;
  }
  int armour() {
    cout << "[Enchanted Backpack]:: Who needs a tank when you have ME in this "
         << "armor?! Adding a whopping " << Constants::DEFAULT_ARMOUR
         << " to my defence! Bring on the enemies!" << endl;
    return Constants::DEFAULT_ARMOUR;
  }
  int bow() {
    cout << "[Enchanted Backpack]:: With the precision of a one-eyed "
         << "adventurer, the arrow from the bow dealt a devastating "
         << Constants::DEFAULT_BOW << " damage straight to the enemy's head!"
         << endl;
    return Constants::DEFAULT_BOW;
  }

public:
  static string getItemName(SpecialItemsType item) {
    switch (item) {
    case SpecialItemsType::MapItem:
      return "Map";
    case SpecialItemsType::SwordItem:
      return "Sword";
    case SpecialItemsType::ShieldItem:
      return "Shield";
    case SpecialItemsType::ArmourItem:
      return "Armour";
    case SpecialItemsType::BowItem:
      return "Bow";
    }
    return "";
  }
  void unlockItem(SpecialItemsType specialItem) {
    switch (specialItem) {
    case SpecialItemsType::MapItem:
      specialItems.push_back(specialItem);
      cout << "[Heisenberg]:: Unlocked the item " << getItemName(specialItem)
           << "!" << endl;
      break;
    case SpecialItemsType::SwordItem:
    case SpecialItemsType::BowItem:
      specialAttackItems.push_back(specialItem);
      cout << "[Heisenberg]:: Unlocked the attacking item "
           << getItemName(specialItem) << "!" << endl;
      break;
    case SpecialItemsType::ShieldItem:
    case SpecialItemsType::ArmourItem:
      specialDefenceItems.push_back(specialItem);
      cout << "[Heisenberg]:: Unlocked the defence item "
           << getItemName(specialItem) << "!" << endl;
      break;
    }
  }
  string getItemsString() {
    string items = "[";
    for (int i = 0; i < specialItems.size(); i++)
      items += getItemName(specialItems[i]) + ",";
    for (int i = 0; i < specialAttackItems.size(); i++)
      items += getItemName(specialAttackItems[i]) + ",";
    for (int i = 0; i < specialDefenceItems.size(); i++)
      items += getItemName(specialDefenceItems[i]) + ",";
    return items + "]";
  }

  int triggerAttack() {
    if (specialAttackItems.size() == 0)
      return 0;
    int choice;
    cout << "[Enchanted Backpack]: Ah, it seems that we have found some items "
            "that "
            "might come "
            "in handy. Let us see what we have here."
         << endl;
    for (int i = 0; i < specialAttackItems.size(); i++)
      cout << (i + 1) << ". " << getItemName(specialAttackItems[i]) << endl;
    cout << "Use?";
    cin >> choice;
    if (choice > 0 && choice <= specialAttackItems.size()) {
      SpecialItemsType item = specialAttackItems[choice - 1];
      switch (item) {
      case SpecialItemsType::SwordItem:
        return sword();
        break;
      case SpecialItemsType::BowItem:
        return bow();
        break;
      default:
        break;
      }
    }
    cout << "[Enchanted Backpack]:: Alas, it seems that the chosen item failed "
            "to be picked "
            "up in time. An opportunity to strike has been missed. The item in "
            "question was "
         << choice << ", what a shame." << endl;

    return 0;
  }
  int triggerDefence() {
    int defence = 0;
    for (int i = 0; i < specialDefenceItems.size(); i++)
      switch (specialDefenceItems[i]) {
      case SpecialItemsType::ShieldItem:
        defence += shield();
        break;
      case SpecialItemsType::ArmourItem:
        defence += armour();
        break;
      default:
        break;
      }
    return defence;
  }
};

class Player : public PlayerEntity, public Abilities, public Items {

public:
  Player(string playerName) : PlayerEntity(playerName) {
    this->setHealth(Constants::DEFAULT_HEALTH);
    this->setMelee(Constants::DEFAULT_MELEE);
    this->setDefence(Constants::DEFAULT_DEFENCE);
    this->setHealthRegeneration(Constants::DEFAULT_HEALTH_REGENERATION);
  }
  ~Player() {
    cout << "[Reaper]:: Oh, what a tragedy! The once-mighty warrior "
         << getName()
         << " falls to the ground, betrayed and backstabbed by a mere "
         << "traveller. And to add insult to injury, their precious hoodies "
         << "are once again stolen by none other than " << Constants::BOSS_NAME
         << "! Who will save them now? Stay tuned for the next episode of the "
         << "Hoodie Heist!" << endl;
  }

  int getDamage() {
    int damage = PlayerEntity::getDamage() + Items::triggerAttack();
    damage += Abilities::triggerAttack(damage);
    return damage;
  }
  int getDamageReduction() {
    int defence = PlayerEntity::getDamageReduction() + Items::triggerDefence();
    defence += Abilities::triggerDefence();
    return defence;
  }

  void show() {
    cout << "\nSTATS OF " << getName() << endl;
    cout << "Health: " << getCurrentHealth() << "/" << getHealth() << endl;
    cout << "Defence: " << getDefence() << endl;
    cout << "Melee: " << getMelee() << endl;
    cout << "Regeneration: " << getHealthRegeneration() << endl;
    cout << "Items: " << getItemsString() << endl;
    cout << "Abilities: " << getAbilitiesString() << endl;
  }

  void attack(PlayerEntity *oppositePlayer) {
    int damage = getDamage();
    cout << "[Mysterious Voice]:: A massive power of destruction worth "
         << damage << " points has been unleashed upon the enemies by "
         << getName() << "!" << endl;
    oppositePlayer->takeDamage(damage);
  }
  void heal() {
    int healed =
        getHealthRegeneration() % (1 + getHealth() - getCurrentHealth());
    cout << "[Mysterious Voice]:: Behold, by the grace of the fairy gods, the "
            "magnificent "
         << getName()
         << " has been bestowed with the power of self-healing, and has "
            "regained "
         << healed << " precious life points!";
    setCurrentHealth(getCurrentHealth() + healed);
    cout << " Their current health points now stand at " << getCurrentHealth()
         << "/" << getHealth() << ". Truly remarkable, isn't it?" << endl;
  }
  void heal(int _healed) {
    int healed = _healed % (1 + getHealth() - getCurrentHealth());
    cout << "[Mysterious Voice]:: How interesting, it appears that "
         << getName() << " has successfully restored themselves for " << healed
         << " precious life points!" << endl;
    setCurrentHealth(getCurrentHealth() + healed);
    cout << " Their current health points now stand at " << getCurrentHealth()
         << "/" << getHealth() << ". Truly remarkable, isn't it?" << endl;
  }
  void takeDamage(int damage) {
    // every 10 defence will reduce 1 damage
    double defence = this->getDamageReduction() / 10.0;
    int final_damage = (damage - defence);
    cout << "[Mysterious Voice]:: Oh no! " << getName()
         << " has taken damage. The damage dealt has been reduced to "
         << final_damage << " hit points";
    setCurrentHealth(getCurrentHealth() - final_damage);
    if (getCurrentHealth() < 0)
      setCurrentHealth(0);
    cout << " leaving health at " << getCurrentHealth() << "/" << getHealth()
         << ". We must be careful and stay vigilant in our battles." << endl;
  }

  int getEntityMove(PlayerEntity *enemy) {
    srand(time(0));
    int choice = 0;
    cout << "\nBehold! The mighty menu of actions! What will you do, oh great "
            "and powerful adventurer? Will you:"
         << endl;
    cout << "\t1. Attack with the fury of a thousand angry goblins?" << endl;
    cout << "\t2. Heal thyself with the magic of ancient unicorns?" << endl;
    cout << "\t3. Or quit like a scaredy-cat who just saw a dragon's toenail?"
         << endl;
    cout << "Choose wisely, oh fearless one!";
    cin >> choice;

    switch (choice) {
    case 1:
      this->attack(enemy);
      return 1;
    case 2:
      this->heal();
      return 2;
    case 3:
      this->setCurrentHealth(0);
      return -1;
    default:
      return getEntityMove(enemy);
    }
  }

  void levelRewards(int level) {
    setHealth((getHealth() / 100.0) * Constants::DEFAULT_STAT_BOOST +
              getHealth());
    setMelee((getMelee() / 100.0) * Constants::DEFAULT_STAT_BOOST + getMelee());
    setDefence((getDefence() / 100.0) * Constants::DEFAULT_STAT_BOOST +
               getDefence());
    setHealthRegeneration((getHealthRegeneration() / 100.0) *
                              Constants::DEFAULT_STAT_BOOST +
                          getHealthRegeneration());

    if (level < Constants::DEFAULT_MAX_LEVEL) {
      unlockItem((SpecialItemsType)level);
      unlockAbility((SpecialAbilitiesType)level);
    }
  }
};
class Enemy : public PlayerEntity {
public:
  Enemy(string enemyName) : PlayerEntity(enemyName) {
    this->setHealth(Constants::DEFAULT_ENEMY_HEALTH);
    this->setMelee(Constants::DEFAULT_ENEMY_MELEE);
    this->setDefence(Constants::DEFAULT_ENEMY_DEFENCE);
    this->setHealthRegeneration(Constants::DEFAULT_HEALTH_REGENERATION);
  }
  virtual ~Enemy() {
    cout << "[Reaper]:: The " << getName()
         << " remains have been harvested by the forces of "
         << "nature, leaving behind a bounty of loot for the taking!" << endl;
  }
  void attack(PlayerEntity *oppositePlayer) {
    int damage = getDamage();
    cout << "[Mysterious Voice]:: Ah, it appears that the " << getName()
         << " has chosen its attack and dealt a damage of " << damage << "."
         << endl;
    oppositePlayer->takeDamage(damage);
  }
  void heal() {
    int healed =
        getHealthRegeneration() % (1 + getHealth() - getCurrentHealth());
    cout << "[Mysterious Voice]:: How interesting! It appears that "
         << getName()
         << " has chosen to use their healing abilities. They have "
            "successfully restored themselves for "
         << healed << " points. Marvelous!";
    setCurrentHealth(getCurrentHealth() + healed);
    cout << " The total health points of " << getCurrentHealth() << "/"
         << getHealth() << " is now something to be admired." << endl;
  }
  void takeDamage(int damage) {
    // every 10 defence will reduce 1 damage
    double defence = getDamageReduction() / 10.0;
    int final_damage = damage - defence;
    cout << "[Mysterious Voice]:: Hark! It appears that the enemy named "
         << getName()
         << " has been struck by a blow. However, the damage done has been "
            "reduced to a mere "
         << final_damage << " points.";
    setCurrentHealth(getCurrentHealth() - final_damage);
    if (getCurrentHealth() < 0)
      setCurrentHealth(0);
    cout << " Alas, the foe still persists with " << getCurrentHealth() << "/"
         << getHealth() << " health remaining." << endl;
  }

  int getEntityMove(PlayerEntity *player) {
    srand(time(0));
    int choice = 1 + rand() % 2;
    switch (choice) {
    case 1:
      this->attack(player);
      return 1;
    case 2:
      this->heal();
      return 2;
    default:
      return getEntityMove(player);
    }
  }
};
class BossEnemy : public Enemy, public Abilities {
public:
  BossEnemy(string bossName) : Enemy(bossName) {
    this->setHealth(Constants::DEFAULT_BOSS_HEALTH);
    this->setMelee(Constants::DEFAULT_BOSS_MELEE);
    this->setDefence(Constants::DEFAULT_BOSS_DEFENCE);

    unlockAbility((SpecialBossAbilitiesType)6);
    unlockAbility((SpecialBossAbilitiesType)7);
    unlockAbility((SpecialBossAbilitiesType)8);
  }
  ~BossEnemy() {
    cout << "[Reaper]:: The mighty " << Constants::BOSS_NAME
         << " has been reaped! But beware, whispers say he'll return to steal "
         << "our hoodies once more!" << endl;
  }

  int getDamage() {
    int damage = PlayerEntity::getDamage();
    damage += Abilities::triggerAttack(damage);
    return damage;
  }
  int getDamageReduction() {
    int defence = PlayerEntity::getDamageReduction();
    defence += Abilities::triggerDefence();
    return defence;
  }

  void heal(int _healed) {
    int healed = _healed % (1 + getHealth() - getCurrentHealth());
    cout << "[Mysterious Voice]:: How interesting, it appears that "
         << getName() << " has successfully restored themselves for " << healed
         << " precious life points!" << endl;
    setCurrentHealth(getCurrentHealth() + healed);
    cout << " Their current health points now stand at " << getCurrentHealth()
         << ". Truly remarkable, isn't it?" << endl;
  }

  void show() {
    cout << "\nSTATS OF " << getName() << endl;
    cout << "Health: " << getCurrentHealth() << "/" << getHealth() << endl;
    cout << "Defence: " << getDefence() << endl;
    cout << "Melee: " << getMelee() << endl;
    cout << "Regeneration: " << getHealthRegeneration() << endl;
    cout << "Abilities" << getAbilitiesString() << endl;
  }
};

class World {
private:
  int current_level, total_enemies;

  void setCurrentLevel(int current_level) {
    this->current_level = current_level;
  }
  void setTotalEnemies(int total_enemies) {
    this->total_enemies = total_enemies;
  }

public:
  World() {
    this->setCurrentLevel(Constants::DEFAULT_LEVEL);
    this->setTotalEnemies(Constants::DEFAULT_ENEMIES);
  }

  int getCurrentLevel() { return current_level; }
  int getTotalEnemies() { return total_enemies; }

  bool isBossDefeated() {
    return Constants::DEFAULT_MAX_LEVEL < getCurrentLevel();
  }

  void spawnCurrentLevel(Player *player) {
    string buffer;
    cout << "\nAs you and the Traveller continue on your quest, the sound of "
            "your steady journey can be heard: uWu-uWu-uWu-uWu...."
         << endl;
    vector<unique_ptr<Enemy>> enemies;
    if (Constants::DEFAULT_MAX_LEVEL > getCurrentLevel()) {
      cout << "[Traveller]:: Look (" << getTotalEnemies()
           << ")Orc's ahead! Defeat them to proceed!" << endl;
      cout << "*With the Traveller at your side, you rush forward towards the "
              "orcs, filled with courage and determination*"
           << endl;
      for (int i = 0; i < getTotalEnemies(); i++) {
        enemies.push_back(
            make_unique<Enemy>(Constants::getEnemyName(getCurrentLevel(), i)));
      }
    } else {
      cout << "\n[Traveller]:: Rise up, warriors! Our nemesis "
           << Constants::BOSS_NAME
           << " stands before us, but we will not be defeated. We fight for "
              "our honor and for the return of our beloved hoodies. Charge "
              "forth and claim victory!"
           << endl;
      cout << "*The demon emerges from the ground with a menacing growl and "
              "charges towards you*"
           << endl;
      cout << "\nPress ENTER to continue.";
      cin.ignore();
      getline(cin, buffer);
      enemies.push_back(make_unique<BossEnemy>(string(Constants::BOSS_NAME)));
    }

    for (int i = 0; i < enemies.size() && player->isAlive(); i++)
      enterBattleArena(player, enemies[i].get());

    if (player->isAlive()) {
      cout
          << "\n[Traveller]:: Well done, adventurer! You took care of those "
             "orcs "
             "with ease. If I had to deal with them, it would have been a mess."
          << endl;
      cout << "[Traveller]:: The spoils of victory are ours, adventurer! Do "
              "not forget to collect your well-deserved loot from the defeated "
              "orcs. It may prove to be valuable in our future endeavors.\n"
           << endl;
      player->levelRewards(getCurrentLevel());
    }

    if (!isBossDefeated() && player->isAlive()) {
      setCurrentLevel(getCurrentLevel() + 1);
      setTotalEnemies(getTotalEnemies() + 1);
    }
    cout << "\nPress ENTER to continue.";
    cin.ignore();
    getline(cin, buffer);
  }

  void enterBattleArena(Player *player, Enemy *enemy) {
    player->show();
    enemy->show();
    while (player->isAlive() && enemy->isAlive()) {
      if (player->isAlive() && enemy->isAlive())
        player->getEntityMove(enemy);
      if (player->isAlive() && enemy->isAlive())
        enemy->getEntityMove(player);
    }
  }
};
class Game {
private:
  void beginAdventure(Player *player, World *world) {
    world->spawnCurrentLevel(player);
    if (player->isAlive() && !world->isBossDefeated()) {
      return beginAdventure(player, world);
    }
  }

public:
  Game() {}
  void start() {
    string playerName, buffer;
    cout << "[Traveller]:: Greetings, adventurer. It feels a bit odd to "
            "address you as such without knowing your name. Might I inquire as "
            "to what I should call you? My name is Traveller, by the way."
         << endl;
    cout << "With a friendly smile, the Traveller awaited your response?";
    cin.ignore();
    getline(cin, playerName);
    cout << "\n[Traveller]:: Ah, I see. " << playerName
         << ", what a unique name. Now, let's focus on our mission. The orcs "
            "you seek left under the cover of night at a quarter to 9:15 PM."
         << endl;
    cout << "[Traveller]:: I followed them to their destination, where they "
            "are working with a dangerous demon and the assistant of the "
            "Superior Dragon named "
         << Constants::BOSS_NAME
         << ". This will be a tough fight, but don't worry! Together, we can "
            "defeat any foe. Let's hurry to their lair and reclaim what is "
            "rightfully yours, brave adventurer."
         << endl;
    cout << "\nPress ENTER to continue.";
    getline(cin, buffer);
    unique_ptr<Player> player(new Player(playerName));
    unique_ptr<World> world(new World());
    beginAdventure(player.get(), world.get());
    if (world->isBossDefeated())
      cout << "\n[Epic Voice]:: Ladies and gentlemen, put your hands together "
              "for the one and only "
           << player->getName()
           << "! What a magnificent victory, our hero has emerged victorious "
              "from this fierce battle! A round of applause, please!"
           << endl;
    else
      cout << "\n[Epic Voice]:: HAHAHA! " << player->getName()
           << " has fallen in battle, what a pathetic display of skill!"
           << endl;
    cout << "\nPress ENTER to continue.";
    getline(cin, buffer);
  }
};

int main() {
  Game game;
  cout << "################################" << endl;
  cout << "### WELCOME TO RPG ADVENTURE ###" << endl;
  cout << "################################" << endl;

  int choice = 0;
  while (choice != -1) {
    cout << "\n[Hobbit]:: Hello, adventurer! Welcome to the Champions Village."
         << endl;
    cout << "[Hobbit]:: Last week, our village was attacked by orcs, and they "
            "took all of my latest collection of hoodies."
         << endl;
    cout << "1. Help the Hobbit" << endl;
    cout << "2. Ignore him like a coward" << endl;
    cout << "Would you be able to help me retrieve them?";
    cin >> choice;
    switch (choice) {
    case 1:
      cout << "[Hobbit]:: I am heartened to hear that you shall aid me in my "
              "plight"
           << endl;
      cout << "[Hobbit]:: I know a skilled traveller who can guide you through "
              "the perilous 'Dark Forest' and the dense 'Small Forest'. With "
              "their help and your courage, we can reclaim my stolen hoodies.\n"
           << endl;
      game.start();
      break;
    case 2:
      choice = -1;
      break;
    default:
      choice = 0;
    }
  }
}