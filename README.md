# Raylib 3D Modeling & Game World Prototype

A simple C / raylib project for building 3D models in-app, then dropping them into a game world to test, play, and iterate — fast.

Inspired by:
- Blender's sculpting freedom
- LEGO Digital Designer's creativity
- Mario Maker's tight build-test loop
- Kid-friendly tools like Makers Empire

---

## Project Goals

Create a minimal, beginner-friendly 3D creation loop:
1. Build objects in a simple 3D modeling editor.
2. Drop them into a live game world immediately.
3. Play, test, break things.
4. Go back to editing — repeat.

Designed for learning designers, kids, and early game devs.

---

## Features (Planned)

- [x] Basic Scene Management
- [x] Primitive Shape Placement (Cube, Sphere, Cylinder, Pyramid)
- [x] Move / Rotate / Scale tools
- [ ] STL Export for 3D printing
- [ ] Game World sandbox to spawn creations
- [ ] File Save / Load
- [ ] Optional physics / interactions
- [ ] Minimal UI using raygui

---

## Build System

- Language: C
- Graphics/Window/Input: [raylib](https://www.raylib.com/)
- GUI: [raygui](https://github.com/raysan5/raygui)
- Build: `Makefile`

---

## Directory Structure

```
raylib-3d-proto/
├── include/              # Header files
│   ├── scene.h           # Scene enums, switching logic
│   ├── model_editor.h    # Model editing API
│   ├── game_world.h      # Game world API
│   └── stl_export.h      # STL export API
├── src/                  # Source code
│   ├── main.c            # Entry + scene loop
│   ├── scene.c           # Scene management
│   ├── model_editor.c    # 3D editor tools
│   ├── game_world.c      # Sandbox world logic
│   └── stl_export.c      # STL file writing
├── assets/               # Models / Textures / Icons
├── Makefile              # Build rules
└── README.md             # This file
```

---

## Building & Running

### Prerequisites
- C Compiler (GCC / clang / MSVC)
- raylib installed (v4.x preferred)
- raygui sources (dropped into `src/` is easiest)

### Build

```bash
make
```

### Run

```bash
./ray3dproto
```

---

## Controls

| Key | Action |
|-----|--------|
| Left Click | Select object |
| WASD | Move camera |
| Scroll | Zoom |
| G | Grab / Move tool |
| R | Rotate tool |
| S | Scale tool |
| Tab | Switch Scene |
| ESC | Quit / Back |

---

## Scenes

| Scene | Purpose |
|-------|---------|
| Main Menu | Navigation hub |
| Model Editor | Build & modify models |
| Game World | Play & test models |
| Export | STL Export confirmation |

---

## License

MIT License — do whatever you want, have fun.

---

## Credits

- raylib by raysan5
- raygui for simple UI
- Inspired by Makers Empire, LEGO tools, Blender

---

## Future Ideas

- Snap-to-grid toggle
- Voxel export option
- Shared online model library
- Lua scripting for world behaviors
- Sound / particle system for fun feedback
- Multiplayer model jam mode

---

This is a clean starting point:
- Friendly for GitHub readers
- Easy to evolve into a real project repo
- Clear vision, scope, build, structure, and future ideas
- Works perfectly with Cursor IDE's code-as-doc mentality

---

Want me to:
- Mock up a Makefile?
- Sketch an example `scene.h` + `main.c` structure?
- Provide a simple starter zip/repo for this?
