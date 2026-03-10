#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Entity.h"   // ต้องมีเพื่อสืบทอด x, y, texture
#include "Math.h"     // ต้องมีเพื่อให้รู้จัก Vector2D

class Enemy : public Entity { // ต้องสืบทอดจาก Entity
public:
    Enemy(float p_x, float p_y, SDL_Texture* p_tex, const std::vector<Vector2D>& p_path);

    void renderHpbar(SDL_Renderer* renderer);
    void update(float deltaTime);
    void takeDamage(float amount);
    void applyStunt(float duration);
    void applySlow(float speedModifier, float duration);
    void applyBurn(float dmg, float duration);
    void pushBack(float distance);
    // Getters
    float getHp() const { return hp; }
    int getReward() const { return reward; }
    void setreward(int r) { reward = r; }
    void setMaxHp(float p_hp) { maxHp = p_hp; hp = p_hp; }
    void setSpeed(float p_speed) { speed = p_speed; }
    bool isAlive() const { return hp > 0; }
    bool hasFinished() const { return finished; }
    bool isRewardGiven() const { return rewardGiven; }
    void setRewardGiven(bool b) { rewardGiven = b; }

    // Helper functions สำหรับ Tower/Projectile
    float getX() const { return x; }
    float getY() const { return y; }
    Vector2D getCenter() { return Vector2D(x, y); }

private:
    std::vector<Vector2D> path;
    int currentPathIndex;
    SDL_Rect collider;

    int reward;
    float maxHp;
    float hp;
    float speed;
    bool alive;
    bool finished;
    bool rewardGiven;
    bool reachedEnd;

    // Status Timers
    float stunTimer;
    float stunCooldown;
    float burnTimer;
    float burnTickTimer;
    float burnDamagePerSec;
    float slowTimer;
    float speedMultiplier;
};

class BossEnemy : public Enemy {
public:
    BossEnemy(float p_x, float p_y, SDL_Texture* p_texture, const std::vector<Vector2D>& p_path)
    : Enemy(p_x, p_y, p_texture, p_path) {
    setMaxHp(800.0f);
    setSpeed(17.5f);
    setreward(500);
    width = 96;
    height = 96;
    }
};