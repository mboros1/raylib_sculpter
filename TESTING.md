# Testing Strategy for Raylib 3D Modeler

## Architecture & Unit Testing Philosophy

This project follows a clean separation of concerns to enable effective unit testing:

1. **Core World Logic** (pure data + functions)
2. **IO Layer** (rendering, input handling, file operations)
3. **Adapters** (binding external systems to world logic)

This architecture enables unit testing without needing to spin up the real runtime environment (like raylib) for every test.

## Current Architecture

The current codebase is structured around these main components:

- `game_world.c/.h` - Game simulation and physics
- `model_editor.c/.h` - Editor functionality and tools
- `scene.h` - Scene management system
- `stl_export.h` - 3D model export functionality
- `main.c` - Application entry point and management

## Path to Testable Architecture

To make the codebase more testable, we're moving toward greater separation between:

### World Library (world.c/.h)

This will be a pure C library with:

- No dependencies on rendering, input, or networking
- All game/server rules, state, and transformations
- Can be tested in isolation

Example API:
```c
typedef struct World World;
void World_Init(World* w);
void World_Step(World* w, float dt);
void World_AddEntity(World* w, Vector3 position);
```

### IO Layer: Separated & Swappable

External systems (like raylib) will interact with the world only via explicit interfaces:

Example:
```c
typedef struct {
    void (*RenderWorld)(const World* world);
    void (*RenderUI)(const World* world);
} RendererIO;
```

This allows:
- Production code to use real raylib implementations
- Unit tests to provide fake/mocked versions of IO

## Mocking Strategy in C

No external mocking library required. The pattern will be:

- Real implementations in real_*.c
- Test mocks in mock_*.c
- Interface declared in .h
- Build system selects which to link based on target

Example mock:
```c
void MockRenderWorld(const World* world) {
    // Just track that it was called, no actual rendering
    render_world_called = true;
}
```

## Unit Testing Strategy

### World Unit Tests

Pure tests of the world logic:
- No IO dependencies
- Deterministic & fast
- Validate state transitions & rules

Example:
```c
World w;
World_Init(&w);
World_AddEntity(&w, (Vector3){1,2,3});
assert(w.entityCount == 1);
```

### IO Adapter Unit Tests

Tests of rendering/input logic:
- Validate correct function calls
- Use mocks for all IO functions
- Check correct parameters are passed

## Build System for Testing

The Makefile will be extended to support:

```
make             # builds production binary
make test        # builds and runs unit tests with mocks
```

## Directory Structure for Tests

```
raylib_sculpter/
├── src/              # Main source files
├── include/          # Header files
├── tests/            # Unit tests
│   ├── test_world.c  # World logic tests
│   ├── test_io.c     # IO adapter tests
│   └── mocks/        # Mock implementations
├── Makefile          # Build targets
└── TESTING.md        # This document
```

## Benefits of This Approach

| Benefit | Why it Matters |
|---------|----------------|
| IDE friendly | All functions are declared in headers, easy navigation |
| Testable everywhere | No raylib runtime needed to test core logic |
| Easy mocking | Manual, explicit, no framework magic |
| Portable | Works in any C compiler / build environment |
| Scalable | Natural growth into multiplayer, networking, persistence |

## Next Steps

1. Gradually refactor game_world.c to separate pure logic from raylib-dependent code
2. Create mock implementations of raylib functions needed for testing
3. Set up initial unit tests for core game world logic
4. Extend Makefile to support building and running tests 