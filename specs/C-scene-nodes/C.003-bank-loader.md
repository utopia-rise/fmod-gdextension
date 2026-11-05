# C.3: Bank loader

`FmodBankLoader` loads a list of banks when it enters the scene tree and lets them go when it leaves.

A scene that needs some banks loads them with a bank loader placed before the nodes that use them; an autoload bank loader keeps banks for the whole game. Banks are shared: several loaders, or a loader and game code, holding the same bank load it once (B.1.R2).

Picking banks in the Inspector is E.2.

## Capabilities
- **C.3.1**: Game developers list banks on a bank loader in the Inspector, in loading order, and the loader loads them when it enters the tree.
- **C.3.2**: A bank loader releases its banks when it leaves the tree, and a bank stays loaded while anything else still holds it.
- **C.3.3**: Banks added to or removed from the list of a bank loader in the tree are loaded or released right away.
- **C.3.4**: Game developers make a bank loader load the sample data of its banks' events with the banks, and unload it when the loader releases them.

## Rules
- **C.3.R1**: A bank loader loads its banks in the order listed, so that dependencies such as the master banks are listed first.
- **C.3.R2**: A bank loader loads nothing in the editor.
