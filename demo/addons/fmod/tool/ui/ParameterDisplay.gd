@tool class_name ParameterDisplay extends MarginContainer

var event_description: FmodEventDescription
var parameter: FmodParameterDescription

func set_event_description(p_event_description: FmodEventDescription):
    event_description = p_event_description

func set_parameter(p_parameter: FmodParameterDescription):
    show()
    parameter = p_parameter

func display_value_selector(should: bool):
    %ValueSetterContainer.visible = should

func _ready():
    if parameter == null:
        hide()
        return
    var minimum_value = parameter.get_minimum()
    var maximum_value = parameter.get_maximum()
    var default_value = parameter.get_default_value()
    
    var copy_icon : Texture = EditorInterface.get_editor_theme().get_icon("ActionCopy", "EditorIcons")
    %NameCopyButton.icon = copy_icon
    %IdCopyButton.icon = copy_icon
    
    
    %NameLabel.text = parameter.get_name()
    %IdLabel.text = str(parameter.get_id())
    if parameter.is_labeled():
        %RangeTitle.text = "Values"
        var values_text = "["
        var is_first: bool = true
        for label: String in event_description.get_parameter_labels_by_id(parameter.get_id()):
            if not is_first:
                values_text += ", "
            values_text += label
            is_first = false
        values_text += "]"
        %RangeLabel.text = values_text
    else:
        %RangeLabel.text = "[%s, %s]" % [minimum_value, maximum_value]
    %DefaultValueLabel.text = str(default_value)
    %NameCopyButton.pressed.connect(_on_copy_name_button)
    %IdCopyButton.pressed.connect(_on_copy_id_button)
    %BackToDefaultButton.pressed.connect(_on_default_value_button)
    
    %ValueSlider.min_value = minimum_value
    %ValueSlider.max_value = maximum_value
    %ValueSlider.value = default_value
    
    _on_slider_value_changed(%ValueSlider.value)
    %ValueSlider.value_changed.connect(_on_slider_value_changed)

func _on_copy_name_button():
    DisplayServer.clipboard_set(%NameLabel.text)

func _on_copy_id_button():
    DisplayServer.clipboard_set(%IdLabel.text)

func _on_default_value_button():
    %ValueSlider.value = parameter.get_default_value()

func _on_slider_value_changed(value: float):
    %CurrentValueLabel.text = str(value)
