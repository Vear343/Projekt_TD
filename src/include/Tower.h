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

public:
    Tower(float p_x, float p_y, SDL_Texture* p_texture);
    virtual ~Tower() = default;

    // ฟังก์ชันสำหรับอัพเดตสถานะของป้อม เช่น การยิงศัตรู
    virtual void updateTower(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
    
    // ฟังก์ชันสำหรับวาดป้อมบนหน้าจอ
    virtual void render(SDL_Renderer* renderer);
    void update(float deltaTime) override { }
};

////////////////////////////////////////////////////
class FireTower : public Tower {
public:
    FireTower(Vector2D pos, SDL_Texture* p_texture);
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};

////////////////////////////////////////////////////
class IceTower : public Tower {
public:
    IceTower(Vector2D pos, SDL_Texture* p_texture);
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};
////////////////////////////////////////////////////
// 1. WIND TOWER (พัดถอยหลัง)
////////////////////////////////////////////////////
class WindTower : public Tower {
public:
    WindTower(Vector2D pos, SDL_Texture* tex) : Tower(pos.x, pos.y, tex) {
        damage = 1.0f; range = 100.0f; attackSpeed = 2.0f;
    }
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};

////////////////////////////////////////////////////
// 2. LIGHT TOWER (ปั๊มเงิน - ต้องการ Pointer ไปที่ระบบเงิน)
////////////////////////////////////////////////////
class LightTower : public Tower {
    float& playerMoney; // อ้างอิงเงินใน Game.cpp
public:
    LightTower(Vector2D pos, SDL_Texture* tex, float& money) 
        : Tower(pos.x, pos.y, tex), playerMoney(money) {
        attackSpeed = 20.0f; // เจนเงินทุก 20 วิ
    }
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};

////////////////////////////////////////////////////
// 3. LIGHTNING TOWER (ชิ่ง 3 ตัว)
////////////////////////////////////////////////////
class LightningTower : public Tower {
public:
    LightningTower(Vector2D pos, SDL_Texture* tex) : Tower(pos.x, pos.y, tex) {
        damage = 15.0f; range = 130.0f; attackSpeed = 1.5f;
    }
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};

////////////////////////////////////////////////////
// 4. WATER TOWER (สโลว์พื้น)
////////////////////////////////////////////////////
class WaterTower : public Tower {
public:
    WaterTower(Vector2D pos, SDL_Texture* tex) : Tower(pos.x, pos.y, tex) {
        range = 150.0f; attackSpeed = 3.0f;
    }
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};