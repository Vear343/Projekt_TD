#include "Tower.h"
#include <cmath>

////////////////////////////////////////////////////
// BASE
////////////////////////////////////////////////////

Tower::Tower(Vector2 pos)
    : position(pos),
      damage(0),
      range(0),
      attackSpeed(0),
      cooldown(0)
{
}

Tower::~Tower() = default;

////////////////////////////////////////////////////
// FIRE TOWER
////////////////////////////////////////////////////

FireTower::FireTower(Vector2 pos)
    : Tower(pos)
{
    damage = 20;
    range = 150;
    attackSpeed = 1.0f;
}

void FireTower::update(float dt,
    std::vector<std::unique_ptr<Enemy>>& enemies)
{
    cooldown -= dt;

    for (auto& enemy : enemies)
    {
        if (!enemy->alive) continue;

        float dx = position.x - enemy->position.x;
        float dy = position.y - enemy->position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range && cooldown <= 0)
        {
            enemy->takeDamage(damage);
            cooldown = attackSpeed;
            break;
        }
    }
}

////////////////////////////////////////////////////
// ICE TOWER
////////////////////////////////////////////////////

IceTower::IceTower(Vector2 pos)
    : Tower(pos)
{
    damage = 10;
    range = 150;
    attackSpeed = 2.0f;
}

void IceTower::update(float dt,
    std::vector<std::unique_ptr<Enemy>>& enemies)
{
    cooldown -= dt;

    for (auto& enemy : enemies)
    {
        if (!enemy->alive) continue;

        float dx = position.x - enemy->position.x;
        float dy = position.y - enemy->position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range && cooldown <= 0)
        {
            enemy->takeDamage(damage);
            cooldown = attackSpeed;
            break;
        }
    }
}