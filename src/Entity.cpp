#include "Entity.h"

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

    // ตั้งค่า Collider ให้ตรงกับตำแหน่งวัตถุ
    collider.x = static_cast<int>(p_x);
    collider.y = static_cast<int>(p_y);
    collider.w = texW;
    collider.h = texH;
}

// void Entity::render(SDL_Renderer* renderer) {
//     if (texture) {
//         // ตำแหน่งที่จะวาดบนหน้าจอ
//         // เนื่องจาก x, y คือจุดกึ่งกลางของ Entity เราต้องปรับให้ dst.x, dst.y เป็นมุมบนซ้าย
//         SDL_Rect dst;
//         dst.x = static_cast<int>(x - (width / 2));
//         dst.y = static_cast<int>(y - (height / 2));
//         dst.w = static_cast<int>(width);
//         dst.h = static_cast<int>(height);

//         // วาด Texture ลงบน Renderer
//         // currentFrame คือส่วนของรูปภาพที่จะตัดมา (Source)
//         // dst คือตำแหน่งและขนาดบนหน้าจอ (Destination)
//         SDL_RenderCopy(renderer, texture, &currentFrame, &dst);
//     }
// }