# B.3: One-shots

Game code plays an event once, without keeping its instance, optionally following a node.

Most sound effects are fired and forgotten: a footstep, an impact, a UI click. Game code should not have to keep and release an instance for them. A one-shot attached to a node follows it while it plays, and ends cleanly if the node goes away.

Events whose instance game code keeps are B.2.

## Capabilities
- **B.3.1**: Game code plays an event once by its path, its GUID or its description, without handling its instance.
- **B.3.2**: Game code plays a one-shot with initial parameter values.
- **B.3.3**: Game code plays a one-shot attached to a node, whose position the event follows while it plays.

## Rules
- **B.3.R1**: A one-shot releases itself when it finishes, and an attached one-shot stops following a node that left the tree or was freed, without error.
