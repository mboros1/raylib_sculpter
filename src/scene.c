#include "scene.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

// Initialize scene context
void InitSceneContext(SceneContext* context)
{
    context->currentScene = SCENE_MAIN_MENU;
    context->prevScene    = SCENE_MAIN_MENU;
    context->sceneChanged = true;

    // Setup default camera
    context->camera.position   = (Vector3){10.0f, 10.0f, 10.0f};
    context->camera.target     = (Vector3){0.0f, 0.0f, 0.0f};
    context->camera.up         = (Vector3){0.0f, 1.0f, 0.0f};
    context->camera.fovy       = 45.0f;
    context->camera.projection = CAMERA_PERSPECTIVE;
}

// Switch to a different scene
void SwitchToScene(SceneContext* context, SceneType scene)
{
    context->prevScene    = context->currentScene;
    context->currentScene = scene;
    context->sceneChanged = true;

    printf("Switching from scene %d to scene %d\n", context->prevScene, context->currentScene);
}

// Update the current scene
void UpdateCurrentScene(SceneContext* context)
{
    // Handle scene-specific updates
    switch (context->currentScene) {
        case SCENE_MAIN_MENU:
            // Main menu updates (animations, etc.)
            break;

        case SCENE_MODEL_EDITOR:
        case SCENE_GAME_WORLD:
            // Camera updates for 3D scenes
            if (IsKeyDown(KEY_W))
                context->camera.position.z -= 0.2f;
            if (IsKeyDown(KEY_S))
                context->camera.position.z += 0.2f;
            if (IsKeyDown(KEY_A))
                context->camera.position.x -= 0.2f;
            if (IsKeyDown(KEY_D))
                context->camera.position.x += 0.2f;

            // Camera zoom with mouse wheel
            float wheelMove = GetMouseWheelMove();
            if (wheelMove != 0) {
                Vector3 direction = Vector3Normalize(
                    Vector3Subtract(context->camera.target, context->camera.position));
                context->camera.position =
                    Vector3Add(context->camera.position, Vector3Scale(direction, wheelMove * 0.5f));
            }

            UpdateCamera(&context->camera, CAMERA_FREE);
            break;

        case SCENE_EXPORT:
            // Export scene updates
            break;

        default:
            break;
    }

    // Reset scene changed flag after handling it
    if (context->sceneChanged) {
        context->sceneChanged = false;
    }
}

// Draw the current scene
void DrawCurrentScene(SceneContext* context)
{
    // Common scene elements or scene-specific drawing can go here
    switch (context->currentScene) {
        case SCENE_MODEL_EDITOR:
        case SCENE_GAME_WORLD:
            // 3D scenes setup
            BeginMode3D(context->camera);

            // Draw grid (common for 3D scenes)
            DrawGrid(10, 1.0f);

            EndMode3D();
            break;

        default:
            break;
    }
}

// Unload the current scene resources
void UnloadCurrentScene(SceneContext* context)
{
    // Unload resources for the current scene
    switch (context->currentScene) {
        case SCENE_MAIN_MENU:
            // Unload main menu resources
            break;

        case SCENE_MODEL_EDITOR:
            // Model editor scene cleanup happens in its own module
            break;

        case SCENE_GAME_WORLD:
            // Game world scene cleanup happens in its own module
            break;

        case SCENE_EXPORT:
            // Export scene cleanup
            break;

        default:
            break;
    }
}