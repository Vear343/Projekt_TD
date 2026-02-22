#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <iostream>

#include "Entity.h"
#include "RenderWindow.h"

#pragma region Entity
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
}

float Entity::getX()
{
    return x;
}

float Entity::getY()
{
    return y;
}

float Entity::getWidth()
{
    return width;
}

float Entity::getHeight()
{
    return height;
}

SDL_Texture* Entity::getTexture()
{
    return texture;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}