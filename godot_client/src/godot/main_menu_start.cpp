#include "main_menu_start.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

void MainMenuStart::_bind_methods(){
}

MainMenuStart::MainMenuStart(){
}

MainMenuStart::~MainMenuStart(){
}

void MainMenuStart::_pressed(){
	if(!Engine::get_singleton()->is_editor_hint()){
        get_tree()->change_scene_to_file("res://scenes/loading_screen.tscn");
	}
}
