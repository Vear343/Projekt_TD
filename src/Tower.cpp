#include "Tower.h"
#include <cmath>

////////////////////////////////////////////////////
// BASE
////////////////////////////////////////////////////

Tower::Tower(Vector2D pos)
    : position(pos),
      damage(0),
      range(0),
      attackSpeed(0),
      cooldown(0)
{
}

////////////////////////////////////////////////////
// FIRE TOWER
////////////////////////////////////////////////////

FireTower::FireTower(Vector2D pos)
    : Tower(pos)
{
    damage = 20.0f;
    range = 150.0f;
    attackSpeed = 1.0f;
}

void FireTower::update(float dt,
    std::vector<std::unique_ptr<Enemy>>& enemies)
{
    cooldown -= dt;

    for (auto& enemy : enemies)
    {
        if (!enemy->isAlive()) continue;

        Vector2D enemyPos = enemy->getPosition();

        float dx = position.x - enemyPos.x;
        float dy = position.y - enemyPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range && cooldown <= 0)
        {
            enemy->takeDamage(damage);
            cooldown = attackSpeed;
            break;
        }
    }
}

void FireTower::render(SDL_Renderer* renderer)
{
    SDL_Rect rect = { (int)position.x - 10, (int)position.y - 10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

////////////////////////////////////////////////////
// ICE TOWER
////////////////////////////////////////////////////

IceTower::IceTower(Vector2D pos)
    : Tower(pos)
{
    damage = 10.0f;
    range = 150.0f;
    attackSpeed = 2.0f;
}

void IceTower::update(float dt,
    std::vector<std::unique_ptr<Enemy>>& enemies)
{
    cooldown -= dt;

    for (auto& enemy : enemies)
    {
        if (!enemy->isAlive()) continue;

        Vector2D enemyPos = enemy->getPosition();

        float dx = position.x - enemyPos.x;
        float dy = position.y - enemyPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range && cooldown <= 0)
        {
            enemy->takeDamage(damage);
            cooldown = attackSpeed;
            break;
        }
    }
}

void IceTower::render(SDL_Renderer* renderer)
{
    SDL_Rect rect = { (int)position.x - 10, (int)position.y - 10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}