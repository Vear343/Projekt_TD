#pragma once

#include "Entity.h"
#include "Enemy.h"
#include "Projectile.h"
#include <memory>
#include <vector>

class Tower : public Entity {
protected:
    float damage;
    float range;
    float attackSpeed;
    float cooldown = 0.0f;
    
    SDL_Texture* projectileTexture;
    
public:
    std::vector<std::unique_ptr<Projectile>> projectiles;

    Tower(
        Vector2D pos,
        SDL_Texture* texture,
        SDL_Texture* projectileTex = nullptr
    );

    virtual ~Tower() = default;

    virtual int getCost() const = 0; // ทำให้ Tower เป็น Abstract Class ที่ไม่สามารถสร้าง Object ได้โดยตรง

    virtual void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        bool waveActive
    ) = 0;

    void update(float deltaTime) override {}

    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies);
    void updateProjectiles(float dt);
};

class FireTower : public Tower {
public:
    static constexpr int cost = 100;

    int getCost() const override { return cost; }

    FireTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 10.0f;
        range = 240.0f;
        attackSpeed = 2.0f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        bool waveActive
    ) override;
};

class IceTower : public Tower {
public:
    static constexpr int cost = 150;

    int getCost() const override { return cost; }

    IceTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 5.0f;
        range = 200.0f;
        attackSpeed = 1.5f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        bool waveActive
    ) override;
};

class WindTower : public Tower {
public:
    static constexpr int cost = 200;

    int getCost() const override { return cost; }

    WindTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 1.0f;
        range = 100.0f;
        attackSpeed = 2.0f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        bool waveActive
    ) override;
};

class LightTower : public Tower {
    float& playerMoney;

public:
    static constexpr int cost = 125;

    int getCost() const override { return cost; }

    LightTower(
        Vector2D pos,
        SDL_Texture* tex,
        float& money
    )
        : Tower(pos, tex, nullptr),
          playerMoney(money) {
        attackSpeed = 10.0f;
        cooldown = attackSpeed; // เริ่มต้นให้พร้อมทำงานทันที
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        bool waveActive
    ) override;
};

class LightningTower : public Tower {
public:
    static constexpr int cost = 175;

    int getCost() const override { return cost; }

    LightningTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 12.0f;
        range = 130.0f;
        attackSpeed = 2.0f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        bool waveActive
    ) override;
};

class WaterTower : public Tower {
public:
    static constexpr int cost = 125;

    int getCost() const override { return cost; }

    WaterTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 5.0f;
        range = 150.0f;
        attackSpeed = 2.0f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        bool waveActive
    ) override;
};