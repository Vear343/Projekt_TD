#include "Entity.h"
#include <iostream>

Entity::Entity(float p_x, float p_y, float p_width, float p_height, SDL_Texture* p_texture)
    : x(p_x), y(p_y), width(p_width), height(p_height), texture(p_texture)
{
    // ถ้า texture มีขนาดไม่ตรงกับ width/height ที่กำหนดไว้ ให้ปรับ currentFrame ให้ตรงกับขนาดของ texture
    int texW, texH;
    if (texture) {
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    } else {
        texW = static_cast<int>(p_width);
        texH = static_cast<int>(p_height);
    }
    // ตั้งค่า Source Rect (สมมติว่าใช้ทั้งรูปภาพเป็น 1 Frame)
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = static_cast<int>(p_width);
    currentFrame.h = static_cast<int>(p_height);

    // ตั้งค่า Collider ให้ตรงกับขนาดของ Entity
    collider.w = (int)p_width/2;
    collider.h = (int)p_height/2;
}

SDL_Rect Entity::getCollider()
{
    SDL_Rect r;
    r.w = collider.w;
    r.h = collider.h;
    r.x = (int)(x - r.w / 2);
    r.y = (int)(y - r.h / 2);
    return r;
}