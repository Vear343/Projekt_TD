#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Entity.h"   // ต้องมีเพื่อสืบทอด x, y, texture
#include "Math.h"     // ต้องมีเพื่อให้รู้จัก Vector2D

class Enemy : public Entity { // ต้องสืบทอดจาก Entity
public:
    Enemy(float p_x, float p_y, SDL_Texture* p_tex, const std::vector<Vector2D>& p_path);

    void update(float deltaTime);
    void takeDamage(float dmg);
    
    // Status Effect Methods (ต้องประกาศเพื่อให้ Tower เรียกใช้ได้)
    void applyStun(float duration);
    void applyBurn(float dmg, float duration);
    void applySlow(float duration);
    void pushBack(float distance);

    // Getters
    float getHp() const { return hp; }
    void setHp(float p_hp) { hp = p_hp; }
    void setSpeed(float p_speed) { speed = p_speed; }
    
    bool isAlive() const { return hp > 0; }
    bool hasFinished() const { return finished; }
    bool isRewardGiven() const { return rewardGiven; }
    void setRewardGiven(bool b) { rewardGiven = b; }

    // Helper functions สำหรับ Tower/Projectile
    float getX() const { return x; }
    float getY() const { return y; }
    Vector2D getCenter() { return Vector2D(x + 32, y + 32); }
    SDL_Rect getCollider() { return collider; }

private:
    std::vector<Vector2D> path;
    int currentPathIndex;
    SDL_Rect collider;

    float hp;
    float speed;
    bool alive;
    bool finished;
    bool rewardGiven;
    bool reachedEnd;

    // Status Timers
    float stunTimer;
    float burnTimer;
    float burnTickTimer;
    float burnDamagePerSec;
    float slowTimer;
    float speedMultiplier;
};