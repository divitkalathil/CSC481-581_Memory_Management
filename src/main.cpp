#include <SDL3/SDL.h>
#include "BulletPool.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Function to render the HUD
void renderHUD(SDL_Renderer* renderer, BulletPool& bulletPool) {
    // Background bar
    SDL_FRect hudBackground = {10, 10, 200, 30};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
    SDL_RenderFillRect(renderer, &hudBackground);

    // Usage bar
    float usagePercent = bulletPool.getUsagePercent();
    SDL_FRect hudBar = {15, 15, (usagePercent / 100.0f) * 190.0f, 20};
    
    // Color coding for usage
    if (usagePercent < 50.0f) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green
    } else if (usagePercent < 80.0f) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // Red
    }
    SDL_RenderFillRect(renderer, &hudBar);

    // Border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &hudBackground);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Custom Memory Allocator Demo", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create the bullet pool
    BulletPool bulletPool(sizeof(Bullet), 200); // Increased capacity
    SDL_Log("BulletPool initialized:");
    SDL_Log("  - Slot size: %zu bytes", sizeof(Bullet));
    SDL_Log("  - Capacity: %d bullets", bulletPool.getCapacity());
    SDL_Log("  - Total memory: %zu bytes", sizeof(Bullet) * bulletPool.getCapacity());

    // Game loop
    bool running = true;
    Uint64 lastFrameTime = SDL_GetPerformanceCounter();

    while (running) {
        // Delta time calculation
        Uint64 currentFrameTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentFrameTime - lastFrameTime) / (float)SDL_GetPerformanceFrequency();
        lastFrameTime = currentFrameTime;

        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running = false;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                float centerX = SCREEN_WIDTH / 2.0f;
                float centerY = SCREEN_HEIGHT / 2.0f;

                float dx = mouseX - centerX;
                float dy = mouseY - centerY;
                float length = SDL_sqrtf(dx * dx + dy * dy);
                float speed = 300.0f;
                
                if (length > 0) {
                    dx = (dx / length) * speed;
                    dy = (dy / length) * speed;
                }

                if (!bulletPool.spawn(centerX, centerY, dx, dy)) {
                    SDL_Log("Failed to spawn bullet - pool is FULL! Active: %d/%d", 
                           bulletPool.getActiveCount(), 
                           bulletPool.getCapacity());
                }
            }
        }

        // Update
        bulletPool.update(deltaTime);

        // Render
        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderClear(renderer);

        // Draw crosshair
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderLine(renderer, SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2, SCREEN_WIDTH/2 + 10, SCREEN_HEIGHT/2);
        SDL_RenderLine(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10);

        bulletPool.render(renderer);
        renderHUD(renderer, bulletPool);

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    SDL_Log("Game ended. Final stats:");
    SDL_Log("  - Active bullets at exit: %d", bulletPool.getActiveCount());
    SDL_Log("  - Pool usage at exit: %.1f%%", bulletPool.getUsagePercent());

    return 0;
}
