# Playing sounds

You can play sounds using `FmodServer` api.  

Here is an example:  
```gdscript
extends Area2D

# Declare member variables here. Examples:
# var a = 2
var music: FmodSound = null

# Called when the node enters the scene tree for the first time.
func _ready():
	FmodServer.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	music = FmodServer.create_sound_instance("res://assets/Music/jingles_SAX07.ogg")
	music.play()
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	music.release()

func _exit_tree():
	FmodServer.unload_file("res://assets/Music/jingles_SAX07.ogg")
```

This script loads an ogg file. Then it connects godot's `body_entered` and `body_exited` signals.  
When we enter the node, it will create a sound instance using `create_sound_instance` method and play it.  
When we exit the node, it will release the sound.  
At the end, we the node is removed from godot tree, it unloads the sound file.