extends Sprite3D

var coins = 5
var playername = "godot"
var hp = 3.5
const SPEED = 2

# Called when the node enters the scene tree for the first time.
func _ready():
	print("yaica")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	rotate_y(0.1)
