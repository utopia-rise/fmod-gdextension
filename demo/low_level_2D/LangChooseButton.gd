class_name WelcomeOptionButton extends OptionButton


var lang_bank: FmodBank

func _enter_tree():
	connect("item_selected", _on_item_selected)

func _on_item_selected(index: int):
	if index == -1:
		lang_bank = null
		return
	var bank_path := "res://assets/Banks/Dialogue_%s.bank" % get_item_text(index)
# warning-ignore:return_value_discarded
	lang_bank = FmodServer.load_bank(bank_path, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
