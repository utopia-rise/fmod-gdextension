extends FmodEventEmitter2D

var isPlaying: bool = true

func _process(_delta):
    if Input.is_action_just_pressed("space"):
        isPlaying = !isPlaying
        if(isPlaying):
            print("Mower playing")
            paused = false
        else:
            print("Mower paused")
            paused = true
    elif Input.is_action_just_pressed("kill_event"):
        self.queue_free()
    if Input.is_action_pressed("engine_power_up"):
        self["event_parameter/RPM/value"] = self["event_parameter/RPM/value"] + 10
    if Input.is_action_pressed("engine_power_down"):
        self["event_parameter/RPM/value"] = self["event_parameter/RPM/value"] - 10
