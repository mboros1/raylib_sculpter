#include "game_world.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

// Initialize the game world
void InitGameWorld(GameWorld* world)
{
    world->objects             = NULL;
    world->objectCount         = 0;
    world->selectedObjectIndex = -1;

    // Environment settings
    world->showGrid       = true;
    world->gravity        = (Vector3){0.0f, -9.8f, 0.0f};
    world->physicsEnabled = false;

    // Player controls
    world->firstPersonMode = false;
    world->playerPosition  = (Vector3){0.0f, 1.0f, 0.0f};

    // Setup player camera (first person view)
    world->playerCamera.position   = world->playerPosition;
    world->playerCamera.target     = Vector3Add(world->playerPosition, (Vector3){1.0f, 0.0f, 0.0f});
    world->playerCamera.up         = (Vector3){0.0f, 1.0f, 0.0f};
    world->playerCamera.fovy       = 60.0f;
    world->playerCamera.projection = CAMERA_PERSPECTIVE;

    printf("Game world initialized\n");
}

// Import objects from model editor to game world
void ImportFromModelEditor(GameWorld* world, ModelEditor* editor)
{
    // Clean existing world objects
    for (int i = 0; i < world->objectCount; i++) {
        UnloadModel(world->objects[i].model);
    }

    if (world->objects != NULL) {
        free(world->objects);
    }

    // Create new objects array
    world->objectCount = editor->objectCount;

    if (world->objectCount == 0) {
        world->objects = NULL;
        return;
    }

    world->objects = (GameObject*)malloc(world->objectCount * sizeof(GameObject));

    if (world->objects == NULL) {
        printf("Failed to allocate memory for game objects\n");
        world->objectCount = 0;
        return;
    }

    // Copy objects from editor
    for (int i = 0; i < world->objectCount; i++) {
        EditorObject* editorObj = &editor->objects[i];
        GameObject* gameObj     = &world->objects[i];

        // Copy basic properties
        gameObj->model    = editorObj->model; // Use the same model pointer (be careful with this!)
        gameObj->position = editorObj->position;
        gameObj->rotation = editorObj->rotation;
        gameObj->scale    = editorObj->scale;
        gameObj->color    = editorObj->color;
        gameObj->id       = editorObj->id;

        // Initialize physics properties
        gameObj->hasPhysics   = true;
        gameObj->mass         = 1.0f; // Default mass
        gameObj->velocity     = (Vector3){0.0f, 0.0f, 0.0f};
        gameObj->acceleration = (Vector3){0.0f, 0.0f, 0.0f};
        gameObj->isStatic     = false;

        // Special cases for physics properties
        if (editorObj->type == SHAPE_PLANE) {
            gameObj->isStatic = true; // Planes are static by default
        }
    }

    printf("Imported %d objects to game world\n", world->objectCount);
}

// Simple physics update
void UpdatePhysics(GameWorld* world, float deltaTime)
{
    if (!world->physicsEnabled)
        return;

    // Apply physics to each object
    for (int i = 0; i < world->objectCount; i++) {
        GameObject* obj = &world->objects[i];

        // Skip static objects
        if (obj->isStatic)
            continue;

        // Apply gravity
        if (obj->hasPhysics) {
            // F = ma, so a = F/m
            obj->acceleration = Vector3Scale(world->gravity, 1.0f / obj->mass);

            // Update velocity
            obj->velocity = Vector3Add(obj->velocity, Vector3Scale(obj->acceleration, deltaTime));

            // Update position
            obj->position = Vector3Add(obj->position, Vector3Scale(obj->velocity, deltaTime));

            // Very simple ground collision
            if (obj->position.y < 0.0f) {
                obj->position.y = 0.0f;
                obj->velocity.y = -obj->velocity.y * 0.5f; // Bounce with damping

                // Apply friction to horizontal movement
                obj->velocity.x *= 0.9f;
                obj->velocity.z *= 0.9f;
            }
        }
    }
}

// Update player first-person camera
void UpdateFPSCamera(GameWorld* world)
{
    // Only update if in first-person mode
    if (!world->firstPersonMode)
        return;

    // Handle camera movement
    if (IsKeyDown(KEY_W)) {
        // Move forward
        Vector3 forward = Vector3Normalize(
            Vector3Subtract(world->playerCamera.target, world->playerCamera.position));
        forward.y                    = 0; // Keep movement horizontal
        forward                      = Vector3Scale(forward, 0.1f);
        world->playerCamera.position = Vector3Add(world->playerCamera.position, forward);
        world->playerCamera.target   = Vector3Add(world->playerCamera.target, forward);
    }

    if (IsKeyDown(KEY_S)) {
        // Move backward
        Vector3 backward = Vector3Normalize(
            Vector3Subtract(world->playerCamera.position, world->playerCamera.target));
        backward.y                   = 0; // Keep movement horizontal
        backward                     = Vector3Scale(backward, 0.1f);
        world->playerCamera.position = Vector3Add(world->playerCamera.position, backward);
        world->playerCamera.target   = Vector3Add(world->playerCamera.target, backward);
    }

    if (IsKeyDown(KEY_A)) {
        // Strafe left
        Vector3 forward = Vector3Normalize(
            Vector3Subtract(world->playerCamera.target, world->playerCamera.position));
        Vector3 right                = Vector3CrossProduct(forward, world->playerCamera.up);
        Vector3 left                 = Vector3Scale(right, -0.1f);
        world->playerCamera.position = Vector3Add(world->playerCamera.position, left);
        world->playerCamera.target   = Vector3Add(world->playerCamera.target, left);
    }

    if (IsKeyDown(KEY_D)) {
        // Strafe right
        Vector3 forward = Vector3Normalize(
            Vector3Subtract(world->playerCamera.target, world->playerCamera.position));
        Vector3 right                = Vector3CrossProduct(forward, world->playerCamera.up);
        right                        = Vector3Scale(right, 0.1f);
        world->playerCamera.position = Vector3Add(world->playerCamera.position, right);
        world->playerCamera.target   = Vector3Add(world->playerCamera.target, right);
    }

    // Capture mouse for look
    if (IsKeyDown(KEY_LEFT_ALT)) {
        // Look around with mouse
        Vector2 mouseDelta = GetMouseDelta();

        // Apply yaw rotation
        if (mouseDelta.x != 0) {
            Vector3 forward =
                Vector3Subtract(world->playerCamera.target, world->playerCamera.position);
            float rotation = -mouseDelta.x * 0.003f;

            // Rotate around Y axis
            forward = Vector3RotateByAxisAngle(forward, world->playerCamera.up, rotation);
            world->playerCamera.target = Vector3Add(world->playerCamera.position, forward);
        }

        // Apply pitch rotation (with limits)
        if (mouseDelta.y != 0) {
            Vector3 forward = Vector3Normalize(
                Vector3Subtract(world->playerCamera.target, world->playerCamera.position));
            Vector3 right  = Vector3CrossProduct(forward, world->playerCamera.up);
            float rotation = -mouseDelta.y * 0.003f;

            // Rotate around X axis (right vector)
            forward                    = Vector3RotateByAxisAngle(forward, right, rotation);
            world->playerCamera.target = Vector3Add(world->playerCamera.position, forward);
        }
    }

    // Update player position from camera
    world->playerPosition = world->playerCamera.position;
}

// Update the game world state
void UpdateGameWorld(GameWorld* world, SceneContext* context)
{
    // Unused context parameter intentionally not referenced
    (void)context;

    // Update physics
    UpdatePhysics(world, GetFrameTime());

    // Update player camera if in first-person mode
    if (world->firstPersonMode) {
        UpdateFPSCamera(world);
    } else {
        // In orbit mode, use the scene context camera
        // We could add specific game world camera controls here
    }
}

// Draw the game world
void DrawGameWorld(GameWorld* world, SceneContext* context)
{
    // Begin 3D mode with appropriate camera
    if (world->firstPersonMode) {
        BeginMode3D(world->playerCamera);
    } else {
        BeginMode3D(context->camera);
    }

    // Draw the ground
    DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){20.0f, 20.0f}, LIGHTGRAY);

    // Draw grid
    if (world->showGrid) {
        DrawGrid(20, 1.0f);
    }

    // Draw all objects
    for (int i = 0; i < world->objectCount; i++) {
        GameObject* obj = &world->objects[i];

        // Draw model with transforms
        DrawModelEx(obj->model, obj->position, (Vector3){0.0f, 1.0f, 0.0f}, obj->rotation.y,
                    obj->scale, obj->color);
    }

    // Draw player marker if in third-person mode
    if (!world->firstPersonMode) {
        DrawSphere(world->playerPosition, 0.2f, RED);
    }

    EndMode3D();

    // Draw UI elements
    DrawRectangle(10, 10, 320, 113, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 10, 320, 113, BLUE);

    // Game info
    DrawText("Game World", 20, 20, 20, BLACK);

    // Physics status
    DrawText(TextFormat("Physics: %s (P to toggle)", world->physicsEnabled ? "ON" : "OFF"), 20, 50,
             10, BLACK);

    // Camera mode
    DrawText(
        TextFormat("Camera: %s (F to toggle)", world->firstPersonMode ? "First Person" : "Orbit"),
        20, 70, 10, BLACK);

    // Controls
    DrawText("R: Reset | ALT+Mouse: Look | ESC: Exit", 20, 90, 10, BLACK);
}

// Unload resources used by the game world
void UnloadGameWorld(GameWorld* world)
{
    // In a production app, we'd need to decide whether to unload models here
    // or leave that to the model editor, depending on how ownership is managed

    // Free memory
    if (world->objects != NULL) {
        free(world->objects);
        world->objects = NULL;
    }

    world->objectCount = 0;
}

// Game world functions

// Toggle physics simulation
void TogglePhysics(GameWorld* world, bool enabled)
{
    world->physicsEnabled = enabled;
    printf("Physics %s\n", enabled ? "enabled" : "disabled");
}

// Reset the game world
void ResetGameWorld(GameWorld* world)
{
    // Reset all object positions and physics
    for (int i = 0; i < world->objectCount; i++) {
        GameObject* obj = &world->objects[i];

        // Reset physics state
        obj->velocity     = (Vector3){0.0f, 0.0f, 0.0f};
        obj->acceleration = (Vector3){0.0f, 0.0f, 0.0f};
    }

    // Reset player position
    world->playerPosition        = (Vector3){0.0f, 1.0f, 0.0f};
    world->playerCamera.position = world->playerPosition;
    world->playerCamera.target   = Vector3Add(world->playerPosition, (Vector3){1.0f, 0.0f, 0.0f});

    printf("Game world reset\n");
}

// Toggle between first-person and orbit camera modes
void ToggleFirstPersonMode(GameWorld* world)
{
    world->firstPersonMode = !world->firstPersonMode;

    if (world->firstPersonMode) {
        // Switch to first-person camera
        world->playerCamera.position = world->playerPosition;
        world->playerCamera.target = Vector3Add(world->playerPosition, (Vector3){1.0f, 0.0f, 0.0f});
    }

    printf("Camera mode: %s\n", world->firstPersonMode ? "First Person" : "Orbit");
}
