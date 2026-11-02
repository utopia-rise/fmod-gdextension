# B.4: Parameters

Game code drives the parameters a sound designer exposed in FMOD Studio, on one event instance or globally.

Parameters are how a game talks to its sound design: an engine's RPM, a surface type, the intensity of a fight. They exist per event instance and globally, are found by name or by ID, and labeled parameters take their labels as values.

Parameters exposed as node properties are C.1.

## Capabilities
- **B.4.1**: Game code reads and sets an event instance's parameters by name or by ID.
- **B.4.2**: Game code reads and sets global parameters by name or by ID.
- **B.4.3**: Game code sets a labeled parameter by its label, on an event instance or globally.
- **B.4.4**: Game code lists the parameters of an event or the global ones, with their description: range, default value, type, flags and labels.
