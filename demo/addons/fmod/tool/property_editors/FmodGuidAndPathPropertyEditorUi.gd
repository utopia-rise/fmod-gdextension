@tool class_name FmodGuidAndPathPropertyEditorUi extends HBoxContainer


func set_callables(window_callable: Callable, path_callable: Callable, guid_callable: Callable):
    %EventExplorerButton.pressed.connect(window_callable)
    %PathLineEdit.text_changed.connect(path_callable)
    %GuidLineEdit.text_changed.connect(guid_callable)

func set_icon(icon: Texture2D):
    %EventExplorerButton.icon = icon
