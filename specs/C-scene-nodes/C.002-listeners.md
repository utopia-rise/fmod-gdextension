# C.2: Listeners

`FmodListener2D` and `FmodListener3D` make a node the ears of the game, and game code manages listeners directly when it needs to.

Positional audio is heard from somewhere: usually the camera or the player. A listener node binds one of FMOD's listeners to its own position. Split-screen and similar games use several listeners at once, each weighted in the final mix.

## Capabilities
- **C.2.1**: Game developers add a listener node to a 2D or 3D scene, and FMOD hears the world from that node's position while it is in the tree.
- **C.2.2**: Games use up to eight listeners at once, each with an index and a weight in the final mix.
- **C.2.3**: A listener is locked so that it stops following its node.
- **C.2.4**: Game code attaches any 2D or 3D node as a listener, removes it, sets its weight and lock, and reads or sets its transform and velocity.

## Rules
- **C.2.R1**: A listener whose node was freed falls silent, with a weight of zero, instead of failing.
- **C.2.R2**: The number of listeners FMOD uses is set before listeners are added, from Project Settings or from code, and is between one and eight.
