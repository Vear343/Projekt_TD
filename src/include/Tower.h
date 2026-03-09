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

    virtual void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies
    ) = 0;

    void update(float deltaTime) override {}

    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies);
    void updateProjectiles(float dt);
};

class FireTower : public Tower {
public:
    FireTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 20.0f;
        range = 240.0f;
        attackSpeed = 1.0f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies
    ) override;
};

class IceTower : public Tower {
public:
    IceTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 5.0f;
        range = 200.0f;
        attackSpeed = 1.0f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies
    ) override;
};

class WindTower : public Tower {
public:
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
        std::vector<std::unique_ptr<Enemy>>& enemies
    ) override;
};

class LightTower : public Tower {
    float& playerMoney;

public:
    LightTower(
        Vector2D pos,
        SDL_Texture* tex,
        float& money
    )
        : Tower(pos, tex, nullptr),
          playerMoney(money) {
        attackSpeed = 20.0f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies
    ) override;
};

class LightningTower : public Tower {
public:
    LightningTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 15.0f;
        range = 130.0f;
        attackSpeed = 1.5f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies
    ) override;
};

class WaterTower : public Tower {
public:
    WaterTower(
        Vector2D pos,
        SDL_Texture* tex,
        SDL_Texture* projectileTex
    )
        : Tower(pos, tex, projectileTex) {
        damage = 0.0f;
        range = 150.0f;
        attackSpeed = 2.5f;
    }

    void updateTower(
        float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies
    ) override;
};