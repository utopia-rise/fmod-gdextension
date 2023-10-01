@tool class_name FmodParameterUi extends VBoxContainer

static var _step := 0.1

func _ready():
	%Slider.value_changed.connect(_on_slider_value_change)
	_on_slider_value_change(%Slider.value)
	
	%ValueSpinBox.value_changed.connect(_on_spin_box_value_change)
	
	%Slider.step = _step
	%ValueSpinBox.step = _step

func set_parameter_name(name: String):
	%NameLabel.text = name

func set_range(min_value: float, max_value: float):
	%Slider.min_value = min_value
	%Slider.max_value = max_value
	
	%ValueSpinBox.min_value = min_value
	%ValueSpinBox.max_value = max_value

func _on_slider_value_change(value: float):
	%ValueSpinBox.value = value

func _on_spin_box_value_change(value: float):
	%Slider.value = value
