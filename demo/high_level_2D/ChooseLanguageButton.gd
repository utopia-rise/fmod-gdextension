class_name ChooseLanguageButton extends OptionButton


@export var root_node: Node
var current_bank_loader: FmodBankLoader = null

func _enter_tree():
	connect("item_selected", _on_item_selected)

func _on_item_selected(index: int):
	if index == -1:
		return
	if (current_bank_loader != null):
		root_node.remove_child(current_bank_loader)
	var bank_path := "res://assets/Banks/Dialogue_%s.bank" % get_item_text(index)
	current_bank_loader = FmodBankLoader.new()
	current_bank_loader.bank_paths = [bank_path]
	root_node.add_child(current_bank_loader)
