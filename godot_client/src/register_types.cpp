
#include "register_types.h"
#include "godot/player.hpp"
#include "godot/model.hpp"
#include "godot/entity_player.hpp"
#include "godot/main_menu_start.hpp"
#include "godot/loading_screen.hpp"
#include "client/1client/client.hpp"
#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <memory>

using namespace godot;

Client* client = nullptr;

void initialize_gdextension_types(ModuleInitializationLevel p_level){
	if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
		return;
	}

	if(!Engine::get_singleton()->is_editor_hint()){
		client = memnew(Client);
		Engine::get_singleton()->register_singleton("client", client);
	}
	ClassDB::register_class<MainMenuStart>();
	ClassDB::register_class<LoadingScreen>();
	ClassDB::register_class<Model>();
	ClassDB::register_class<Player>();
	ClassDB::register_class<EntityPlayer>();
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level){
	if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
		return;
	}
	client->shutdown();
}

extern "C"{
	// Initialization
	GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization){
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}