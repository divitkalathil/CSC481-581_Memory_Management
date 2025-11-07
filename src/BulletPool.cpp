#include "BulletPool.hpp"
#include <new>  // For placement new
#include <SDL3/SDL.h>

BulletPool::BulletPool(int slotSize, int numSlots)
    : alloc(slotSize, numSlots), activeCount(0), capacity(numSlots) {
    // Dynamically allocate the array for tracking active IDs
    activeIDs = new int[numSlots];
}

BulletPool::~BulletPool() {
    // Manually destruct any remaining active bullets
    for (int i = 0; i < activeCount; ++i) {
        int id = activeIDs[i];
        Bullet* bullet = reinterpret_cast<Bullet*>(alloc.getPtr(id));
        bullet->~Bullet();
    }
    // Free the tracking array
    delete[] activeIDs;
}

Bullet* BulletPool::spawn(float x, float y, float vx, float vy) {
    // Check if the pool is already at capacity
    if (activeCount >= capacity) {
        SDL_Log("BulletPool FULL! Cannot spawn more bullets.");
        return nullptr;
    }

    // Allocate a memory slot
    int id = alloc.alloc();
    if (id == -1) {
        // This can happen if alloc logic differs from activeCount, a safeguard.
        SDL_Log("Allocator returned -1, pool is full.");
        return nullptr;
    }

    // Get a pointer to the allocated memory
    void* mem = alloc.getPtr(id);
    
    // Use placement new to construct a Bullet object at the memory location
    Bullet* bullet = new (mem) Bullet(x, y, vx, vy);
    
    // Add the new bullet's ID to the active list
    activeIDs[activeCount++] = id;

    return bullet;
}

void BulletPool::update(float dt) {
    // Iterate through active bullets and update them
    for (int i = 0; i < activeCount; ) {
        int id = activeIDs[i];
        Bullet* bullet = reinterpret_cast<Bullet*>(alloc.getPtr(id));
        
        bullet->update(dt);

        // If bullet is no longer active, clean it up
        if (!bullet->active) {
            // 1. Explicitly call the destructor
            bullet->~Bullet();
            // 2. Free the memory slot
            alloc.freeSlot(id);
            // 3. Remove from active list using swap-and-pop
            activeIDs[i] = activeIDs[activeCount - 1];
            activeCount--;
            // Do not increment 'i' since we need to check the swapped element
        } else {
            i++; // Increment only if no removal occurred
        }
    }
}

void BulletPool::render(SDL_Renderer* renderer) {
    // Render all active bullets
    for (int i = 0; i < activeCount; i++) {
        int id = activeIDs[i];
        Bullet* bullet = reinterpret_cast<Bullet*>(alloc.getPtr(id));
        bullet->render(renderer);
    }
}