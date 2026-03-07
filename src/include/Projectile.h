#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Entity.h>
#include <Enemy.h>

enum class ProjectileEffect {
    NORMAL,
    BURN,
    STUNT,
    SLOW,
    
};

class Projectile : public Entity{   
public:
    Projectile(float x, float y, SDL_Texture* texture, Enemy* target, ProjectileEffect effect = ProjectileEffect::NORMAL);

    void update(float deltatime);
    bool hasHit() {return hashit;};
private:
    const float speed = 120;
    const float damage = 20;

    Enemy* target;
    ProjectileEffect effect;

    bool hashit = false;
};