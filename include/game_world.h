#pragma once

#include <stdbool.h>

// Simple math types that don't depend on raylib
typedef struct WorldVector3 {
  float x;
  float y;
  float z;
} WorldVector3;

typedef struct WorldColor {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} WorldColor;

// Game object (derived from editor objects but with physics properties)
typedef struct GameObject {
  WorldVector3 position;
  WorldVector3 scale;
  WorldVector3 rotation_axis;
  float rotation_angle;
  WorldColor color;
  int id;

  // Physics properties (for future implementation)
  bool hasPhysics;
  float mass;
  WorldVector3 velocity;
  WorldVector3 acceleration;
  bool isStatic;
} GameObject;

// Game world state
typedef struct GameWorld {
  GameObject *objects;
  int objectCount;
  int selectedObjectIndex;

  // Environment settings
  bool showGrid;
  WorldVector3 gravity;
  bool physicsEnabled;

  // Player controls
  WorldVector3 cameraPosition;
  WorldVector3 cameraTarget;
  float cameraFOV;
  bool firstPersonMode;
} GameWorld;

// Initialize the game world
void InitGameWorld(GameWorld *world);

// Unload resources used by the game world
void UnloadGameWorld(GameWorld *world);

// Game world functions
void TogglePhysics(GameWorld *world, bool enabled);
void ResetGameWorld(GameWorld *world);
void ToggleFirstPersonMode(GameWorld *world);

// Object manipulation
void AddObject(GameWorld *world, WorldVector3 position, WorldVector3 scale,
               WorldColor color);
void RemoveObject(GameWorld *world, int index);
void UpdateWorldPhysics(GameWorld *world, float deltaTime);
