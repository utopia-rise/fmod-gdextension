# Playing sounds

You can play sounds using `FmodServer` api.  

Here is an example:  
```gdscript
extends Area2D

# Declare member variables here. Examples:
# var a = 2
var music_file: FmodFile = null
var music: FmodSound = null

# Called when the node enters the scene tree for the first time.
func _ready():
	music_file = FmodServer.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	music = music_file.create_instance()
	music.play()
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	music.release()
```

This script loads an ogg file as an `FmodFile`. Then it connects godot's `body_entered` and `body_exited` signals.  
When we enter the node, it will create a sound instance using the file's `create_instance` method and play it.  
When we exit the node, it will release the sound.  
The file stays loaded as long as something references it: the script's `music_file` variable, or a sound instance
created from it. Once the node is freed and its instances are gone, the file is unloaded. Release every file and sound
instance before calling `FmodServer.shutdown()`.

## Positioning a sound

A sound instance can be placed in the world with its `position_3d` (a `Vector3`) and `velocity_3d`, or `position_2d` and
`velocity_2d` (`Vector2`) from a 2D scene. Setting any of them makes the sound 3D, so FMOD spatializes it against the
listeners:

```gdscript
var sound := FmodServer.load_file_as_sound("res://assets/Sounds/doorOpen_1.ogg").create_instance()
sound.position_3d = Vector3(2, 0, -5)
sound.play()
```

Positions use the same units as the nodes of the extension: meters in 3D, pixels in 2D, with the Distance Factor 2D and
Distance Factor 3D settings converting them to meters. The velocity is only used for the Doppler effect: update the position yourself as
the sound moves.
