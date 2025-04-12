#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "model_editor.h"
#include "scene.h"
#include <raylib.h>
#include <stdbool.h>

// Game object (derived from editor objects but with physics properties)
typedef struct {
    Model model;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Color color;
    int id;

    // Physics properties (for future implementation)
    bool hasPhysics;
    float mass;
    Vector3 velocity;
    Vector3 acceleration;
    bool isStatic;
} GameObject;

// Game world state
typedef struct {
    GameObject* objects;
    int objectCount;
    int selectedObjectIndex;

    // Environment settings
    bool showGrid;
    Vector3 gravity;
    bool physicsEnabled;

    // Player controls
    Camera3D playerCamera;
    bool firstPersonMode;
    Vector3 playerPosition;
} GameWorld;

// Initialize the game world
void InitGameWorld(GameWorld* world);

// Import objects from model editor to game world
void ImportFromModelEditor(GameWorld* world, ModelEditor* editor);

// Update the game world state
void UpdateGameWorld(GameWorld* world, SceneContext* context);

// Draw the game world
void DrawGameWorld(GameWorld* world, SceneContext* context);

// Unload resources used by the game world
void UnloadGameWorld(GameWorld* world);

// Game world functions
void TogglePhysics(GameWorld* world, bool enabled);
void ResetGameWorld(GameWorld* world);
void ToggleFirstPersonMode(GameWorld* world);

#endif // GAME_WORLD_H
