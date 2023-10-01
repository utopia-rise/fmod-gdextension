@tool class_name ParameterDisplay extends VBoxContainer

var parameter: FmodParameterDescription

func set_parameter(p_parameter: FmodParameterDescription):
	parameter = p_parameter

func display_value_selector(should: bool):
	%ValueSetterContainer.visible = should

func _ready():
	if parameter == null:
		return
	var minimum_value = parameter.get_minimum()
	var maximum_value = parameter.get_maximum()
	var default_value = parameter.get_default_value()
	
	%NameLabel.text = parameter.get_name()
	%IdLabel.text = str(parameter.get_id())
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
