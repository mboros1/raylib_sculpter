#pragma once
#include <raylib.h>

#include "game_world.h"

// Conversion helpers
Vector3 ToRaylibVector3(WorldVector3 v);
Color ToRaylibColor(WorldColor c);

// Main rendering function
void RenderWorld(const GameWorld *world);