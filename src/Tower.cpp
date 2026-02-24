#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>

////////////////////////////////////////////////////////////
// BASIC STRUCTS
////////////////////////////////////////////////////////////

struct Vector2 {
    float x, y;
};

float distance(Vector2 a, Vector2 b) {
    return std::sqrt(
        (a.x - b.x) * (a.x - b.x) +
        (a.y - b.y) * (a.y - b.y)
    );
}

////////////////////////////////////////////////////////////
// STATUS SYSTEM
////////////////////////////////////////////////////////////

enum class StatusType {
    Burn,
    Slow,
    Stun
};

struct StatusEffect {
    StatusType type;
    float duration;
    float value;
};

////////////////////////////////////////////////////////////
// ENEMY
////////////////////////////////////////////////////////////

class Enemy {
public:
    Vector2 position;
    float hp;
    float baseSpeed;
    float currentSpeed;
    bool alive;

    std::vector<StatusEffect> effects;

    Enemy(float x, float y)
        : position{x,y}, hp(200),
          baseSpeed(40), currentSpeed(40),
          alive(true)
    {}

    void takeDamage(float dmg) {
        hp -= dmg;
        if (hp <= 0) {
            alive = false;
            std::cout << "Enemy Dead\n";
        }
    }

    void applyEffect(StatusEffect effect) {
        effects.push_back(effect);
    }

    void update(float dt) {

        currentSpeed = baseSpeed;

        for (auto& effect : effects) {

            effect.duration -= dt;

            switch (effect.type) {

            case StatusType::Burn:
                takeDamage(effect.value * dt);
                break;

            case StatusType::Slow:
                currentSpeed = baseSpeed * (1.0f - effect.value);
                break;

            case StatusType::Stun:
                currentSpeed = 0;
                break;
            }
        }

        effects.erase(
            std::remove_if(effects.begin(), effects.end(),
                [](StatusEffect& e) { return e.duration <= 0; }),
            effects.end()
        );

        position.x += currentSpeed * dt;
    }
};

////////////////////////////////////////////////////////////
// TOWER BASE CLASS
////////////////////////////////////////////////////////////

class Tower {
protected:
    Vector2 position;
    float damage;
    float range;
    float attackSpeed;
    float cooldown;

public:
    Tower(Vector2 pos)
        : position(pos), cooldown(0) {}

    virtual ~Tower() = default;

    virtual void update(float dt,
                        std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
};

////////////////////////////////////////////////////////////
// FIRE TOWER
////////////////////////////////////////////////////////////

class FireTower : public Tower {
public:
    FireTower(Vector2 pos)
        : Tower(pos)
    {
        damage = 20;
        range = 150;
        attackSpeed = 1.0f;
    }

    void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) override
    {
        cooldown -= dt;

        for (auto& enemy : enemies) {

            if (!enemy->alive) continue;

            if (distance(position, enemy->position) <= range) {

                if (cooldown <= 0) {

                    enemy->takeDamage(damage);

                    enemy->applyEffect({
                        StatusType::Burn,
                        3.0f,
                        10.0f
                    });

                    std::cout << "Burn applied!\n";

                    cooldown = attackSpeed;
                }

                break;
            }
        }
    }
};

////////////////////////////////////////////////////////////
// ICE TOWER
////////////////////////////////////////////////////////////

class IceTower : public Tower {
public:
    IceTower(Vector2 pos)
        : Tower(pos)
    {
        damage = 10;
        range = 150;
        attackSpeed = 2.0f;
    }

    void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) override
    {
        cooldown -= dt;

        for (auto& enemy : enemies) {

            if (!enemy->alive) continue;

            if (distance(position, enemy->position) <= range) {

                if (cooldown <= 0) {

                    enemy->takeDamage(damage);

                    enemy->applyEffect({
                        StatusType::Stun,
                        1.5f,
                        0
                    });

                    std::cout << "Enemy stunned!\n";

                    cooldown = attackSpeed;
                }

                break;
            }
        }
    }
};

////////////////////////////////////////////////////////////
// MAIN (ONLY ONE)
////////////////////////////////////////////////////////////

int main() {

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;

    enemies.push_back(std::make_unique<Enemy>(100, 100));

    towers.push_back(std::make_unique<FireTower>(Vector2{50, 100}));
    towers.push_back(std::make_unique<IceTower>(Vector2{60, 120}));

    for (int frame = 0; frame < 100; frame++) {

        float dt = 0.16f;

        for (auto& enemy : enemies)
            enemy->update(dt);

        for (auto& tower : towers)
            tower->update(dt, enemies);

        if (!enemies[0]->alive)
            break;
    }

    return 0;
}