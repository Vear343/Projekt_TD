#pragma once
#include <vector>
#include <memory>
#include "Vector2.h"
#include "Enemy.h"

class Tower
{
protected:
    Vector2 position;
    float damage;
    float range;
    float attackSpeed;
    float cooldown;

public:
    Tower(Vector2 pos);
    virtual ~Tower();

    virtual void update(float dt,
    std::vector<Enemy>& enemies) = 0;
};

////////////////////////////////////////////////////
// FIRE TOWER
////////////////////////////////////////////////////

class FireTower : public Tower
{
public:
    FireTower(Vector2 pos);

    void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) override;
};

////////////////////////////////////////////////////
// ICE TOWER
////////////////////////////////////////////////////

class IceTower : public Tower
{
public:
    IceTower(Vector2 pos);

    void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) override;
};