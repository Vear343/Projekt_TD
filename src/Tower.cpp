#include "Tower.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Tower::Tower(float p_x, float p_y, SDL_Texture* p_texture, SDL_Texture* p_projectile_texture)
    : Entity(p_x, p_y, 32, 32, p_texture), // กำหนดขนาดเริ่มต้น 32x32
      damage(0), range(0), attackSpeed(0), cooldown(0), projectileTexture(p_projectile_texture)
{}

void Tower::render(SDL_Renderer* renderer) {
    // If tower has no texture, skip rendering
    if (!texture) return;

    // Query actual texture dimensions for source rectangle
    int texWidth, texHeight;
    SDL_QueryTexture(texture, NULL, NULL, &texWidth, &texHeight);

    // Define the source rectangle (use full texture)
    SDL_Rect src = { 0, 0, texWidth, texHeight };

    // Define the destination rectangle (center on tower position, use tower size)
    SDL_Rect dst;
    dst.x = (int)(x - (width / 2.0f));  // Center on x-axis
    dst.y = (int)(y - (height / 2.0f)); // Center on y-axis
    dst.w = (int)width;   // Use tower's width
    dst.h = (int)height;  // Use tower's height

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

Enemy* Tower::findTarget(std::vector<std::unique_ptr<Enemy>>& enemies) {
    Enemy* closestEnemy = nullptr;
    float closestDistSq = range * range; // ใช้ Squared Distance

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float dx = (x + width/2) - enemy->getX();
        float dy = (y + height/2) - enemy->getY();
        float distSq = dx*dx + dy*dy;

        if (distSq <= closestDistSq) {
            closestDistSq = distSq;
            closestEnemy = enemy.get();
        }
    }
    return closestEnemy;
}

////////////////////////////////////////////////////
// FIRE TOWER: ทำดาเมจ + ติดไฟ (Burn) 3 วินาที
////////////////////////////////////////////////////
FireTower::FireTower(Vector2D pos, SDL_Texture* p_texture, SDL_Texture* p_projectile_texture)
    : Tower(pos.x, pos.y, p_texture, p_projectile_texture)
{
    damage = 20.0f;
    range = 240.0f;
    attackSpeed = 1.0f; // ยิงทุก 1 วินาที
}

void FireTower::updateTower(float deltatime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (cooldown > 0) cooldown -= deltatime;

    if (cooldown <= 0) {
        Enemy* target = findTarget(enemies);
        if (target) {
            projectiles.push_back(std::make_unique<Projectile>(
                x + width/2, 
                y + height/2, 
                projectileTexture, 
                target,
                ProjectileEffect::BURN
            ));
            cooldown = attackSpeed;
        }
    }
    // ยิง projectile
    for (auto& p : projectiles)
        p->update(deltatime);

    // ลบ projectile ที่โดนเป้าหมายแล้ว
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const std::unique_ptr<Projectile>& p) { return p->hasHit(); }),
        projectiles.end());
}

////////////////////////////////////////////////////
// ICE TOWER: ดาเมจน้อย + สตั้น (Stun) 0.5 วินาที
////////////////////////////////////////////////////
IceTower::IceTower(Vector2D pos, SDL_Texture* p_texture, SDL_Texture* p_projectile_texture)
    : Tower(pos.x, pos.y, p_texture, p_projectile_texture) 
{
    damage = 5.0f;
    range = 200.0f;
    attackSpeed = 2.0f;
}

void IceTower::updateTower(float deltatime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (cooldown > 0) cooldown -= deltatime;

    if (cooldown <= 0) {
        Enemy* target = findTarget(enemies);
        if (target) {
            projectiles.push_back(std::make_unique<Projectile>(
                x + width/2, 
                y + height/2, 
                projectileTexture, 
                target,
                ProjectileEffect::SLOW
            ));
            cooldown = attackSpeed;
        }
    }

    // ยิง projectile
    for (auto& p : projectiles)
        p->update(deltatime);

    // ลบ projectile ที่โดนเป้าหมายแล้ว
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const std::unique_ptr<Projectile>& p) { return p->hasHit(); }),
        projectiles.end());
}