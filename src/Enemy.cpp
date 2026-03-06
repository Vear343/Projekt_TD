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
      SlowTimer(0.0f),
      burnTimer(0.0f),
      burnTickTimer(0.0f),
      burnDamagePerSec(0.0f)
{
    // ตั้งค่า Collider เริ่มต้นให้ตรงกับตำแหน่งเกิด
    collider.w = static_cast<int>(width);
    collider.h = static_cast<int>(height);
    collider.x = static_cast<int>(x);
    collider.y = static_cast<int>(y);
}

void Enemy::update(float deltaTime) {
    if (!alive || finished) return;

    // --- 1. จัดการระบบ SLOW ---
    if (SlowTimer > 0) {
        SlowTimer -= deltaTime;
        // ปรับสีให้ดูตัวแข็ง/มึน (สีฟ้าอ่อน)
        SDL_SetTextureColorMod(texture, 150, 150, 255); 
        return; // หยุดการทำงานด้านล่างทั้งหมด (ไม่ขยับ)
    }

    // --- 2. จัดการระบบ BURN (Damage Over Time) ---
    if (burnTimer > 0) {
        burnTimer -= deltaTime;
        burnTickTimer += deltaTime;

        // ย้อมสีตัวละครเป็นสีส้ม/แดง เมื่อติดไฟ
        SDL_SetTextureColorMod(texture, 255, 100, 100);

        if (burnTickTimer >= 1.0f) { // ทำดาเมจทุกๆ 1 วินาที
            takeDamage(burnDamagePerSec);
            burnTickTimer = 0.0f; 
        }
    } else {
        // ถ้าไม่ติดสถานะอะไรเลย ให้คืนสีปกติ
        SDL_SetTextureColorMod(texture, 255, 255, 255);
    }

    // --- 3. เช็คว่าตายหรือยัง (หลังจากโดนสถานะต่างๆ) ---
    if (Hp <= 0) {
        alive = false;
        return;
    }

    // --- 4. Logic การเคลื่อนที่ตาม Path ---
    if (currentPathIndex >= path.size()) {
        finished = true;
        return;
    }

    Vector2D target = path[currentPathIndex];
    Vector2D currentPos(x, y);
    Vector2D direction = target - currentPos;

    // คำนวณระยะห่าง
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // ถ้าใกล้จุดเป้าหมายมากพอ ให้เปลี่ยนไปจุดถัดไป
    if (distance < 5.0f) {
        currentPathIndex++;
        return;
    }

    // ทำ Direction ให้เป็น Unit Vector (Normalize)
    direction.x /= distance;
    direction.y /= distance;

    // เคลื่อนที่ตามทิศทางและความเร็ว
    x += direction.x * speed * deltaTime;
    y += direction.y * speed * deltaTime;

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
    // ใช้ std::max เผื่อกรณีโดนยิงซ้ำ จะได้ยึดเวลาที่นานที่สุด
    speed *= speedModifier; // ลดความเร็วลงตาม modifier
}

void Enemy::applyBurn(float dmg, float duration) {
    burnDamagePerSec = dmg;
    burnTimer = std::max(burnTimer, duration);
}