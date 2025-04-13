#include "stl_load.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Loads a binary STL file and returns a Mesh structure with vertices and
// normals populated. The caller is responsible for freeing the allocated memory
// in mesh.vertices and mesh.normals.
Mesh LoadSTL(const char *fileName) {
  Mesh mesh = {0};
  FILE *file = fopen(fileName, "rb");
  if (!file) {
    fprintf(stderr, "Cannot open STL file: %s\n", fileName);
    return mesh;
  }

  // Read 80-byte header
  char header[80];
  if (fread(header, 1, 80, file) != 80) {
    fprintf(stderr, "Cannot read STL file header\n");
    fclose(file);
    return mesh;
  }

  // Read number of triangles
  uint32_t triangleCount = 0;
  if (fread(&triangleCount, sizeof(uint32_t), 1, file) != 1) {
    fprintf(stderr, "Cannot read triangle count\n");
    fclose(file);
    return mesh;
  }

  int totalVertices = triangleCount * 3;
  mesh.vertexCount = totalVertices;
  mesh.triangleCount = triangleCount;

  // Allocate memory for vertices (3 floats per vertex) and normals (3 floats
  // per vertex)
  mesh.vertices = (float *)malloc(sizeof(float) * totalVertices * 3);
  mesh.normals = (float *)malloc(sizeof(float) * totalVertices * 3);
  if (!mesh.vertices || !mesh.normals) {
    fprintf(stderr, "Memory allocation failed\n");
    if (mesh.vertices) free(mesh.vertices);
    if (mesh.normals) free(mesh.normals);
    fclose(file);
    return mesh;
  }

  // For each triangle, read normal, vertices and attribute byte count
  for (uint32_t i = 0; i < triangleCount; i++) {
    float normal[3];
    float verts[9];  // 3 vertices * 3 coordinates each
    if (fread(normal, sizeof(float), 3, file) != 3) {
      fprintf(stderr, "Error reading normal for triangle %u\n", i);
      goto read_error;
    }
    if (fread(verts, sizeof(float), 9, file) != 9) {
      fprintf(stderr, "Error reading vertices for triangle %u\n", i);
      goto read_error;
    }
    unsigned short attribute;
    if (fread(&attribute, sizeof(unsigned short), 1, file) != 1) {
      fprintf(stderr, "Error reading attribute for triangle %u\n", i);
      goto read_error;
    }

    // Copy vertices
    int base = i * 9;
    memcpy(&mesh.vertices[base], verts, sizeof(float) * 9);

    // Replicate normal for each of the triangle's 3 vertices
    for (int j = 0; j < 3; j++) {
      int idx = i * 9 + j * 3;
      mesh.normals[idx] = normal[0];
      mesh.normals[idx + 1] = normal[1];
      mesh.normals[idx + 2] = normal[2];
    }
  }

  fclose(file);
  return mesh;

read_error:
  free(mesh.vertices);
  free(mesh.normals);
  mesh.vertices = NULL;
  mesh.normals = NULL;
  mesh.vertexCount = 0;
  mesh.triangleCount = 0;
  fclose(file);
  return mesh;
}

// #ifdef TEST_STL_LOAD
// Minimal test program to load an STL file using LoadSTL
int main(void) {
  const char *filePath = "./assets/cube.stl";
  printf("Loading STL file: %s\n", filePath);
  Mesh mesh = LoadSTL(filePath);
  if (mesh.vertexCount == 0) {
    printf("Failed to load mesh or mesh is empty.\n");
    return 1;
  }
  printf("Successfully loaded mesh. Triangle count: %d, Vertex count: %d\n",
         mesh.triangleCount, mesh.vertexCount);

  // Clean up allocated memory
  free(mesh.vertices);
  free(mesh.normals);
  return 0;
}
// #endif