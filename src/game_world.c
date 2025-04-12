#include "game_world.h"

#include <stdio.h>
#include <stdlib.h>

// Initialize the game world
void InitGameWorld(GameWorld *world) {
  world->objects = NULL;
  world->objectCount = 0;
  world->selectedObjectIndex = -1;

  // Environment settings
  world->showGrid = true;
  world->physicsEnabled = false;

  // Player controls
  world->firstPersonMode = false;

  // Initialize camera defaults
  world->cameraPosition = (WorldVector3){0.0f, 2.0f, 4.0f};
  world->cameraTarget = (WorldVector3){0.0f, 0.0f, 0.0f};
  world->cameraFOV = 60.0f;

  printf("Game world initialized\n");
}

// Simple vector math operations
static WorldVector3 Vector3Add(WorldVector3 a, WorldVector3 b) {
  return (WorldVector3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static WorldVector3 Vector3Scale(WorldVector3 v, float scalar) {
  return (WorldVector3){v.x * scalar, v.y * scalar, v.z * scalar};
}

// Simplified physics update
void UpdateWorldPhysics(GameWorld *world, float deltaTime) {
  if (!world->physicsEnabled) return;

  const WorldVector3 gravity = {0.0f, -9.81f, 0.0f};

  for (int i = 0; i < world->objectCount; i++) {
    GameObject *obj = &world->objects[i];
    if (!obj->hasPhysics || obj->isStatic) continue;

    // Apply gravity
    obj->velocity = Vector3Add(obj->velocity, Vector3Scale(gravity, deltaTime));
    obj->position =
        Vector3Add(obj->position, Vector3Scale(obj->velocity, deltaTime));
  }
}

// Game world functions

// Toggle physics simulation
void TogglePhysics(GameWorld *world, bool enabled) {
  world->physicsEnabled = enabled;
  printf("Physics %s\n", enabled ? "enabled" : "disabled");
}

// Reset the game world
void ResetGameWorld(GameWorld *world) {
  // Reset all object positions and physics
  for (int i = 0; i < world->objectCount; i++) {
    GameObject *obj = &world->objects[i];

    // Reset physics state
    obj->velocity = (WorldVector3){0.0f, 0.0f, 0.0f};
    obj->acceleration = (WorldVector3){0.0f, 0.0f, 0.0f};
  }

  // Reset player position
  world->cameraPosition = (WorldVector3){0.0f, 1.0f, 0.0f};
  world->cameraPosition = world->cameraPosition;
  world->cameraTarget =
      Vector3Add(world->cameraPosition, (WorldVector3){1.0f, 0.0f, 0.0f});

  printf("Game world reset\n");
}

// Toggle between first-person and orbit camera modes
void ToggleFirstPersonMode(GameWorld *world) {
  world->firstPersonMode = !world->firstPersonMode;

  if (world->firstPersonMode) {
    // Switch to first-person camera
    world->cameraPosition = world->cameraPosition;
    world->cameraTarget =
        Vector3Add(world->cameraPosition, (WorldVector3){1.0f, 0.0f, 0.0f});
  }

  printf("Camera mode: %s\n",
         world->firstPersonMode ? "First Person" : "Orbit");
}
