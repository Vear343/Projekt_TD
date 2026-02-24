#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <iostream>

#include "Entity.h"

Entity::Entity(float p_x, float p_y, float p_width, float p_height, SDL_Texture* p_texture)
    :x(p_x),
     y(p_y), 
     width(p_width),
     height(p_height),
     texture(p_texture)
{
    currentFrame.w = width;
    currentFrame.h = height;
    currentFrame.x = p_x;
    currentFrame.y = p_y;

    collider.w = width;
    collider.h = height;
}

void Entity::update(float deltatime){
    collider.x = static_cast<int>(x);
    collider.y = static_cast<int>(y);
}