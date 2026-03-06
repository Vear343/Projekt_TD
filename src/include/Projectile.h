#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Entity.h>
#include <Enemy.h>

class Projectile : public Entity{
public:
    Projectile(float x, float y, SDL_Texture* texture, Enemy* target);

    void update(float deltatime);
    bool hasHit();
private:
    const float speed = 120;
    const float damage = 20;

    Enemy* target;
    bool hashit = false;
};