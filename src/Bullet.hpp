#ifndef BULLET_HPP
#define BULLET_HPP

#include <SDL3/SDL.h>

/**
 * Bullet: A simple projectile object
 */
class Bullet {
public:
    // State
    float x, y;
    float vx, vy;
    bool active;
    int size;

    /**
     * Constructor: Initialize a bullet
     */
    Bullet(float startX, float startY, float velX, float velY);

    /**
     * Destructor: Clean up resources
     */
    ~Bullet();

    /**
     * Update the bullet's position and state
     */
    void update(float dt);

    /**
     * Render the bullet to the screen
     */
    void render(SDL_Renderer* renderer);
};

#endif // BULLET_HPP