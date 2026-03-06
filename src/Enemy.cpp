#include "Enemy.h"
#include <cmath>
#include <algorithm>

Enemy::Enemy(float p_x, float p_y, SDL_Texture* p_texture, const std::vector<Vector2D>& p_path)
    : Entity(p_x, p_y, width, height, p_texture), 
      path(p_path), 
      currentPathIndex(0), 
      alive(true), 
      finished(false),
      Hp(100.0f),
      speed(50.0f),
      burnTimer(0.0f),
      burnTickTimer(0.0f),
      burnDamagePerSec(0.0f),
      slowTimer(0.0f),      // อย่าลืมกำหนดค่าเริ่มต้น
      speedMultiplier(1.0f) 
{
    collider.w = static_cast<int>(width);
    collider.h = static_cast<int>(height);
    collider.x = static_cast<int>(x);
    collider.y = static_cast<int>(y);
}

void Enemy::update(float deltaTime) {
    if (!alive || finished) return;

    // --- 1. จัดการสถานะผิดปกติ (Status Effects) ---

    // ระบบ STUN (หยุดเดิน)
    if (stunTimer > 0) {
        stunTimer -= deltaTime;
        SDL_SetTextureColorMod(texture, 150, 150, 255); // สีฟ้า
        return; 
    }

    // ระบบ SLOW (ลดความเร็ว)
    if (slowTimer > 0) {
        slowTimer -= deltaTime;
        speedMultiplier = 0.5f;
        SDL_SetTextureColorMod(texture, 150, 255, 255); // สีฟ้าอ่อน/น้ำ
    } else {
        speedMultiplier = 1.0f;
    }

    // ระบบ BURN (ดาเมจต่อเนื่อง)
    if (burnTimer > 0) {
        burnTimer -= deltaTime;
        burnTickTimer += deltaTime;
        SDL_SetTextureColorMod(texture, 255, 100, 100); // สีส้มแดง
        if (burnTickTimer >= 1.0f) {
            takeDamage(burnDamagePerSec);
            burnTickTimer = 0.0f; 
        }
    } 

    // คืนสีปกติถ้าไม่มีสถานะอะไรเลย
    if (stunTimer <= 0 && slowTimer <= 0 && burnTimer <= 0) {
        SDL_SetTextureColorMod(texture, 255, 255, 255);
    }

    // --- 2. เช็คการตาย ---
    if (Hp <= 0) {
        alive = false;
        return;
    }

    // --- 3. การเคลื่อนที่ตาม Path ---
    if (currentPathIndex >= path.size()) {
        finished = true;
        return;
    }

    Vector2D target = path[currentPathIndex];
    Vector2D currentPos(x, y);
    Vector2D direction = target - currentPos;

    // หาความยาว (Distance) โดยใช้ length() จาก Vector2D หรือคำนวณสด
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        currentPathIndex++;
        return;
    }

    // Normalize ทิศทาง
    direction.x /= distance;
    direction.y /= distance;

    // คำนวณความเร็วสุดท้าย (ความเร็วพื้นฐาน * ตัวคูณสโลว์)
    float finalSpeed = speed * speedMultiplier;

    // อัปเดตตำแหน่ง
    x += direction.x * finalSpeed * deltaTime;
    y += direction.y * finalSpeed * deltaTime;

    // อัปเดตตำแหน่ง Collider
    collider.x = static_cast<int>(x);
    collider.y = static_cast<int>(y);
}

void Enemy::takeDamage(float dmg) {
    Hp -= dmg;
    if (Hp <= 0) {
        Hp = 0;
        alive = false;
    }
}

void Enemy::applySlow(float speedModifier, float duration) {
    speedMultiplier = speedModifier;
    slowTimer = std::max(slowTimer, duration);
}

void Enemy::applyStunt(float duration) {
    stunTimer = std::max(stunTimer, duration);
}

void Enemy::applyBurn(float dmg, float duration) {
    burnDamagePerSec = dmg;
    burnTimer = std::max(burnTimer, duration);
}

void Enemy::pushBack(float distance) {
    // ต้องมีอย่างน้อย 1 จุดใน path ถึงจะถอยได้
    if (currentPathIndex > 0) {
        Vector2D prevPoint = path[currentPathIndex - 1];
        Vector2D currentPos(x, y);
        Vector2D pushDir = prevPoint - currentPos;
        
        float dist = std::sqrt(pushDir.x * pushDir.x + pushDir.y * pushDir.y);
        if (dist > 0) {
            // ดันกลับไปทางจุดก่อนหน้า
            x += (pushDir.x / dist) * distance;
            y += (pushDir.y / dist) * distance;
        }
    } else {
        // ถ้าอยู่ที่จุดเริ่ม ให้ดันถอยหลังออกจากจุดเป้าหมายแรก
        Vector2D targetPoint = path[0];
        Vector2D pushDir = Vector2D(x, y) - targetPoint;
        float dist = std::sqrt(pushDir.x * pushDir.x + pushDir.y * pushDir.y);
        if (dist > 0) {
            x += (pushDir.x / dist) * distance;
            y += (pushDir.y / dist) * distance;
        }
    }
}