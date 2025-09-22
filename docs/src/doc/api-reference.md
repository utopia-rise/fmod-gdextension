# API Reference

The FMOD GDExtension provides comprehensive API documentation directly within Godot 4.3+. This documentation appears in Godot's Help system and provides detailed information about all FMOD classes, methods, properties, and signals.

## Accessing In-Engine Documentation

With the FMOD GDExtension enabled in your project, you can access comprehensive API documentation directly in Godot:

1. **Help Dock**: Open the Help dock in Godot (`Window` > `Help`)
2. **Search for Classes**: Type "Fmod" to see all available FMOD classes
3. **Browse Documentation**: Click on any class to view detailed documentation

## Available Classes

The following FMOD classes are fully documented in-engine:

### Core Classes
- **FmodServer** - Main singleton for audio system management
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

## Documentation Features

Each class includes:

- **Comprehensive class descriptions** explaining purpose and usage
- **Detailed method documentation** with parameter and return value descriptions  
- **Property documentation** with default values and behavior information
- **Signal documentation** for callback events
- **Usage examples** and best practices

## Example: Getting Help

```gdscript
# Get help for any FMOD class directly in Godot
var server = FmodServer.get_singleton()  # Press F1 to see documentation
var event = server.create_event_instance("event:/Music/Level1")  # Method docs available
```

The documentation automatically loads when the FMOD GDExtension is enabled, providing immediate access to comprehensive help without external dependencies.

## Source Files

The documentation is generated from XML files located in the `doc_classes/` directory of the FMOD GDExtension source code. These files follow Godot's class documentation schema and are automatically integrated into the Help system.