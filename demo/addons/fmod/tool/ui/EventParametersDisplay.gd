@tool class_name EventParametersDisplay extends ScrollContainer

static var parameter_display_scene: PackedScene = load("res://addons/fmod/tool/ui/ParameterDisplay.tscn")

func set_fmod_event(event: FmodEventDescription) -> bool: # returns false if there were no parameters
    for child in %ParameterDisplaysContainer.get_children():
        %ParameterDisplaysContainer.remove_child(child)
        child.queue_free()
    
    var event_parameters: Array = event.get_parameters()
    if event_parameters:
        show()
        for parameter : FmodParameterDescription in event_parameters:
            var parameter_display: ParameterDisplay = parameter_display_scene.instantiate()
            parameter_display.set_event_description(event)
            parameter_display.set_parameter(parameter)
            if %ParameterDisplaysContainer.get_child_count() > 0:
                %ParameterDisplaysContainer.add_child(HSeparator.new())
            %ParameterDisplaysContainer.add_child(parameter_display)
        return true # we had parameters to show!
    else:
        return false # no parameters to visualise
