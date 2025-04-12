#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_world.h"
#include "model_editor.h"
#include "raylib_shim.h"
#include "scene.h"
#include "stl_export.h"

// Main application state
typedef struct {
  bool running;
  SceneContext sceneContext;
  ModelEditor modelEditor;
  GameWorld gameWorld;
  STLExportSettings exportSettings;
} AppState;

// Initialize application state
void InitApp(AppState *app) {
  app->running = true;

  // Initialize scene context
  InitSceneContext(&app->sceneContext);

  // Initialize model editor
  InitModelEditor(&app->modelEditor);

  // Initialize game world
  InitGameWorld(&app->gameWorld);

  // Set default export settings
  app->exportSettings.filename = "model.stl";
  app->exportSettings.useAsciiFormat = true;
  app->exportSettings.combineObjects = true;
  app->exportSettings.scaleToMillimeters = true;
}

// Process input based on current scene
void ProcessInput(AppState *app) {
  // Global input handling
  if (IsKeyPressed(KEY_ESCAPE)) {
    if (app->sceneContext.currentScene == SCENE_MAIN_MENU) {
      app->running = false;
    } else {
      SwitchToScene(&app->sceneContext, SCENE_MAIN_MENU);
    }
  }

  // Scene-specific input handling
  switch (app->sceneContext.currentScene) {
    case SCENE_MAIN_MENU:
      if (IsKeyPressed(KEY_ONE))
        SwitchToScene(&app->sceneContext, SCENE_MODEL_EDITOR);
      if (IsKeyPressed(KEY_TWO))
        SwitchToScene(&app->sceneContext, SCENE_GAME_WORLD);
      if (IsKeyPressed(KEY_THREE))
        SwitchToScene(&app->sceneContext, SCENE_EXPORT);
      break;

    case SCENE_MODEL_EDITOR:
      // Toggle tools based on keyboard shortcuts
      if (IsKeyPressed(KEY_G)) app->modelEditor.currentTool = TOOL_MOVE;
      if (IsKeyPressed(KEY_R)) app->modelEditor.currentTool = TOOL_ROTATE;
      if (IsKeyPressed(KEY_S)) app->modelEditor.currentTool = TOOL_SCALE;
      if (IsKeyPressed(KEY_TAB)) app->modelEditor.currentTool = TOOL_SELECT;

      // Primitive selection
      if (IsKeyPressed(KEY_ONE)) app->modelEditor.currentPrimitive = SHAPE_CUBE;
      if (IsKeyPressed(KEY_TWO))
        app->modelEditor.currentPrimitive = SHAPE_SPHERE;
      if (IsKeyPressed(KEY_THREE))
        app->modelEditor.currentPrimitive = SHAPE_CYLINDER;
      if (IsKeyPressed(KEY_FOUR))
        app->modelEditor.currentPrimitive = SHAPE_PYRAMID;
      if (IsKeyPressed(KEY_FIVE))
        app->modelEditor.currentPrimitive = SHAPE_PLANE;

      // Space to create primitive
      if (IsKeyPressed(KEY_SPACE)) {
        Vector3 pos = {0.0f, 0.0f, 0.0f};
        CreatePrimitive(&app->modelEditor, app->modelEditor.currentPrimitive,
                        pos);
      }

      // Delete selected object
      if (IsKeyPressed(KEY_DELETE)) {
        DeleteSelectedObject(&app->modelEditor);
      }
      break;

    case SCENE_GAME_WORLD:
      // Toggle physics
      if (IsKeyPressed(KEY_P)) {
        TogglePhysics(&app->gameWorld, !app->gameWorld.physicsEnabled);
      }

      // Toggle first-person mode
      if (IsKeyPressed(KEY_C)) {
        ToggleFirstPersonMode(&app->gameWorld);
      }

      // Reset game world
      if (IsKeyPressed(KEY_R)) {
        ResetGameWorld(&app->gameWorld);
      }
      break;

    case SCENE_EXPORT:
      // Export confirmation
      if (IsKeyPressed(KEY_ENTER)) {
        if (app->modelEditor.selectedObjectIndex >= 0) {
          ExportModelToSTL(
              &app->modelEditor.objects[app->modelEditor.selectedObjectIndex],
              app->exportSettings.filename, app->exportSettings.useAsciiFormat);
        } else {
          ExportAllModelsToSTL(&app->modelEditor, app->exportSettings.filename,
                               app->exportSettings.useAsciiFormat,
                               app->exportSettings.combineObjects);
        }
        SwitchToScene(&app->sceneContext, SCENE_MODEL_EDITOR);
      }
      break;

    default:
      break;
  }
}

// Update application state
void UpdateApp(AppState *app) {
  // Update current scene
  UpdateCurrentScene(&app->sceneContext);

  // Scene-specific updates
  switch (app->sceneContext.currentScene) {
    case SCENE_MODEL_EDITOR:
      UpdateModelEditor(&app->modelEditor, &app->sceneContext);
      break;

    case SCENE_GAME_WORLD:
      UpdateGameWorld(&app->gameWorld, &app->sceneContext);
      break;

    default:
      break;
  }
}

// Draw the application
void DrawApp(AppState *app) {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  // Draw current scene
  DrawCurrentScene(&app->sceneContext);

  // Scene-specific drawing
  switch (app->sceneContext.currentScene) {
    case SCENE_MAIN_MENU:
      DrawText("Raylib 3D Modeling & Game World Prototype", 20, 20, 30, BLACK);
      DrawText("1 - Model Editor", 20, 100, 20, DARKGRAY);
      DrawText("2 - Game World", 20, 130, 20, DARKGRAY);
      DrawText("3 - Export", 20, 160, 20, DARKGRAY);
      DrawText("ESC - Quit", 20, 220, 20, DARKGRAY);
      break;

    case SCENE_MODEL_EDITOR:
      DrawModelEditor(&app->modelEditor, &app->sceneContext);
      break;

    default:
      break;
  }

  EndDrawing();
}

// Clean up resources
void CleanupApp(AppState *app) {}

// Main entry point
int main(void) {
  // Initialize game world
  GameWorld world;
  InitGameWorld(&world);

  // Initialize raylib window
  InitWindow(800, 600, "3D Modeler");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Update physics
    UpdateWorldPhysics(&world, GetFrameTime());

    // Handle input
    if (IsKeyPressed(KEY_P)) TogglePhysics(&world, !world.physicsEnabled);
    if (IsKeyPressed(KEY_C)) ToggleFirstPersonMode(&world);

    // Rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Use shim to render world
    RenderWorld(&world);

    // Draw UI
    DrawText("Press P to toggle physics", 10, 10, 20, DARKGRAY);
    DrawText("Press C to toggle camera", 10, 40, 20, DARKGRAY);

    EndDrawing();
  }

  UnloadGameWorld(&world);
  CloseWindow();
  return 0;
}
