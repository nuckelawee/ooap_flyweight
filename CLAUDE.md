# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Qt6 QML application named "particles" built with CMake. It uses Qt 6.8 with the Quick module and C++20. The application follows the standard Qt QML architecture with a C++ backend and QML frontend.

## Build System

The project uses CMake with presets for configuration:

**Configure (first time or after CMakeLists.txt changes):**
```bash
cmake --preset debug    # For debug builds
cmake --preset release  # For release builds
```

**Build:**
```bash
cmake --build build/debug      # Debug build
cmake --build build/release    # Release build
```

Or using the build presets:
```bash
cmake --build --preset debug-build
cmake --build --preset release-build
```

**Run the application:**
```bash
./build/debug/appparticles      # Debug build
./build/release/appparticles    # Release build
```

**Note:** Qt 6.8.0 is expected to be installed at `~/Qt/6.8.0/gcc_64`. If Qt is installed elsewhere, update `CMAKE_PREFIX_PATH` in `CMakePresets.json`.

## Architecture

### C++ Backend
- **Entry point:** `src/main.cpp` - Initializes QGuiApplication and QQmlApplicationEngine
- **Module name:** "particles" - QML module URI used to load the main QML file
- **C++ sources:** Additional C++ sources can be added to `src/CMakeLists.txt` (currently commented out)

### QML Frontend
- **Main QML file:** `qml/Main.qml` - Application UI entry point
- **QML module:** Registered as "particles" module with version 1.0
- Additional QML files should be added to the `QML_FILES` list in the root `CMakeLists.txt`

### Project Structure
```
particles/
├── CMakeLists.txt          # Main build configuration
├── CMakePresets.json       # Build presets (debug/release)
├── src/
│   ├── main.cpp           # Application entry point
│   └── CMakeLists.txt     # Additional C++ source configuration
├── qml/
│   └── Main.qml           # Main UI window
└── build/
    ├── debug/             # Debug build output
    └── release/           # Release build output
```

## Development Workflow

1. **Adding C++ sources:** Uncomment and modify `src/CMakeLists.txt` to add source files using `target_sources()`
2. **Adding QML files:** Add new QML files to `qml/` directory and list them in the `QML_FILES` section of the root `CMakeLists.txt`
3. **Reconfigure:** Run `cmake --preset debug` after modifying CMakeLists.txt files
4. **Rebuild:** Run `cmake --build build/debug` after code changes

## Key Qt Specifics

- The project uses Qt's standard project setup with `qt_standard_project_setup(REQUIRES 6.8)`
- QML module is created with `qt_add_qml_module()` which handles QML type registration
- The executable is configured for cross-platform deployment (MACOSX_BUNDLE, WIN32_EXECUTABLE)
- Compile commands are exported for IDE support (`CMAKE_EXPORT_COMPILE_COMMANDS`)
