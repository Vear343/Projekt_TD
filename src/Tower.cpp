#include "Tower.h"
#include <cmath>

Tower::Tower(float p_x, float p_y, SDL_Texture* p_texture)
    : Entity(p_x, p_y, 32, 32, p_texture), // กำหนดขนาดเริ่มต้น 32x32
      damage(0), range(0), attackSpeed(0), cooldown(0) 
{}

void Tower::render(SDL_Renderer* renderer) {
    // วาดตัวป้อมโดยใช้ข้อมูลจาก Entity
    SDL_Rect src = getCurrentFrame();
    SDL_Rect dst = { (int)x, (int)y, (int)width, (int)height };
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

////////////////////////////////////////////////////
// FIRE TOWER: ทำดาเมจ + ติดไฟ (Burn) 3 วินาที
////////////////////////////////////////////////////
FireTower::FireTower(Vector2D pos, SDL_Texture* p_texture)
    : Tower(pos.x, pos.y, p_texture)
{
    damage = 20.0f;
    range = 150.0f;
    attackSpeed = 1.0f; // ยิงทุก 1 วินาที
}

void FireTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (cooldown > 0) cooldown -= dt;

    if (cooldown <= 0) {
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;

            float dx = (x + width/2) - enemy->getX();
            float dy = (y + height/2) - enemy->getY();
            float distSq = dx*dx + dy*dy; // ใช้ Squared Distance เพื่อลดภาระ CPU

            if (distSq <= range * range) {
                enemy->takeDamage(damage);
                enemy->applyBurn(5.0f, 3.0f); // ดาเมจเผา 5, นาน 3 วิ
                cooldown = attackSpeed;
                break;
            }
        }
    }
}

////////////////////////////////////////////////////
// ICE TOWER: ดาเมจน้อย + สตั้น (Stun) 0.5 วินาที
////////////////////////////////////////////////////
IceTower::IceTower(Vector2D pos, SDL_Texture* p_texture)
    : Tower(pos.x, pos.y, p_texture) 
{
    damage = 5.0f;
    range = 120.0f;
    attackSpeed = 2.0f;
}

void IceTower::updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (cooldown > 0) cooldown -= dt;

    if (cooldown <= 0) {
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;

            float dx = (x + width/2) - enemy->getX();
            float dy = (y + height/2) - enemy->getY();
            float distSq = dx*dx + dy*dy;

            if (distSq <= range * range) {
                enemy->takeDamage(damage);
                enemy->applyStun(0.5f); // หยุดเดิน 0.5 วิ
                cooldown = attackSpeed;
                break;
            }
        }
    }
}