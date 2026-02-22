#include<iostream>
#include<SDL2/SDL.h>

#include"LEVEL.h"
#include"RenderWindow.h"
#include"Entity.h"
#include"Enemy.h"

const int WIDTH = 1280, HEIGHT = 720;

int main(int argc, char* argv[])
{  
    Uint64 framstart = SDL_GetPerformanceCounter();
    Uint64 framend;
    float deltaTime = 0;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Create a window
    RenderWindow window("SDL Window", WIDTH, HEIGHT);
    
    // load texture
    SDL_Texture* test_tex = window.loadTexture("assets/test_texture.png");
    SDL_Texture* enemy_tex = window.loadTexture("assets/test_enemy.png");
    
    
    Level mylevel;
    SDL_Event event;
    

    // สร้างเส้นทางสำหรับ Enemy โดยใช้ฟังก์ชัน gridToWorld เพื่อแปลง Grid Index เป็นพิกัดโลก (World Position)
    std::vector<Vector2D> path = {
        mylevel.gridToWorld(0, 0),
        mylevel.gridToWorld(10, 0),
        mylevel.gridToWorld(10, 10),
        mylevel.gridToWorld(20, 10),
        mylevel.gridToWorld(20, 20),
        mylevel.gridToWorld(55, 20),
        mylevel.gridToWorld(0, 20),
        mylevel.gridToWorld(0,0),
    };

    // // สรร้าง Entity >> Entity entity_name(pos_x, pos_y, width, height, texture);
    // Entity test_tile(0, 0, 16, 16, test_tex);
    // สรร้าง Enemy >> Enemy enemy_name(pos_x, pos_y, width, height, speed, texture, path);
    Enemy enemy(200, 200, 32, 32, 300.0f, enemy_tex, path);
    
    // Main loop
    while(true){

        framend = framstart;
        framstart = SDL_GetPerformanceCounter();
        // Create a deltaTime เพื่อใช้ในการอัพเดตตำแหน่งของ Enemy ให้เคลื่อนที่อย่างราบรื่นตามความเร็วที่กำหนด
        // deltaTime คือเวลาที่ผ่านไประหว่างเฟรมปัจจุบันกับเฟรมก่อนหน้า
        deltaTime = (float)(framstart - framend) / (float)SDL_GetPerformanceFrequency();

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) return false;
            else if (event.type == SDL_KEYDOWN) { 
                if (event.key.keysym.sym == SDLK_ESCAPE) return false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                
                int gx, gy;
                if (mylevel.worldToGrid(Vector2D((float)mouseX, (float)mouseY), gx, gy)) {
                    std::cout << "Clicked on grid: (" << gx << ", " << gy << ")\n";
                } else {
                    std::cout << "Clicked outside of grid\n";
                }
            }
        }

        enemy.follow_path(deltaTime); // Update enemy position based on its path and speed

        window.clear();
        mylevel.render_level(window.getRenderer(), test_tex);
        window.render(enemy);
        window.display();
    }
    // Clean up and quit SDL
    window.cleanUp();
    SDL_Quit();

    return 0;
}