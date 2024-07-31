class_name WelcomeOptionButton extends OptionButton


var current_index := -1

func _enter_tree():
	connect("item_selected", _on_item_selected)

func _on_item_selected(index: int):
	if index == -1:
		return
	if (current_index != -1):
		FmodServer.unload_bank("res://assets/Banks/%s" % ("Dialogue_%s.bank" % get_item_text(current_index)))
	current_index = index
	var bank_path := "res://assets/Banks/Dialogue_%s.bank" % get_item_text(index)
# warning-ignore:return_value_discarded
	FmodServer.load_bank(bank_path, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
