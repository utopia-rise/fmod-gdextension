# Listeners

In this guide we'll cover how to add FMOD listeners to your godot project.

## Fmod listener nodes

`FmodListener2D` and `FmodListener3D` are nodes to add a FMOD listener and bind it to them, this means position of the 
FMOD listener will match position of the `FmodListener` node.  

![listener-image]

### Properties:

#### listener_index
Index of the listener. You can have up to 8 listener concurrently.
#### is_locked
If `true`, listener will not update its position according to node's position.
#### weight
Used to compute the relative contribution to the final sound.

## Using FmodServer api

You can add and remove listeners using `FmodServer` api:  

```gdscript
func _ready():
	FmodServer.add_listener(0, self)
```

In this script we set a listener with index `0` and attached it to the current node.  

```gdscript
func _ready():
	FmodServer.remove_listener(0, self)
```

In this script we remove the listener with index `0` attached to the current node.  

You can also set the listener weight using `FmodServer`:  
```gdscript
FmodServer.set_system_listener_weight(0, 1.5)
```

In this script we set the weight of the listener with index `0` to `1.5`.


[listener-image]: ./assets/listeners.png