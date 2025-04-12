#ifndef RAYLIB_MOCKS_H
#define RAYLIB_MOCKS_H

// We don't want to include the real raylib.h/raymath.h since we're mocking it
// Instead, define the minimal structures we need
#ifndef RAYLIB_H
// Basic Vector/Matrix types
typedef struct
{
    float x;
    float y;
} Vector2;

typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} Vector4;

typedef Vector4 Quaternion;

typedef struct
{
    float m0, m4, m8, m12;
    float m1, m5, m9, m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;
} Matrix;

typedef struct
{
    unsigned int id; // OpenGL texture id
    int width;       // Texture base width
    int height;      // Texture base height
    int mipmaps;     // Mipmap levels, 1 by default
    int format;      // Data format (PixelFormat)
} Texture2D;

typedef struct
{
    Texture2D texture; // Mesh texture
    Color color;       // Mesh color
} Material;

typedef struct
{
    int vertexCount;         // Number of vertices stored in arrays
    int triangleCount;       // Number of triangles stored (indexed or not)
    float *vertices;         // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;        // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *texcoords2;       // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    float *normals;          // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float *tangents;         // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    unsigned char *colors;   // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices; // Vertex indices (in case vertex data comes indexed)
    unsigned int vaoId;      // OpenGL Vertex Array Object id
    unsigned int *vboId;     // OpenGL Vertex Buffer Objects id (default vertex data)
} Mesh;

typedef struct
{
    Matrix transform;    // Local transform matrix
    int meshCount;       // Number of meshes
    int materialCount;   // Number of materials
    Mesh *meshes;        // Meshes array
    Material *materials; // Materials array
    int *meshMaterial;   // Mesh material number
} Model;

typedef struct
{
    Vector3 position; // Camera position
    Vector3 target;   // Camera target it looks-at
    Vector3 up;       // Camera up vector (rotation over its axis)
    float fovy;       // Camera field-of-view aperture in Y (degrees)
    int projection;   // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} Camera3D;

typedef Camera3D Camera; // Camera type fallback, defaults to Camera3D

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

typedef struct
{
    Vector3 position;  // Ray position (origin)
    Vector3 direction; // Ray direction
} Ray;

// Define key constants
#define KEY_NULL 0
#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68
#define KEY_F 70
#define KEY_P 80
#define KEY_R 82
#define KEY_LEFT_ALT 342
#define CAMERA_PERSPECTIVE 0

// Colors
#define LIGHTGRAY (Color){200, 200, 200, 255}
#define GRAY (Color){130, 130, 130, 255}
#define RED (Color){230, 41, 55, 255}
#define SKYBLUE (Color){102, 191, 255, 255}
#define BLUE (Color){0, 121, 241, 255}
#define BLACK (Color){0, 0, 0, 255}

#endif // RAYLIB_H

#include <stdbool.h>

// Tracking variables for mock calls
extern bool DrawModelEx_called;
extern bool BeginMode3D_called;
extern bool EndMode3D_called;
extern bool DrawText_called;
extern bool DrawRectangle_called;
extern bool DrawRectangleLines_called;
extern bool DrawPlane_called;
extern bool DrawGrid_called;
extern bool DrawSphere_called;
extern bool UnloadModel_called;

// Function to initialize/reset mock tracking variables
void init_raylib_mocks(void);

// Mock implementations of raylib functions
void BeginMode3D(Camera3D camera);
void EndMode3D(void);
void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint);
void DrawText(const char *text, int posX, int posY, int fontSize, Color color);
void DrawRectangle(int posX, int posY, int width, int height, Color color);
void DrawRectangleLines(int posX, int posY, int width, int height, Color color);
void DrawPlane(Vector3 centerPos, Vector2 size, Color color);
void DrawGrid(int slices, float spacing);
void DrawSphere(Vector3 centerPos, float radius, Color color);
void UnloadModel(Model model);

// Mock Vector3 operations
Vector3 Vector3Add(Vector3 v1, Vector3 v2);
Vector3 Vector3Subtract(Vector3 v1, Vector3 v2);
Vector3 Vector3Scale(Vector3 v, float scalar);
Vector3 Vector3Normalize(Vector3 v);
Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2);
Vector3 Vector3RotateByAxisAngle(Vector3 v, Vector3 axis, float angle);

// Mock fade function
Color Fade(Color color, float alpha);

// Other mock functions as needed
const char *TextFormat(const char *text, ...);
Vector2 GetMouseDelta(void);
float GetFrameTime(void);
bool IsKeyDown(int key);
bool IsKeyPressed(int key);

#endif // RAYLIB_MOCKS_H