#include "raylib_mocks.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

// Tracking variables for mock calls
bool DrawModelEx_called = false;
bool BeginMode3D_called = false;
bool EndMode3D_called = false;
bool DrawText_called = false;
bool DrawRectangle_called = false;
bool DrawRectangleLines_called = false;
bool DrawPlane_called = false;
bool DrawGrid_called = false;
bool DrawSphere_called = false;
bool UnloadModel_called = false;

// Buffer for TextFormat mock
static char textFormatBuffer[1024];

// Initialize/reset mock tracking
void init_raylib_mocks(void)
{
    DrawModelEx_called = false;
    BeginMode3D_called = false;
    EndMode3D_called = false;
    DrawText_called = false;
    DrawRectangle_called = false;
    DrawRectangleLines_called = false;
    DrawPlane_called = false;
    DrawGrid_called = false;
    DrawSphere_called = false;
    UnloadModel_called = false;
    memset(textFormatBuffer, 0, sizeof(textFormatBuffer));
}

// Mock implementations of raylib functions

void BeginMode3D(Camera3D camera)
{
    BeginMode3D_called = true;
    // No real 3D mode to begin, just track the call
    (void)camera; // Prevent unused parameter warning
}

void EndMode3D(void)
{
    EndMode3D_called = true;
    // No real 3D mode to end, just track the call
}

void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint)
{
    DrawModelEx_called = true;
    // No real drawing, just track the call
    (void)model;
    (void)position;
    (void)rotationAxis;
    (void)rotationAngle;
    (void)scale;
    (void)tint;
}

void DrawText(const char *text, int posX, int posY, int fontSize, Color color)
{
    DrawText_called = true;
    // No real drawing, just track the call
    (void)text;
    (void)posX;
    (void)posY;
    (void)fontSize;
    (void)color;
}

void DrawRectangle(int posX, int posY, int width, int height, Color color)
{
    DrawRectangle_called = true;
    // No real drawing, just track the call
    (void)posX;
    (void)posY;
    (void)width;
    (void)height;
    (void)color;
}

void DrawRectangleLines(int posX, int posY, int width, int height, Color color)
{
    DrawRectangleLines_called = true;
    // No real drawing, just track the call
    (void)posX;
    (void)posY;
    (void)width;
    (void)height;
    (void)color;
}

void DrawPlane(Vector3 centerPos, Vector2 size, Color color)
{
    DrawPlane_called = true;
    // No real drawing, just track the call
    (void)centerPos;
    (void)size;
    (void)color;
}

void DrawGrid(int slices, float spacing)
{
    DrawGrid_called = true;
    // No real drawing, just track the call
    (void)slices;
    (void)spacing;
}

void DrawSphere(Vector3 centerPos, float radius, Color color)
{
    DrawSphere_called = true;
    // No real drawing, just track the call
    (void)centerPos;
    (void)radius;
    (void)color;
}

void UnloadModel(Model model)
{
    UnloadModel_called = true;
    // No real unloading needed, just track the call
    (void)model;
}

// Simplified Vector3 operations for testing

Vector3 Vector3Add(Vector3 v1, Vector3 v2)
{
    Vector3 result = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z};
    return result;
}

Vector3 Vector3Subtract(Vector3 v1, Vector3 v2)
{
    Vector3 result = {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z};
    return result;
}

Vector3 Vector3Scale(Vector3 v, float scalar)
{
    Vector3 result = {
        v.x * scalar,
        v.y * scalar,
        v.z * scalar};
    return result;
}

Vector3 Vector3Normalize(Vector3 v)
{
    Vector3 result = {0};
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

    if (length > 0)
    {
        float ilength = 1.0f / length;
        result.x = v.x * ilength;
        result.y = v.y * ilength;
        result.z = v.z * ilength;
    }

    return result;
}

Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2)
{
    Vector3 result = {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x};
    return result;
}

Vector3 Vector3RotateByAxisAngle(Vector3 v, Vector3 axis, float angle)
{
    // Simplified implementation for testing
    // In real tests, we might not need the full implementation
    return v;
}

// Color operations

Color Fade(Color color, float alpha)
{
    if (alpha < 0.0f)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;

    Color result = {
        color.r,
        color.g,
        color.b,
        (unsigned char)(255.0f * alpha)};

    return result;
}

// Other mock functions

const char *TextFormat(const char *text, ...)
{
    va_list args;
    va_start(args, text);
    vsnprintf(textFormatBuffer, sizeof(textFormatBuffer), text, args);
    va_end(args);

    return textFormatBuffer;
}

Vector2 GetMouseDelta(void)
{
    Vector2 delta = {0.0f, 0.0f};
    return delta;
}

float GetFrameTime(void)
{
    return 0.016f; // Simulate 60 FPS
}

bool IsKeyDown(int key)
{
    return false; // Default to no keys pressed
}

bool IsKeyPressed(int key)
{
    return false; // Default to no keys pressed
}