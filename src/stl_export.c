#include "stl_export.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STL binary header size (80 bytes)
#define STL_HEADER_SIZE 80

// Export a model to STL (autodetects format based on useAsciiFormat flag)
bool ExportModelToSTL(EditorObject* object, const char* filename, bool useAsciiFormat)
{
    if (!object)
        return false;

    if (useAsciiFormat) {
        return ExportModelToSTLAscii(object, filename);
    } else {
        return ExportModelToSTLBinary(object, filename);
    }
}

// Export all models in the scene to a combined STL file
bool ExportAllModelsToSTL(ModelEditor* editor, const char* baseFilename, bool useAsciiFormat,
                          bool combineObjects)
{
    if (!editor || editor->objectCount <= 0)
        return false;

    // Check if we should combine all objects into one file
    if (combineObjects) {
        // Create a combined file
        FILE* outFile = NULL;

        if (useAsciiFormat) {
            // ASCII STL format
            outFile = fopen(baseFilename, "w");
            if (!outFile)
                return false;

            fprintf(outFile, "solid RaylibScene\n");

            // Add each object
            for (int i = 0; i < editor->objectCount; i++) {
                EditorObject* obj = &editor->objects[i];

                // For each model, process its triangles
                Mesh mesh = obj->model.meshes[0];

                // Convert mesh to triangles
                // Note: This is simplified and assumes the mesh is already triangulated
                if (mesh.vertices && mesh.indices) {
                    int numFaces = mesh.triangleCount;

                    for (int f = 0; f < numFaces; f++) {
                        // Get vertices for this face
                        Vector3 v1, v2, v3;
                        int idx1 = mesh.indices[f * 3];
                        int idx2 = mesh.indices[f * 3 + 1];
                        int idx3 = mesh.indices[f * 3 + 2];

                        v1.x = mesh.vertices[idx1 * 3];
                        v1.y = mesh.vertices[idx1 * 3 + 1];
                        v1.z = mesh.vertices[idx1 * 3 + 2];

                        v2.x = mesh.vertices[idx2 * 3];
                        v2.y = mesh.vertices[idx2 * 3 + 1];
                        v2.z = mesh.vertices[idx2 * 3 + 2];

                        v3.x = mesh.vertices[idx3 * 3];
                        v3.y = mesh.vertices[idx3 * 3 + 1];
                        v3.z = mesh.vertices[idx3 * 3 + 2];

                        // Apply object transforms
                        // This would need a proper transform matrix in a real implementation

                        // Calculate normal (simplified)
                        Vector3 normal =
                            Vector3CrossProduct(Vector3Subtract(v2, v1), Vector3Subtract(v3, v1));
                        normal = Vector3Normalize(normal);

                        // Write the triangle to the STL file
                        fprintf(outFile, "facet normal %f %f %f\n", normal.x, normal.y, normal.z);
                        fprintf(outFile, "  outer loop\n");
                        fprintf(outFile, "    vertex %f %f %f\n", v1.x, v1.y, v1.z);
                        fprintf(outFile, "    vertex %f %f %f\n", v2.x, v2.y, v2.z);
                        fprintf(outFile, "    vertex %f %f %f\n", v3.x, v3.y, v3.z);
                        fprintf(outFile, "  endloop\n");
                        fprintf(outFile, "endfacet\n");
                    }
                }
            }

            fprintf(outFile, "endsolid RaylibScene\n");
        } else {
            // Binary STL format
            outFile = fopen(baseFilename, "wb");
            if (!outFile)
                return false;

            // Write STL header (80 bytes)
            char header[STL_HEADER_SIZE] = "Raylib 3D Model Exporter";
            fwrite(header, 1, STL_HEADER_SIZE, outFile);

            // Count total triangles
            int totalTriangles = 0;
            for (int i = 0; i < editor->objectCount; i++) {
                totalTriangles += editor->objects[i].model.meshes[0].triangleCount;
            }

            // Write total number of triangles (4 bytes)
            fwrite(&totalTriangles, 4, 1, outFile);

            // Add each object
            for (int i = 0; i < editor->objectCount; i++) {
                EditorObject* obj = &editor->objects[i];

                // Process each mesh
                Mesh mesh = obj->model.meshes[0];

                // Write each triangle
                if (mesh.vertices && mesh.indices) {
                    int numFaces = mesh.triangleCount;

                    for (int f = 0; f < numFaces; f++) {
                        // Get vertices for this face
                        Vector3 v1, v2, v3;
                        int idx1 = mesh.indices[f * 3];
                        int idx2 = mesh.indices[f * 3 + 1];
                        int idx3 = mesh.indices[f * 3 + 2];

                        v1.x = mesh.vertices[idx1 * 3];
                        v1.y = mesh.vertices[idx1 * 3 + 1];
                        v1.z = mesh.vertices[idx1 * 3 + 2];

                        v2.x = mesh.vertices[idx2 * 3];
                        v2.y = mesh.vertices[idx2 * 3 + 1];
                        v2.z = mesh.vertices[idx2 * 3 + 2];

                        v3.x = mesh.vertices[idx3 * 3];
                        v3.y = mesh.vertices[idx3 * 3 + 1];
                        v3.z = mesh.vertices[idx3 * 3 + 2];

                        // Apply object transforms (simplified)

                        // Calculate normal
                        Vector3 normal =
                            Vector3CrossProduct(Vector3Subtract(v2, v1), Vector3Subtract(v3, v1));
                        normal = Vector3Normalize(normal);

                        // Write normal (3 floats)
                        fwrite(&normal, 12, 1, outFile);

                        // Write vertices (9 floats)
                        fwrite(&v1, 12, 1, outFile);
                        fwrite(&v2, 12, 1, outFile);
                        fwrite(&v3, 12, 1, outFile);

                        // Write attribute byte count (2 bytes)
                        unsigned short attribCount = 0;
                        fwrite(&attribCount, 2, 1, outFile);
                    }
                }
            }
        }

        fclose(outFile);
        return true;
    } else {
        // Export each object to a separate file
        bool allSucceeded = true;

        for (int i = 0; i < editor->objectCount; i++) {
            char buffer[256];
            sprintf(buffer, "%s_%d.stl", baseFilename, i);

            if (!ExportModelToSTL(&editor->objects[i], buffer, useAsciiFormat)) {
                allSucceeded = false;
            }
        }

        return allSucceeded;
    }
}

// Export a model to ASCII STL format
bool ExportModelToSTLAscii(EditorObject* object, const char* filename)
{
    if (!object)
        return false;

    FILE* file = fopen(filename, "w");
    if (!file)
        return false;

    // Start solid
    fprintf(file, "solid %s\n", filename);

    // Get mesh data
    Mesh mesh = object->model.meshes[0];

    // Process each triangle in the mesh
    if (mesh.vertices && mesh.indices) {
        int numFaces = mesh.triangleCount;

        for (int f = 0; f < numFaces; f++) {
            // Get vertices for this face
            Vector3 v1, v2, v3;
            int idx1 = mesh.indices[f * 3];
            int idx2 = mesh.indices[f * 3 + 1];
            int idx3 = mesh.indices[f * 3 + 2];

            v1.x = mesh.vertices[idx1 * 3];
            v1.y = mesh.vertices[idx1 * 3 + 1];
            v1.z = mesh.vertices[idx1 * 3 + 2];

            v2.x = mesh.vertices[idx2 * 3];
            v2.y = mesh.vertices[idx2 * 3 + 1];
            v2.z = mesh.vertices[idx2 * 3 + 2];

            v3.x = mesh.vertices[idx3 * 3];
            v3.y = mesh.vertices[idx3 * 3 + 1];
            v3.z = mesh.vertices[idx3 * 3 + 2];

            // Apply object transforms (this is simplified)
            // In a real exporter, we'd apply proper transformations

            // Calculate normal
            Vector3 normal = Vector3CrossProduct(Vector3Subtract(v2, v1), Vector3Subtract(v3, v1));
            normal         = Vector3Normalize(normal);

            // Write the triangle to the STL file
            fprintf(file, "facet normal %f %f %f\n", normal.x, normal.y, normal.z);
            fprintf(file, "  outer loop\n");
            fprintf(file, "    vertex %f %f %f\n", v1.x, v1.y, v1.z);
            fprintf(file, "    vertex %f %f %f\n", v2.x, v2.y, v2.z);
            fprintf(file, "    vertex %f %f %f\n", v3.x, v3.y, v3.z);
            fprintf(file, "  endloop\n");
            fprintf(file, "endfacet\n");
        }
    }

    // End solid
    fprintf(file, "endsolid %s\n", filename);

    fclose(file);
    return true;
}

// Export a model to binary STL format
bool ExportModelToSTLBinary(EditorObject* object, const char* filename)
{
    if (!object)
        return false;

    FILE* file = fopen(filename, "wb");
    if (!file)
        return false;

    // Write STL header (80 bytes)
    char header[STL_HEADER_SIZE] = "Raylib 3D Model Exporter";
    fwrite(header, 1, STL_HEADER_SIZE, file);

    // Get mesh data
    Mesh mesh = object->model.meshes[0];

    // Write total number of triangles (4 bytes)
    int numFaces = mesh.triangleCount;
    fwrite(&numFaces, 4, 1, file);

    // Process each triangle in the mesh
    if (mesh.vertices && mesh.indices) {
        for (int f = 0; f < numFaces; f++) {
            // Get vertices for this face
            Vector3 v1, v2, v3;
            int idx1 = mesh.indices[f * 3];
            int idx2 = mesh.indices[f * 3 + 1];
            int idx3 = mesh.indices[f * 3 + 2];

            v1.x = mesh.vertices[idx1 * 3];
            v1.y = mesh.vertices[idx1 * 3 + 1];
            v1.z = mesh.vertices[idx1 * 3 + 2];

            v2.x = mesh.vertices[idx2 * 3];
            v2.y = mesh.vertices[idx2 * 3 + 1];
            v2.z = mesh.vertices[idx2 * 3 + 2];

            v3.x = mesh.vertices[idx3 * 3];
            v3.y = mesh.vertices[idx3 * 3 + 1];
            v3.z = mesh.vertices[idx3 * 3 + 2];

            // Apply object transforms (simplified)

            // Calculate normal
            Vector3 normal = Vector3CrossProduct(Vector3Subtract(v2, v1), Vector3Subtract(v3, v1));
            normal         = Vector3Normalize(normal);

            // Write normal (3 floats)
            fwrite(&normal, 12, 1, file);

            // Write vertices (9 floats)
            fwrite(&v1, 12, 1, file);
            fwrite(&v2, 12, 1, file);
            fwrite(&v3, 12, 1, file);

            // Write attribute byte count (2 bytes)
            unsigned short attribCount = 0;
            fwrite(&attribCount, 2, 1, file);
        }
    }

    fclose(file);
    return true;
}

// Show export dialog (this would integrate with a proper GUI in a real application)
void ShowExportDialog(STLExportSettings* settings)
{
    // In a real app, this would show UI controls for export settings
    // For now, we just print info to console
    printf("STL Export Settings:\n");
    printf("  Filename: %s\n", settings->filename);
    printf("  Format: %s\n", settings->useAsciiFormat ? "ASCII" : "Binary");
    printf("  Combine objects: %s\n", settings->combineObjects ? "Yes" : "No");
    printf("  Scale to mm: %s\n", settings->scaleToMillimeters ? "Yes" : "No");
}

// Get default export path
char* GetDefaultExportPath(const char* baseFilename)
{
    static char buffer[256];

    if (baseFilename) {
        strncpy(buffer, baseFilename, 255);
    } else {
        strcpy(buffer, "model.stl");
    }

    return buffer;
}