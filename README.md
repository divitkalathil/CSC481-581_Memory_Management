# CSC481/581 - Custom Memory Allocator for Game Systems

## Overview

This project demonstrates **custom memory allocation** for game objects using a fixed-size memory pool. Students will learn how modern game engines manage memory to achieve predictable, high-performance object creation and destruction.

### Learning Objectives
* **LO 1:** Understand contiguous memory allocation and cache-friendly data structures
* **LO 2:** Implement placement new and manual object lifetime management
* **LO 3:** Use index-based handles instead of raw pointers
* **LO 4:** Design memory pools for real-time game systems


## Core Concepts

### 1. Custom Allocators
Instead of using `new`/`delete` (which are slow and unpredictable), game engines allocate a large memory block upfront and manage it manually.

**Benefits:**
- **Fast**: No system calls during gameplay
- **Predictable**: No memory fragmentation
- **Cache-friendly**: All objects live close together
- **Debuggable**: Easy to track allocations

### 2. Memory Pool Pattern
```
┌────────┬────────┬────────┬────────┬────────┐
│ Slot 0 │ Slot 1 │ Slot 2 │ Slot 3 │ Slot 4 │
│  FREE  │  USED  │  USED  │  FREE  │  FREE  │
└────────┴────────┴────────┴────────┴────────┘
```

Each slot holds one object. When you need an object, grab a free slot. When done, mark it free again.

### 3. Placement New
Normal `new` does two things: allocate memory AND call constructor.
Placement new ONLY calls the constructor at a specific address:
```cpp
void* memory = allocator.getPtr(id);
Bullet* b = new (memory) Bullet(x, y, vx, vy);
```

### 4. Manual Lifetime Management
With placement new, you MUST manually call the destructor:
```cpp
b->~Bullet();              // Call destructor
allocator.freeSlot(id);    // Free the memory slot
```


## Your Task

### Part 1: Understanding the Code
1. Read through `CustomAllocator.hpp` and `.cpp` - understand how memory is allocated
2. Examine `BulletPool.hpp` and `.cpp` - see how bullets use the allocator
3. Study `main.cpp` - observe the spawn/update/render loop

### Part 2: Experiments
Run the program and try these experiments:

**Experiment 1: Capacity Testing**
- Change pool size in `main.cpp`: `BulletPool pool(sizeof(Bullet), 50);`
- Try 50, 100, 200 slots
- Click rapidly - when does it fail?

**Experiment 2: Slot Size Impact**
- Try: `BulletPool pool(64, 100);` (oversized slots)
- Calculate wasted memory: `(slotSize - sizeof(Bullet)) * slotCount`

**Experiment 3: Performance Observation**
- Watch the HUD bar (top-left) showing memory usage
- Green = healthy, Yellow = filling, Red = nearly full

### Part 3: Extensions (Optional)
1. **Add a bullet counter** showing active bullets on screen
2. **Implement bullet types** (FastBullet, SlowBullet) - how to handle different sizes?
3. **Add collision** between bullets and screen boundaries
4. **Profile performance** - compare spawning 1000 bullets vs traditional `new`

---

## Prerequisites

You must have **SDL3** installed:

* **macOS:** `brew install sdl3`
* **Linux (Debian/Ubuntu):** `sudo apt update && sudo apt install libsdl3-dev`
* **Windows (MSYS2):** `pacman -S mingw-w64-ucrt-x86_64-SDL3`


## How to Build and Run

### 1. Create Build Directory
```bash
mkdir build
cd build
```

### 2. Configure with CMake
```bash
cmake ..
```

### 3. Compile
```bash
make
```

### 4. Run
```bash
./CustomAllocatorDemo
```

---

## Controls

* **Left Mouse Click**: Spawn bullets toward cursor
* **ESC or Close Window**: Quit


## Understanding the Output

### HUD Display (Top-Left)
- **Green bar**: Memory pool < 50% full
- **Yellow bar**: Memory pool 50-80% full  
- **Red bar**: Memory pool > 80% full

### Console Messages
```
BulletPool created with capacity: 100 bullets
BulletPool FULL! Cannot spawn more bullets.
Failed to spawn bullet - pool is full!
```

## Performance Comparison

| Method | Cycles per Allocation | Cache Friendly | Fragmentation |
|--------|----------------------|----------------|---------------|
| `new`/`delete` | ~500-1000 | No | Yes |
| Memory Pool | ~50-100 | Yes | No |

## Troubleshooting

### Build Errors
```bash
# If SDL3 not found:
brew install sdl3  # macOS
sudo apt install libsdl3-dev  # Linux

# If CMake version too old:
brew upgrade cmake  # macOS
sudo apt install cmake  # Linux
```

### Runtime Issues
- **No window appears**: Check SDL initialization errors in console
- **Bullets not spawning**: Pool might be full - check console for "FULL" messages
- **Crash on spawn**: Verify `sizeof(Bullet)` matches slotSize in pool creation

