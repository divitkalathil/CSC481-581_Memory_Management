#include "Bullet.hpp"

Bullet::Bullet(float startX, float startY, float velX, float velY)
    : x(startX), y(startY), vx(velX), vy(velY), active(true), size(6) {
    // Called via placement new
}

Bullet::~Bullet() {
    // Nothing to clean up in this simple case,
    // but demonstrates proper lifetime management.
}

void Bullet::update(float dt) {
    // Move bullet
    x += vx * dt;
    y += vy * dt;

    // Deactivate if off-screen
    if (x < -10 || y < -10 || x > 810 || y > 610) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    // Draw as a yellow square
    SDL_FRect rect = {x - size / 2.0f, y - size / 2.0f, (float)size, (float)size};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}