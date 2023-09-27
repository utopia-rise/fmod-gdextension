@tool class_name ParameterDisplay extends HBoxContainer

var parameter: FmodParameterDescription

func set_parameter(p_parameter: FmodParameterDescription):
	parameter = p_parameter

func _ready():
	%NameLabel.text = parameter.get_name()
	%IdLabel.text = str(parameter.get_id())
	%RangeLabel.text = "[%s, %s]" % [parameter.get_minimum(), parameter.get_maximum()]
	%DefaultValueLabel.text = str(parameter.get_default_value())
	%NameCopyButton.pressed.connect(_on_copy_name_button)
	%IdCopyButton.pressed.connect(_on_copy_id_button)

func _on_copy_name_button():
	DisplayServer.clipboard_set(%NameLabel.text)

func _on_copy_id_button():
	DisplayServer.clipboard_set(%IdLabel.text)
