#include "Tower.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Tower::Tower(Vector2D pos, SDL_Texture* p_texture, SDL_Texture* p_projectile_texture)
    : Entity(pos.x, pos.y, 64, 64, p_texture),
      damage(0), range(0), attackSpeed(0), cooldown(0),
      projectileTexture(p_projectile_texture)
{
    collider.w = static_cast<int>(width);
    collider.h = static_cast<int>(height);
}

Enemy* Tower::findTarget(std::vector<std::unique_ptr<Enemy>>& enemies) {
    Enemy* closestEnemy = nullptr;
    float closestDistSq = range * range;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float dx = x - enemy->getX();
        float dy = y - enemy->getY();
        float distSq = dx * dx + dy * dy;

        if (distSq <= closestDistSq) {
            closestDistSq = distSq;
            closestEnemy = enemy.get();
        }
    }

    return closestEnemy;
}

void Tower::updateProjectiles(float dt) {
    for (auto& p : projectiles)
        p->update(dt);

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
        [](const std::unique_ptr<Projectile>& p) { return p->hasHit(); }),
        projectiles.end());
}

void FireTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (cooldown > 0) cooldown -= dt;

    if (cooldown <= 0) {
        Enemy* target = findTarget(enemies);

        if (target) {
            projectiles.push_back(std::make_unique<Projectile>(
                x,
                y,
                projectileTexture,
                target,
                ProjectileEffect::BURN
            ));

            target->takeDamage(damage);
            cooldown = attackSpeed;
        }
    }

    for (auto& p : projectiles)
        p->update(dt);

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
        [](const std::unique_ptr<Projectile>& p) { return p->hasHit(); }),
        projectiles.end());
}

void IceTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (cooldown > 0) cooldown -= dt;

    if (cooldown <= 0) {
        Enemy* target = findTarget(enemies);

        if (target) {
            projectiles.push_back(std::make_unique<Projectile>(
                x,
                y,
                projectileTexture,
                target,
                ProjectileEffect::STUNT
            ));

            target->takeDamage(damage);
            cooldown = attackSpeed;
        }
    }

    for (auto& p : projectiles)
        p->update(dt);

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
        [](const std::unique_ptr<Projectile>& p) { return p->hasHit(); }),
        projectiles.end());
}

void WindTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    cooldown -= dt;

    if (cooldown <= 0) {
        for (auto& enemy : enemies) {
            if ((getCenter() - enemy->getCenter()).length() <= range) {
                enemy->takeDamage(damage);
                enemy->pushBack(30.0f);
                cooldown = attackSpeed;
                break;
            }
        }
    }
}

void LightTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    cooldown -= dt;

    if (cooldown <= 0) {
        playerMoney += 50.0f;
        cooldown = attackSpeed;

        std::cout << "Light Tower generated 50 gold, Current: "
                  << playerMoney << std::endl;
    }
}

void LightningTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    cooldown -= dt;

    if (cooldown <= 0) {
        int targetsHit = 0;
        float currentDamage = damage;

        for (auto& enemy : enemies) {
            if (targetsHit >= 3) break;

            if ((getCenter() - enemy->getCenter()).length() <= range) {
                enemy->takeDamage(currentDamage);
                currentDamage *= 0.6f;
                targetsHit++;
            }
        }

        if (targetsHit > 0)
            cooldown = attackSpeed;
    }
}

void WaterTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    cooldown -= dt;

    if (cooldown <= 0) {
        for (auto& enemy : enemies) {
            if ((getCenter() - enemy->getCenter()).length() <= range) {
                enemy->applySlow(0.5f, 2.0f);
                cooldown = attackSpeed;
                break;
            }
        }
    }
}
