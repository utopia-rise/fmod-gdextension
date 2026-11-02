extends SceneTree
class_name ExportTestMain
## Entry point of exported test builds: the tests_* export presets carry the export-tests feature, which makes this
## the project's main loop (application/run/main_loop_type.export-tests) and ExportTestScene, an empty node, its main
## scene: a custom main loop still loads the main scene, and the demo's would play events of its own during the tests.
## It boots ExportTestRunner the way GdUnitCmdTool.gd boots gdUnit4's editor runner.

const ExportTestRunner := preload("res://test_runner/ExportTestRunner.gd")


func _initialize() -> void:
	root.add_child(ExportTestRunner.new())
