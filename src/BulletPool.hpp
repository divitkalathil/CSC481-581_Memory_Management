#ifndef BULLETPOOL_HPP
#define BULLETPOOL_HPP

#include "Bullet.hpp"
#include "CustomAllocator.hpp"

/**
 * BulletPool: Manages a pool of bullets using a custom allocator
 */
class BulletPool {
public:
    /**
     * Constructor: Initialize the bullet pool
     */
    BulletPool(int slotSize, int numSlots);
    
    /**
     * Destructor: Cleans up pool resources
     */
    ~BulletPool();

    /**
     * Spawn a new bullet
     * @return Pointer to new bullet, or nullptr if pool is full
     */
    Bullet* spawn(float x, float y, float vx, float vy);

    /**
     * Update all active bullets
     */
    void update(float dt);

    /**
     * Render all active bullets
     */
    void render(SDL_Renderer* renderer);

    /**
     * Get pool statistics
     */
    int getActiveCount() const { return activeCount; }
    int getCapacity() const { return capacity; }
    float getUsagePercent() const { return alloc.getUsagePercent(); }

private:
    CustomAllocator alloc;
    int* activeIDs;       // Dynamically allocated array of active IDs
    int activeCount;      // Number of active bullets
    int capacity;         // Total pool capacity
};

#endif // BULLETPOOL_HPP