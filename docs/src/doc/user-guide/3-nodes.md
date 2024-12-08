# High level nodes

We provide nodes to easily create your game using fmod.  
Those nodes are gdextension, so you can extends them using scripts like any other godot node.

## Fmod listeners

`FmodListener2D` and `FmodListener3D` are nodes to add a Fmod listener and bind it to them.
![listener-image]

### Properties:

#### listener_index
Index of the listener. You can have up to 8 listener concurrently.
#### is_locked
If `true`, listener will not update its position according to node's position.
#### weight
Used to compute the relative contribution to the final sound.


[fmod-bank-image]: ./assets/fmod-bank.png
[emitter-image]: ./assets/emitter.png
[listener-image]: ./assets/listeners.png
