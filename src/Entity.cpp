#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <iostream>

#include "Entity.h"
#include "RenderWindow.h"

#pragma region Entity
Entity::Entity(float p_x, float p_y, SDL_Texture* p_texture)
    :x(p_x),
     y(p_y), 
     texture(p_texture)
{
    currentFrame.w = 32;
    currentFrame.h = 32;
    currentFrame.x = 0;
    currentFrame.y = 0;
}

float Entity::getX()
{
    return x;
}

float Entity::getY()
{
    return y;
}

SDL_Texture* Entity::getTexture()
{
    return texture;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}