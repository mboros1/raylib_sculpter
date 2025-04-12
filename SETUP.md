# Setting Up Raylib 3D Sculpter

This document explains how to set up your environment to build and run the Raylib 3D Sculpter application.

## Prerequisites

You need to have the following tools installed:

1. C compiler (gcc/clang)
2. Make build system
3. Raylib library (version 4.x recommended)
4. VS Code with C/C++ extension (optional but recommended)

## Installing Raylib

### On macOS

Using Homebrew:

```bash
brew install raylib
```

### On Linux

Using package manager (Ubuntu/Debian):

```bash
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

### On Windows

1. Download and install MinGW: http://mingw-w64.org/
2. Download raylib installer from https://github.com/raysan5/raylib/releases
3. Or build from source following the instructions at https://github.com/raysan5/raylib/wiki/Working-on-Windows

## Building the Project

Once raylib is installed, you can build the project:

```bash
# Clone the repository (if you haven't already)
git clone https://github.com/yourusername/raylib_sculpter.git
cd raylib_sculpter

# Build the project
make

# Run the application
./ray3dproto
```

## VS Code Integration

This project includes VS Code configuration files:

- `.vscode/c_cpp_properties.json`: Configures the C/C++ extension
- `.vscode/tasks.json`: Defines build tasks
- `.vscode/launch.json`: Configures debugging
- `.vscode/settings.json`: Sets editor preferences

To build and run from VS Code:

1. Open VS Code in the project directory
2. Press `Ctrl+Shift+B` (or `Cmd+Shift+B` on macOS) to build
3. Press `F5` to build and debug

## Troubleshooting

### Raylib not found

If you get errors about raylib.h not being found:

1. Make sure raylib is installed on your system
2. Check the include paths in `.vscode/c_cpp_properties.json`
3. You may need to modify the Makefile to point to your raylib installation

### Build errors

If you encounter build errors:

1. Make sure you have the correct compiler installed
2. Check that raylib is properly installed
3. Check the Makefile for any platform-specific settings

For macOS users, you might need to update the frameworks in the Makefile depending on your system configuration. 