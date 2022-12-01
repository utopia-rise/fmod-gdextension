extends Panel

onready var dsp_usage = get_node("%DSPUsage")
onready var mem_usage = get_node("%MemUsage")
onready var rms_val = get_node("%RMSValue")

func _process(delta):
	if Fmod.started:
		dsp_usage.text = "dsp = " + "%.1f" % Fmod.get_performance_data()['CPU']['dsp'] + "%, studio = " + "%.1f" % Fmod.get_performance_data()['CPU']['studio'] + "%"
		mem_usage.text = "cur = " + "%.f" % (Fmod.get_performance_data()['memory']['currently_allocated'] / 1024 / 1024) + "MB, max = " + "%.f" % (Fmod.get_performance_data()['memory']['max_allocated'] / 1024 / 1024) + "MB"
	else:
		dsp_usage.text = "FMOD Not Started"
		mem_usage.text = "FMOD Not Started"

func set_visibility(vis: bool):
	set_process(vis)
	visible = vis
