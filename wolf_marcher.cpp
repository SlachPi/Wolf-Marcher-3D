#include <SDL2/SDL.h>

#include <iostream>
#include "wolf_marcher.h"

#define WIN_WIDTH 420
#define WIN_HEIGHT 480

#define HALF_FOV 30
#define NUM_RAYS 210

void display_walls(RayCaster* rc, SDL_Renderer* ren);
void display_ground(SDL_Renderer* ren);

int main()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    return EXIT_FAILURE;

  SDL_Window* win = SDL_CreateWindow("Wolf-Marcher 3D", 100, 100, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  RayCaster rc("map.txt");

  bool stop = false;

  while(!stop) 
  {
    SDL_Event event;
    
    while(SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_QUIT) 
        stop = true;

      if (event.type == SDL_KEYDOWN) {
        int key = event.key.keysym.sym;
        switch(key) {
          case SDLK_UP :
          rc.move_player(30);
          break;

          case SDLK_DOWN :
          rc.move_player(-30);
          break;

          case SDLK_LEFT :
          rc.rotate_player(-10);
          break;

          case SDLK_RIGHT :
          rc.rotate_player(10);
          break;
        } 
      }
    }

    SDL_SetRenderDrawColor(ren,200,200,200,255);
    SDL_RenderClear(ren);
    
    display_ground(ren);
    display_walls(&rc, ren);

    SDL_RenderPresent(ren);
}
    
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return EXIT_SUCCESS;
}

void display_ground(SDL_Renderer* ren) {

    SDL_Rect bottom_half;
    bottom_half.x = 0;
    bottom_half.y = WIN_HEIGHT / 2;
    bottom_half.w = WIN_WIDTH;
    bottom_half.h = WIN_HEIGHT / 2;

    SDL_SetRenderDrawColor(ren, 0, 100, 100, 250);

    SDL_RenderFillRect(ren, &bottom_half);
}

void display_walls(RayCaster* rc, SDL_Renderer* ren) {
    
    int unit_width = WIN_WIDTH / NUM_RAYS;
    int unit_height = 2 * WIN_HEIGHT;
    float unit_angle = float(HALF_FOV) / NUM_RAYS;

    for (int i = 0; i < NUM_RAYS; ++i) {

      float theta = - HALF_FOV + 2.0 * i * unit_angle;
    
      float distance;

      rc->ray_cast(theta, distance);

      SDL_Rect rect;
      rect.w = unit_width;
      rect.h = int(unit_height / distance);
      rect.x = i * unit_width;
      rect.y = (WIN_HEIGHT - rect.h) / 2;

      SDL_SetRenderDrawColor(ren, 0, 0, 100 + int(155/distance), 100 + int(155/distance));

      SDL_RenderFillRect(ren, &rect);
    }

}