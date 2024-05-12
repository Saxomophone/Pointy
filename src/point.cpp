#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>

#define GRAVITY 9.81
#define AIR_DENSITY 1.225


class Point {
  public:
    Point(double newX, double newY) : newX(newX), newY(newY) {}
    double newX;
    double newY;
    double velocity = 0;    //m/s
    double mass = 100;        //g
    double dt = 0.1;        //s
    double dragCoefficient = 1.05;
  
    void draw(SDL_Renderer* renderer) {
      SDL_Rect rect;
      rect.x = newX;
      rect.y = newY;
      SDL_RenderDrawPoint(renderer, newX, newY);
    }

    
    double accelerationY() {
      double drag = 0.5 * AIR_DENSITY * dragCoefficient* pow(velocity, 2);
      double forceY = (mass * GRAVITY) - drag;    //F = ma to get gravitational force then subtract drag force
      double accelerationY = forceY / mass;

      accelerationY = accelerationY * dt;      

      return accelerationY;
    }

    void update(int floorY) {
      velocity += accelerationY() * dt;  
      newY += velocity;
    }
};

int initialiseSDL() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cout << "Failed to initialize the SDL2 library\n";
      return -1;
  }
  return 0;
}

SDL_Window* createWindow(const int width, const int height) {
  SDL_Window* window = SDL_CreateWindow("SDL2 Window",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        width,
                                        height,
                                        0);

  if(!window)
    {
        std::cout << "Failed to create window\n";
        exit(-1);
    }
  
  return window;
}

SDL_Surface* createSurface(SDL_Window* window) {
  SDL_Surface* window_surface = SDL_GetWindowSurface(window);

  if (!window_surface) {
    std::cout << "Failed to get the surface from the window\n";
    exit(-1);
  }

  return window_surface;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    std::cout << "Failed to create renderer\n";
    exit(-1);
  }

  return renderer;
}

int main() {
  initialiseSDL();
  SDL_Window* window = createWindow(800, 800);
  SDL_Surface* window_surface = createSurface(window);

  SDL_Renderer* renderer = createRenderer(window);

  int window_width, window_height;

  bool checkStep = true;

  std::vector<Point> points;
  std::cout << "created vector<Point>" << std::endl;
  for (int i = 0; i < 50; i+=1) {
    std::cout << "before push_back to vector<Point>, i:" << i << std::endl;
    points.push_back(Point{400+i, 0});
    std::cout << "pushed to vector<Point>" << std::endl;
  }

  // Event loop
  SDL_Event event;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }
        if (event.type == SDL_KEYDOWN) {
          if (event.key.keysym.sym == SDLK_SPACE) {
            checkStep = true;
          }
        }
    }

    if (checkStep) {
      // std::cout << "checkStep" << checkStep << std::endl;

      SDL_GetRendererOutputSize(renderer, &window_width, &window_height);
      //clear screen
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer); // Clear the renderer to a solid color

      //draw floor        
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      int floorY = window_height - 20;
      SDL_RenderFillRect(renderer, new SDL_Rect{0, floorY, window_width, 20}); 

      SDL_SetRenderDrawColor(renderer, 255, 20, 20, 255);

      std::cout << points.size() << std::endl;
      for (int i = 0; i < points.size(); i++) {
        points[i].draw(renderer);
        points[i].update(floorY);
        std::cout << points[i].newY << std::endl;
      }

      SDL_RenderPresent(renderer);

      checkStep = false;
    }
}
  
  

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}