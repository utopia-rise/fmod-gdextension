# FMOD GDExtension Documentation

This directory contains comprehensive XML documentation for all FMOD GDExtension classes, providing in-engine help in Godot 4.3+.

## Documentation Coverage

The following classes are fully documented:

### Core Classes
- **FmodServer** - Main singleton for audio system management (50+ methods)
- **FmodEvent** - Event instances with playback and parameter control
- **FmodEventDescription** - Event metadata and factory methods

### Node Classes
- **FmodEventEmitter2D/3D** - Positioned audio emitters with automatic transform tracking
- **FmodListener2D/3D** - Audio listener nodes for spatial audio positioning
- **FmodBankLoader** - Utility node for automatic bank loading

### Audio System Classes
- **FmodBank** - Bank management with loading state and content inspection
- **FmodBus** - Audio bus controls for mixing and effects
- **FmodVCA** - Volume control automation for centralized volume management
- **FmodParameterDescription** - Parameter metadata for validation and UI

## Usage

The documentation appears automatically in Godot's Help system when the FMOD GDExtension is loaded. You can access it by:

1. Opening the Help dock in Godot
2. Searching for "Fmod" classes
3. Clicking on any FMOD class to view its documentation

## Documentation Features

Each class includes:
- Comprehensive class descriptions explaining purpose and usage
- Detailed method documentation with parameter and return value descriptions
- Property documentation with default values and behavior
- Signal documentation for callback events
- Usage examples and best practices

## Technical Details

- Documentation follows Godot's XML class documentation schema
- Files are located in `res://addons/fmod/doc_classes/`
- Configuration is added to `fmod.gdextension` under `[documentation]` section
- All 12 XML files have been validated for correct syntax

The documentation provides a complete reference for FMOD integration without requiring external API documentation, making it easier for developers to learn and use the FMOD GDExtension effectively.