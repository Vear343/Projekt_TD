#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <algorithm>

Enemy::Enemy(float p_x, float p_y, SDL_Texture* p_texture, const std::vector<Vector2D>& p_path)
    : Entity(p_x, p_y, 64, 64, p_texture), 
      path(p_path), 
      currentPathIndex(0), 
      reward(50),
      maxHp(100.0f),
      hp(maxHp),
      speed(30.0f),
      alive(true), 
      finished(false),
      rewardGiven(false),
      reachedEnd(false),
      stunTimer(0.0f),
      burnTimer(0.0f),
      burnTickTimer(0.0f),
      burnDamagePerSec(0.0f),
      slowTimer(0.0f),
      speedMultiplier(1.0f)
{
    collider.w = 32;
    collider.h = 32;
}

void Enemy::renderHpbar(SDL_Renderer* renderer)
{
    SDL_Rect dest = {(int)x, (int)y, (int)width, (int)height};

    // Draw enemy
    SDL_RenderCopy(renderer, texture, &currentFrame, &dest);

    // HP bar settings
    int barWidth = width;
    int barHeight = 6;

    int barX = (int)(x - width / 2);
    int barY = (int)(y - height / 2 - 10);

    float hpPercent = hp / maxHp;

    SDL_Rect bgBar = {barX, barY, barWidth, barHeight};
    SDL_Rect hpBar = {barX, barY, (int)(barWidth * hpPercent), barHeight};

    // background
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &bgBar);

    // HP
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &hpBar);

    // border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bgBar);
}

void Enemy::update(float deltaTime) {
    // Check if enemy is dead - end movement and updates
    if (hp <= 0) {
        alive = false;
        SDL_SetTextureColorMod(texture, 255, 255, 255); // Reset color when dead
        return;
    }

    if (finished) return;

    // 1. Status Effects
    if (stunCooldown > 0) {
        stunCooldown -= deltaTime;
    }

    if (stunTimer > 0) {
        stunTimer -= deltaTime;
        SDL_SetTextureColorMod(texture, 150, 150, 255);
        return; 
    }

    if (slowTimer > 0) {
        slowTimer -= deltaTime;
        speedMultiplier = 0.5f;
        SDL_SetTextureColorMod(texture, 150, 255, 255);
    } else {
        speedMultiplier = 1.0f;
    }

    if (burnTimer > 0) {
        burnTimer -= deltaTime;
        burnTickTimer += deltaTime;
        SDL_SetTextureColorMod(texture, 255, 100, 100);
        if (burnTickTimer >= 1.0f) {
            takeDamage(burnDamagePerSec);
            burnTickTimer = 0.0f; 
        }
    } 

    if (stunTimer <= 0 && slowTimer <= 0 && burnTimer <= 0) {
        SDL_SetTextureColorMod(texture, 255, 255, 255);
    }

    // 2. Movement

    if (currentPathIndex >= path.size()) {
        finished = true;
        return;
    }

    Vector2D target = path[currentPathIndex];
    Vector2D currentPos(x, y);
    Vector2D direction = target - currentPos;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        currentPathIndex++;
        return;
    }

    x += (direction.x / distance) * speed * speedMultiplier * deltaTime;
    y += (direction.y / distance) * speed * speedMultiplier * deltaTime;

    collider.x = static_cast<int>(x - (collider.w / 2));
    collider.y = static_cast<int>(y - (collider.h / 2));
}

void Enemy::takeDamage(float dmg) {
    if (hp <= 0) return; // Already dead, skip damage
    
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        alive = false;
    }
}

void Enemy::applySlow(float speedModifier, float duration) {
    speedMultiplier = speedModifier;
    slowTimer = std::max(slowTimer, duration);
}

void Enemy::applyStunt(float duration) {
    if (stunCooldown > 0) return; // ยังอยู่ในช่วง Cooldown ของ Stun

    stunTimer = std::max(stunTimer, duration);
    stunCooldown = 6.0f; // เริ่ม Cooldown ทันทีหลังจากถูก Stun
}

void Enemy::applyBurn(float dmg, float duration) {
    burnDamagePerSec = dmg;
    burnTimer = std::max(burnTimer, duration);
}

void Enemy::pushBack(float distance) {
    if (currentPathIndex > 0) {
        Vector2D pushDir = path[currentPathIndex - 1] - Vector2D(x, y);
        float dist = std::sqrt(pushDir.x * pushDir.x + pushDir.y * pushDir.y);
        if (dist > 0) {
            x += (pushDir.x / dist) * distance;
            y += (pushDir.y / dist) * distance;
        }
    }
}