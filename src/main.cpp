#include<iostream>
#include<SDL2/SDL.h>
#include"LEVEL.h"
#include"RenderWindow.h"
#include"Entity.h"

const int WIDTH = 1280, HEIGHT = 720;

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Create a window
    RenderWindow window("SDL Window", WIDTH, HEIGHT);

    SDL_Texture* test_tex = window.loadTexture("assets/test_texture.png");
    SDL_Texture* grass_tex = window.loadTexture("assets/grassblock.png");

    // Create an entity with the loaded texture
    //               /pos_x/pos_y/ texture /
    Entity test_tile(0, 0, test_tex);
    Entity grass_tile(0, 0, grass_tex);

    Level mylevel;
    SDL_Event event;
    // Main loop
    while(true){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) return false;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) return false;
            }
            // Clear the window, render the texture, and display it
            window.clear();
            mylevel.render(window.getRenderer(), test_tile);
            window.render(grass_tile);
            window.display();
        }
    }
    // Clean up and quit SDL
    window.cleanUp();
    SDL_Quit();

    return 0;
}