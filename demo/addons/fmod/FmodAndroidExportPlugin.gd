@tool
class_name FmodAndroidExportPlugin extends EditorExportPlugin

var plugin_name: String = "fmod"

func _supports_platform(platform):
	if platform is EditorExportPlatformAndroid:
		return true
	return false

func _get_android_libraries(platform, debug):
	if debug:
		return PackedStringArray([plugin_name + "/libs/android/" + plugin_name + "-debug.aar"])
	else:
		return PackedStringArray([plugin_name + "/libs/android/" + plugin_name + "-release.aar"])


func _get_name():
	return plugin_name
