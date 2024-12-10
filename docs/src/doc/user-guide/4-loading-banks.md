# Loading banks

In this guide we'll explore how to load banks within your game.  

## FmodBankLoader node

`FmodBankLoader` is in charge of loading banks when entering the scene. You should place it, in the scene hierarchy,
before all other fmod nodes using this bank. Banks are unloaded on exit tree.  

Banks are `RefCounted`, so several `FmodBankLoader` can share same banks.

If you want to load your bank when starting game and keep them loaded, use this node and add it as **autoload** node.  

You can add banks with fmod project explorer, using the `+` button with bank icon, or manually add a bank using bottom
line edit. You can also remove and re-order banks:  
![fmod-bank-image]

!!! warning
Make sure to first place `Master.strings.bank` first, and `Master.bank` in second. Those banks are dependencies needed
by other banks. So if you don't load them first, you won't be able to load other banks.

## FmodServer api

You can also load banks using `FmodServer` api.  
For this purpose, you should use `load_bank` method of `FmodServer` singleton.  

Here is an example:  
```gdscript
var banks := Array()

func _ready():
    banks.append(FmodServer.load_bank("res://Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))
    banks.append(FmodServer.load_bank("res://Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))
    banks.append(FmodServer.load_bank("res://Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))
```

!!! warning
As banks are `RefCounted`, don't forget to store them. Otherwise reference counter will be directly decremented leading
to unload of the bank.

!!! warning
Make sure to first load `Master.strings.bank`, and `Master.bank` in second. Those banks are dependencies needed by other
banks. So if you don't load them first, you won't be able to load other banks.

[fmod-bank-image]: ./assets/fmod-bank.png