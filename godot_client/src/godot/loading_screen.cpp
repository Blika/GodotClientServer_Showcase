#include "loading_screen.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include "../client/1client/client.hpp"

using namespace godot;

void LoadingScreen::_bind_methods(){
}

LoadingScreen::LoadingScreen(){
}

LoadingScreen::~LoadingScreen(){
}

void LoadingScreen::_process(double delta){
	if(!Engine::get_singleton()->is_editor_hint()){
        Label* text = get_node<Label>("Label");
        text->set_text("Поиск сервера...");
		Client* client = (Client*) Engine::get_singleton()->get_singleton("client");
        client->startup();
        text->set_text("Подключение к серверу...");
		while(!client->serverFound){
		}
        text->set_text("Загрузка мира...");
        get_tree()->change_scene_to_file("res://scenes/main.tscn");
	}

}
