#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Projectile.h"
#include <vector>
#include <memory>

class Tower : public Entity
{
protected:
    float damage;
    float range;
    float attackSpeed;
    float cooldown;
    SDL_Texture* projectileTexture; // Texture for projectiles

public:
    Tower(float p_x, float p_y, SDL_Texture* p_texture, SDL_Texture* p_projectile_texture);
    virtual ~Tower() = default;

    // ฟังก์ชันสำหรับอัพเดตสถานะของป้อม เช่น การยิงศัตรู
    virtual void updateTower(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
    
    // ฟังก์ชันสำหรับวาดป้อมบนหน้าจอ
    virtual void render(SDL_Renderer* renderer);
    void update(float deltaTime) override { }

    std::vector<std::unique_ptr<Projectile>> projectiles; // เก็บกระสุนที่ป้อมยิงออกไป

    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies);
};

////////////////////////////////////////////////////
class FireTower : public Tower {
public:
    FireTower(Vector2D pos, SDL_Texture* p_texture, SDL_Texture* p_projectile_texture);
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};

////////////////////////////////////////////////////
class IceTower : public Tower {
public:
    IceTower(Vector2D pos, SDL_Texture* p_texture, SDL_Texture* p_projectile_texture);
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};