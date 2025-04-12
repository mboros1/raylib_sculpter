#ifndef STL_EXPORT_H
#define STL_EXPORT_H

#include "model_editor.h"
#include <raylib.h>

// STL export settings
typedef struct {
    const char* filename;
    bool useAsciiFormat; // STL can be ASCII or binary
    bool combineObjects; // Export as a single STL or multiple files
    bool scaleToMillimeters;
} STLExportSettings;

// Functions for STL export
bool ExportModelToSTL(EditorObject* object, const char* filename, bool useAsciiFormat);
bool ExportAllModelsToSTL(ModelEditor* editor, const char* baseFilename, bool useAsciiFormat,
                          bool combineObjects);
bool ExportModelToSTLAscii(EditorObject* object, const char* filename);
bool ExportModelToSTLBinary(EditorObject* object, const char* filename);

// Utility functions
void ShowExportDialog(STLExportSettings* settings);
char* GetDefaultExportPath(const char* baseFilename);

#endif // STL_EXPORT_H
