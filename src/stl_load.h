#ifndef STL_LOAD_H
#define STL_LOAD_H

#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Loads an STL file and returns a Mesh structure.
// The caller is responsible for freeing the allocated memory (using UnloadMesh
// if available, or free manually).
Mesh LoadSTL(const char *fileName);

#ifdef __cplusplus
}
#endif

#endif  // STL_LOAD_H