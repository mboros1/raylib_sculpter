#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Include our raylib mocks first to prevent conflicts
#include "mocks/raylib_mocks.h"

// Then include the game world implementation
#include "../include/game_world.h"

// Test helper function to create a basic game world for testing
static GameWorld *create_test_world(void)
{
    GameWorld *world = (GameWorld *)malloc(sizeof(GameWorld));
    if (!world)
    {
        fprintf(stderr, "Failed to allocate memory for test world\n");
        exit(1);
    }

    InitGameWorld(world);
    return world;
}

// Test helper function to clean up a test world
static void destroy_test_world(GameWorld *world)
{
    if (world)
    {
        UnloadGameWorld(world);
        free(world);
    }
}

// Test initialization of game world
static void test_init_game_world(void)
{
    printf("Testing game world initialization...\n");

    GameWorld *world = create_test_world();

    // Verify initial state
    assert(world->objects == NULL);
    assert(world->objectCount == 0);
    assert(world->selectedObjectIndex == -1);
    assert(world->showGrid == true);
    assert(world->physicsEnabled == false);
    assert(world->firstPersonMode == false);

    destroy_test_world(world);
    printf("Game world initialization test passed\n");
}

// Test physics toggle functionality
static void test_toggle_physics(void)
{
    printf("Testing physics toggle...\n");

    GameWorld *world = create_test_world();

    // Test enabling physics
    TogglePhysics(world, true);
    assert(world->physicsEnabled == true);

    // Test disabling physics
    TogglePhysics(world, false);
    assert(world->physicsEnabled == false);

    destroy_test_world(world);
    printf("Physics toggle test passed\n");
}

// Test camera mode toggle functionality
static void test_toggle_first_person_mode(void)
{
    printf("Testing first-person mode toggle...\n");

    GameWorld *world = create_test_world();

    // Initial state should be orbit mode (first-person disabled)
    assert(world->firstPersonMode == false);

    // Toggle to first-person mode
    ToggleFirstPersonMode(world);
    assert(world->firstPersonMode == true);

    // Toggle back to orbit mode
    ToggleFirstPersonMode(world);
    assert(world->firstPersonMode == false);

    destroy_test_world(world);
    printf("First-person mode toggle test passed\n");
}

// Main test function
int main(void)
{
    printf("Running game world unit tests...\n");

    // Initialize mock tracking
    init_raylib_mocks();

    // Run tests
    test_init_game_world();
    test_toggle_physics();
    test_toggle_first_person_mode();

    printf("All game world tests passed!\n");
    return 0;
}