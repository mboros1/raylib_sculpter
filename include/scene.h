#ifndef SCENE_H
#define SCENE_H

#include <raylib.h>

// Scene types
typedef enum {
    SCENE_MAIN_MENU = 0,
    SCENE_MODEL_EDITOR,
    SCENE_GAME_WORLD,
    SCENE_EXPORT
} SceneType;

// Scene context containing all data needed to manage scenes
typedef struct {
    SceneType currentScene;
    SceneType prevScene;
    bool sceneChanged;
    
    // Camera for 3D scenes
    Camera3D camera;
    
    // Add more scene-specific data as needed
} SceneContext;

// Functions for scene management
void InitSceneContext(SceneContext *context);
void SwitchToScene(SceneContext *context, SceneType scene);
void UpdateCurrentScene(SceneContext *context);
void DrawCurrentScene(SceneContext *context);
void UnloadCurrentScene(SceneContext *context);

#endif // SCENE_H 
