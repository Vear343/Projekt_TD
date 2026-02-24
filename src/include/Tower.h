#pragma once
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include "Vector2D.h"
#include "Enemy.h"

class Tower
{
protected:
    Vector2D position;
    float damage;
    float range;
    float attackSpeed;
    float cooldown;

public:
    Tower(Vector2D pos);
    virtual ~Tower() = default;

    virtual void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) = 0;

    virtual void render(SDL_Renderer* renderer) = 0;
};

////////////////////////////////////////////////////
// FIRE TOWER
////////////////////////////////////////////////////

class FireTower : public Tower
{
public:
    FireTower(Vector2D pos);

    void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) override;

    void render(SDL_Renderer* renderer) override;
};

////////////////////////////////////////////////////
// ICE TOWER
////////////////////////////////////////////////////

class IceTower : public Tower
{
public:
    IceTower(Vector2D pos);

    void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies) override;

    void render(SDL_Renderer* renderer) override;
};