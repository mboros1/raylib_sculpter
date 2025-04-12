#include "raylib_shim.h"

#include <raylib.h>

// Conversion helpers
Vector3 ToRaylibVector3(WorldVector3 v) { return (Vector3){v.x, v.y, v.z}; }

Color ToRaylibColor(WorldColor c) { return (Color){c.r, c.g, c.b, c.a}; }

void RenderWorld(const GameWorld *world) {
  // Setup camera
  Camera3D camera = {.position = ToRaylibVector3(world->cameraPosition),
                     .target = ToRaylibVector3(world->cameraTarget),
                     .up = {0.0f, 1.0f, 0.0f},
                     .fovy = world->cameraFOV,
                     .projection = CAMERA_PERSPECTIVE};

  BeginMode3D(camera);
  {
    // Draw all objects
    for (int i = 0; i < world->objectCount; i++) {
      const GameObject *obj = &world->objects[i];
      DrawCubeV(ToRaylibVector3(obj->position), ToRaylibVector3(obj->scale),
                ToRaylibColor(obj->color));
    }
  }
  EndMode3D();
}