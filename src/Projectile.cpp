#include <Projectile.h>

Projectile::Projectile(float p_x, float p_y, SDL_Texture* p_texture, Enemy* p_target)
    : Entity(p_x, p_y, 16, 16, p_texture)
{
    target = p_target;
    collider.x = p_x;
    collider.y = p_y;
    collider.w = width;
    collider.h = height;
}

// void Projectile::hasHit(){

// }

void Projectile::update(float deltatime){

    Vector2D currentPos(x, y);
    Vector2D targetPos(target->getX(), target->getY());
    Vector2D direction = targetPos - currentPos;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Update collider position
    collider.x = static_cast<int>(x);
    collider.y = static_cast<int>(y);
    collider.w = 16;
    collider.h = 16;

    // Check collision - if projectile hitbox intersects with enemy hitbox
    SDL_Rect enemyCollider = target->getCollider();
    if (SDL_HasIntersection(&collider, &enemyCollider)) {
        if (!hashit) {
            target->takeDamage(damage);
            hashit = true;
            // if (hasHit){

            // }
        }
        return;
    }

    // Normalize direction and move towards target
    if (distance > 0.0f) {
        direction.x /= distance;
        direction.y /= distance;

        x += direction.x * speed * deltatime;
    }
}