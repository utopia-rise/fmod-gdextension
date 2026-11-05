# Loading banks

In this guide we'll explore how to load banks within your game.  

## FmodBankLoader node

`FmodBankLoader` is in charge of loading banks when entering the scene. You should place it, in the scene hierarchy,
before all other fmod nodes using this bank. Banks are unloaded on exit tree.  

Banks are `RefCounted`, so several `FmodBankLoader` can share same banks.

If you want to load your bank when starting game and keep them loaded, use this node and add it as **autoload** node.  

Adding a bank to the loader while it is in the scene loads it, and removing one releases it.

Check **Preload Sample Data** to also load the sample data of the banks' events, so they play without loading delay.
The loader unloads it when it releases the banks.

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
    banks.append(FmodServer.load_bank("res://Master.bank", FmodBank.StudioLoadBankFlags.NORMAL))
    banks.append(FmodServer.load_bank("res://Master.strings.bank", FmodBank.StudioLoadBankFlags.NORMAL))
    banks.append(FmodServer.load_bank("res://Music.bank", FmodBank.StudioLoadBankFlags.NORMAL))
```

!!! warning
As banks are `RefCounted`, don't forget to store them. Otherwise reference counter will be directly decremented leading
to unload of the bank.

!!! warning
Make sure to first load `Master.strings.bank`, and `Master.bank` in second. Those banks are dependencies needed by other
banks. So if you don't load them first, you won't be able to load other banks.

## Sample data

By default, FMOD loads the sample data of an event when one of its instances is created, and unloads it once the last
one is released, which can delay the first play. `load_sample_data()` on a bank loads the sample data of all its events
ahead of time (or on an `FmodEventDescription`, of one event), and `unload_sample_data()` lets it go:

```gdscript
var music_bank := FmodServer.load_bank("res://Music.bank", FmodBank.StudioLoadBankFlags.NORMAL)
music_bank.load_sample_data()
FmodServer.wait_for_all_loads()
# music_bank.get_sample_loading_state() is now FmodBank.StudioLoadingState.LOADED
```

Sample data loads in the background; `get_sample_loading_state()` tells when it is ready. Each `load_sample_data()` is
matched by one `unload_sample_data()`.

[fmod-bank-image]: ./assets/fmod-bank.png