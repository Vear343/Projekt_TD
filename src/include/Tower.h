#pragma once
#include "Entity.h"
#include "Enemy.h"
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
    // ส่งค่าไปยัง Entity Constructor
    Tower(float p_x, float p_y, SDL_Texture* p_texture);
    virtual ~Tower() = default;

    // ฟังก์ชัน update ที่รับ enemies เพื่อหาเป้าหมาย
    virtual void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
    
    // ใช้ render ของ Entity หรือเขียนทับถ้าต้องการวาด Range วงกลม
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
    IceTower(float p_x, float p_y, SDL_Texture* p_texture);
    void updateTower(float dt, std::vector<std::unique_ptr<Enemy>>& enemies) override;
};