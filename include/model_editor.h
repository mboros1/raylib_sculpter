#ifndef MODEL_EDITOR_H
#define MODEL_EDITOR_H

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include "scene.h"

// Tool types for the model editor
typedef enum {
    TOOL_SELECT = 0,
    TOOL_MOVE,
    TOOL_ROTATE,
    TOOL_SCALE,
    TOOL_CREATE
} EditorToolType;

// Primitive shape types that can be created
typedef enum {
    SHAPE_CUBE = 0,
    SHAPE_SPHERE,
    SHAPE_CYLINDER,
    SHAPE_PYRAMID,
    SHAPE_PLANE
} PrimitiveType;

// Object in the scene
typedef struct {
    Model model;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    bool selected;
    Color color;
    int id;
    PrimitiveType type;
} EditorObject;

// Editor state
typedef struct {
    EditorToolType currentTool;
    PrimitiveType currentPrimitive;
    EditorObject* objects;
    int objectCount;
    int selectedObjectIndex;
    bool isDragging;
    Vector3 dragStart;
    // Grid settings
    bool showGrid;
    float gridSize;
} ModelEditor;

// Initialize the model editor
void InitModelEditor(ModelEditor* editor);

// Update the model editor state
void UpdateModelEditor(ModelEditor* editor, SceneContext* context);

// Draw the model editor scene
void DrawModelEditor(ModelEditor* editor, SceneContext* context);

// Unload resources used by the model editor
void UnloadModelEditor(ModelEditor* editor);

// Tool functions
void SelectObject(ModelEditor* editor, Ray ray);
void MoveSelectedObject(ModelEditor* editor, Vector3 direction);
void RotateSelectedObject(ModelEditor* editor, Vector3 rotation);
void ScaleSelectedObject(ModelEditor* editor, Vector3 scale);
void CreatePrimitive(ModelEditor* editor, PrimitiveType type, Vector3 position);
void DeleteSelectedObject(ModelEditor* editor);

#endif // MODEL_EDITOR_H 
