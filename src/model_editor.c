#include "model_editor.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

// Initialize the model editor
void InitModelEditor(ModelEditor *editor) {
  editor->currentTool = TOOL_SELECT;
  editor->currentPrimitive = SHAPE_CUBE;
  editor->objects = NULL;
  editor->objectCount = 0;
  editor->selectedObjectIndex = -1;
  editor->isDragging = false;
  editor->dragStart = (Vector3){0.0f, 0.0f, 0.0f};
  editor->showGrid = true;
  editor->gridSize = 1.0f;

  printf("Model editor initialized\n");
}

// Update the model editor state
void UpdateModelEditor(ModelEditor *editor, SceneContext *context) {
  // Handle tool operations
  if (editor->currentTool == TOOL_SELECT) {
    // Object selection with mouse click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Ray ray = GetMouseRay(GetMousePosition(), context->camera);
      SelectObject(editor, ray);
    }
  } else if (editor->currentTool == TOOL_MOVE &&
             editor->selectedObjectIndex >= 0) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editor->isDragging = true;
      Vector2 mousePos = GetMousePosition();
      editor->dragStart = (Vector3){mousePos.x, mousePos.y, 0.0f};
    }

    if (editor->isDragging) {
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Calculate movement based on mouse delta
        Vector2 mousePos = GetMousePosition();
        Vector2 delta = Vector2Subtract(
            mousePos, (Vector2){editor->dragStart.x, editor->dragStart.y});

        // Apply movement to selected object
        // This is a simplified movement - in a real app, you'd project mouse
        // movement to 3D space
        Vector3 movement = {
            delta.x * 0.01f,
            delta.y * -0.01f,  // Invert Y for more intuitive control
            0.0f};

        MoveSelectedObject(editor, movement);
        editor->dragStart = (Vector3){mousePos.x, mousePos.y, 0.0f};
      } else {
        editor->isDragging = false;
      }
    }
  } else if (editor->currentTool == TOOL_ROTATE &&
             editor->selectedObjectIndex >= 0) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editor->isDragging = true;
      Vector2 mousePos = GetMousePosition();
      editor->dragStart = (Vector3){mousePos.x, mousePos.y, 0.0f};
    }

    if (editor->isDragging) {
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Calculate rotation based on mouse delta
        Vector2 mousePos = GetMousePosition();
        Vector2 delta = Vector2Subtract(
            mousePos, (Vector2){editor->dragStart.x, editor->dragStart.y});

        // Apply rotation to selected object
        Vector3 rotation = {
            delta.y * 0.5f,  // Pitch
            delta.x * 0.5f,  // Yaw
            0.0f             // Roll (not used with mouse)
        };

        RotateSelectedObject(editor, rotation);
        editor->dragStart = (Vector3){mousePos.x, mousePos.y, 0.0f};
      } else {
        editor->isDragging = false;
      }
    }
  } else if (editor->currentTool == TOOL_SCALE &&
             editor->selectedObjectIndex >= 0) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      editor->isDragging = true;
      Vector2 mousePos = GetMousePosition();
      editor->dragStart = (Vector3){mousePos.x, mousePos.y, 0.0f};
    }

    if (editor->isDragging) {
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Calculate scale based on mouse delta
        Vector2 mousePos = GetMousePosition();
        Vector2 delta = Vector2Subtract(
            mousePos, (Vector2){editor->dragStart.x, editor->dragStart.y});

        // Apply scaling to selected object
        float scaleFactor = 1.0f + (delta.x + delta.y) * 0.01f;
        Vector3 scale = {scaleFactor, scaleFactor, scaleFactor};

        ScaleSelectedObject(editor, scale);
        editor->dragStart = (Vector3){mousePos.x, mousePos.y, 0.0f};
      } else {
        editor->isDragging = false;
      }
    }
  }
}

// Draw the model editor scene
void DrawModelEditor(ModelEditor *editor, SceneContext *context) {
  // Draw 3D elements
  BeginMode3D(context->camera);

  // Draw grid
  if (editor->showGrid) {
    DrawGrid(10, editor->gridSize);
  }

  // Draw all objects
  for (int i = 0; i < editor->objectCount; i++) {
    EditorObject *obj = &editor->objects[i];

    // Draw model with transforms
    DrawModelEx(obj->model, obj->position, (Vector3){0.0f, 1.0f, 0.0f},
                obj->rotation.y, obj->scale, obj->selected ? RED : obj->color);

    // Draw bounding box for selected object
    if (obj->selected) {
      DrawBoundingBox(GetModelBoundingBox(obj->model), GREEN);
    }
  }

  // Draw coordinate axes
  DrawLine3D((Vector3){0, 0, 0}, (Vector3){2, 0, 0}, RED);
  DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 2, 0}, GREEN);
  DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, 2}, BLUE);

  EndMode3D();

  // Draw UI elements
  DrawRectangle(10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
  DrawRectangleLines(10, 10, 320, 133, BLUE);

  // Tool info
  DrawText("Model Editor", 20, 20, 20, BLACK);

  // Tools
  DrawText(
      TextFormat("Tool: %s", editor->currentTool == TOOL_SELECT   ? "Select"
                             : editor->currentTool == TOOL_MOVE   ? "Move (G)"
                             : editor->currentTool == TOOL_ROTATE ? "Rotate (R)"
                             : editor->currentTool == TOOL_SCALE  ? "Scale (S)"
                                                                  : "Unknown"),
      20, 50, 10, BLACK);

  // Primitive type
  DrawText(TextFormat("Primitive: %s",
                      editor->currentPrimitive == SHAPE_CUBE       ? "Cube"
                      : editor->currentPrimitive == SHAPE_SPHERE   ? "Sphere"
                      : editor->currentPrimitive == SHAPE_CYLINDER ? "Cylinder"
                      : editor->currentPrimitive == SHAPE_PYRAMID  ? "Pyramid"
                      : editor->currentPrimitive == SHAPE_PLANE    ? "Plane"
                                                                   : "Unknown"),
           20, 70, 10, BLACK);

  // Object count
  DrawText(TextFormat("Objects: %d", editor->objectCount), 20, 90, 10, BLACK);

  // Controls
  DrawText("SPACE: Create | DEL: Delete | TAB: Switch Scene", 20, 110, 10,
           BLACK);
}

// Unload resources used by the model editor
void UnloadModelEditor(ModelEditor *editor) {
  // Unload all models
  for (int i = 0; i < editor->objectCount; i++) {
    UnloadModel(editor->objects[i].model);
  }

  // Free memory
  if (editor->objects != NULL) {
    free(editor->objects);
    editor->objects = NULL;
  }

  editor->objectCount = 0;
  editor->selectedObjectIndex = -1;
}

// Tool functions

// Select an object with a ray cast
void SelectObject(ModelEditor *editor, Ray ray) {
  float closestHit = 100000.0f;
  int closestObjectIndex = -1;

  // Check each object for intersection
  for (int i = 0; i < editor->objectCount; i++) {
    EditorObject *obj = &editor->objects[i];

    // Create a ray/box test based on the model's bounding box
    BoundingBox bounds = GetModelBoundingBox(obj->model);

    // Transform bounding box by object's transform
    // This is a simplified approach - in a real app, you'd transform the bounds
    // properly
    bounds.min = Vector3Add(bounds.min, obj->position);
    bounds.max = Vector3Add(bounds.max, obj->position);

    RayCollision collision = GetRayCollisionBox(ray, bounds);

    if (collision.hit && collision.distance < closestHit) {
      closestHit = collision.distance;
      closestObjectIndex = i;
    }
  }

  // Deselect previous selection
  if (editor->selectedObjectIndex >= 0) {
    editor->objects[editor->selectedObjectIndex].selected = false;
  }

  // Select new object
  editor->selectedObjectIndex = closestObjectIndex;

  if (editor->selectedObjectIndex >= 0) {
    editor->objects[editor->selectedObjectIndex].selected = true;
    printf("Selected object %d\n", editor->selectedObjectIndex);
  }
}

// Move the selected object
void MoveSelectedObject(ModelEditor *editor, Vector3 direction) {
  if (editor->selectedObjectIndex >= 0) {
    EditorObject *obj = &editor->objects[editor->selectedObjectIndex];
    obj->position = Vector3Add(obj->position, direction);
  }
}

// Rotate the selected object
void RotateSelectedObject(ModelEditor *editor, Vector3 rotation) {
  if (editor->selectedObjectIndex >= 0) {
    EditorObject *obj = &editor->objects[editor->selectedObjectIndex];
    obj->rotation = Vector3Add(obj->rotation, rotation);
  }
}

// Scale the selected object
void ScaleSelectedObject(ModelEditor *editor, Vector3 scale) {
  if (editor->selectedObjectIndex >= 0) {
    EditorObject *obj = &editor->objects[editor->selectedObjectIndex];
    obj->scale.x *= scale.x;
    obj->scale.y *= scale.y;
    obj->scale.z *= scale.z;

    // Prevent scaling to zero or negative
    if (obj->scale.x < 0.1f) obj->scale.x = 0.1f;
    if (obj->scale.y < 0.1f) obj->scale.y = 0.1f;
    if (obj->scale.z < 0.1f) obj->scale.z = 0.1f;
  }
}

// Create a new primitive shape
void CreatePrimitive(ModelEditor *editor, PrimitiveType type,
                     Vector3 position) {
  // Allocate or expand object array
  editor->objects = (EditorObject *)realloc(
      editor->objects, (editor->objectCount + 1) * sizeof(EditorObject));

  if (editor->objects == NULL) {
    printf("Failed to allocate memory for new primitive\n");
    return;
  }

  // Set up the new object
  EditorObject *obj = &editor->objects[editor->objectCount];
  obj->position = position;
  obj->rotation = (Vector3){0.0f, 0.0f, 0.0f};
  obj->scale = (Vector3){1.0f, 1.0f, 1.0f};
  obj->selected = false;
  obj->id = editor->objectCount;
  obj->type = type;

  // Create primitive model based on type
  switch (type) {
    case SHAPE_CUBE:
      obj->model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
      obj->color = BLUE;
      break;

    case SHAPE_SPHERE:
      obj->model = LoadModelFromMesh(GenMeshSphere(0.5f, 16, 16));
      obj->color = RED;
      break;

    case SHAPE_CYLINDER:
      obj->model = LoadModelFromMesh(GenMeshCylinder(0.5f, 1.0f, 16));
      obj->color = GREEN;
      break;

    case SHAPE_PYRAMID:
      obj->model = LoadModelFromMesh(
          GenMeshCube(1.0f, 1.0f, 1.0f));  // Replace with a cube for now
      obj->color = YELLOW;
      break;

    case SHAPE_PLANE:
      obj->model = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 1, 1));
      obj->color = PURPLE;
      break;

    default:
      obj->model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
      obj->color = GRAY;
      break;
  }

  editor->objectCount++;
  printf("Created new %d primitive, total objects: %d\n", type,
         editor->objectCount);
}

// Delete the selected object
void DeleteSelectedObject(ModelEditor *editor) {
  if (editor->selectedObjectIndex < 0 || editor->objectCount <= 0) {
    return;
  }

  // Unload the model
  UnloadModel(editor->objects[editor->selectedObjectIndex].model);

  // Shift remaining objects to fill the gap
  for (int i = editor->selectedObjectIndex; i < editor->objectCount - 1; i++) {
    editor->objects[i] = editor->objects[i + 1];
    editor->objects[i].id = i;  // Update ID
  }

  editor->objectCount--;

  // Reallocate array to new size (or free if empty)
  if (editor->objectCount > 0) {
    editor->objects = (EditorObject *)realloc(
        editor->objects, editor->objectCount * sizeof(EditorObject));
  } else {
    free(editor->objects);
    editor->objects = NULL;
  }

  // Reset selection
  editor->selectedObjectIndex = -1;

  printf("Deleted object, remaining: %d\n", editor->objectCount);
}
