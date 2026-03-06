#include <Projectile.h>

Projectile::Projectile(float p_x, float p_y, SDL_Texture* p_texture, Enemy* p_target, ProjectileEffect p_effect)
    : Entity(p_x, p_y, 16, 16, p_texture)
{
    target = p_target;
    effect = p_effect;
    collider.x = p_x;
    collider.y = p_y;
    collider.w = width;
    collider.h = height;
}

void Projectile::update(float deltatime){

    // if target is already dead -> delete projectile
    if (!target || !target->isAlive()) {
        hashit = true;
        return;
    }

    Vector2D currentPos(x, y);
    Vector2D targetPos(target->getX(), target->getY());
    Vector2D direction = targetPos - currentPos;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Update collider position to match projectile's current location
    collider.x = static_cast<int>(x);
    collider.y = static_cast<int>(y);
    collider.w = 16;
    collider.h = 16;

    // Check collision - if projectile hitbox intersects with enemy hitbox
    SDL_Rect enemyCollider = target->getCollider();
    if (SDL_HasIntersection(&collider, &enemyCollider)) {
        if (!hashit) {
            target->takeDamage(damage);

            switch (effect){
                case ProjectileEffect::BURN:
                    target->applyBurn(5.0f, 3.0f); // ติดไฟ 5 damage ต่อวินาที เป็นเวลา 3 วินาที
                    break;
                case ProjectileEffect::SLOW:
                    target->applySlow(0.5f, 2.0f); // สโลว์ศัตรูเป็นเวลา 2 วินาที
                    break;
                case ProjectileEffect::STUNT:
                    target->applyStunt(2.0f); // สตั๊นศัตรูเป็นเวลา 2 วินาที
                    break;
                default:
                    break;
            }

            hashit = true;
        }
        return;
    }

    // Normalize direction and move towards target
    if (distance > 0.0f) {
        direction.x /= distance;
        direction.y /= distance;

        x += direction.x * speed * deltatime;
        y += direction.y * speed * deltatime;
    }
}