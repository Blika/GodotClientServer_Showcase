
#include "register_types.h"
#include "godot/player.h"
#include "godot/entity_player.h"
#include "client/1client/client.hpp"
#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <memory>

using namespace godot;

Client* client = nullptr;
ThreadPool* threadpool = nullptr;

void runClient(){
	client->run();
}

void initThreadpool(){
	threadpool = memnew(ThreadPool);
	threadpool->setThreadCount(std::thread::hardware_concurrency());
	std::cout << "Set thread count to " << std::thread::hardware_concurrency() << '\n';
	threadpool->clientThread = threadpool->getLeastBusyThread();
	if(threadpool->clientThread == -1) threadpool->clientThread = 0;
	threadpool->threads[threadpool->clientThread]->addJob([=] {runClient();});
}

void shutdownThreadpool(){
	threadpool->wait();
}

void initialize_gdextension_types(ModuleInitializationLevel p_level){
	if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
		return;
	}

	if(!Engine::get_singleton()->is_editor_hint()){
		client = memnew(Client);
		initThreadpool();
		Engine::get_singleton()->register_singleton("client", client);
		Engine::get_singleton()->register_singleton("threadpool", threadpool);
	}
	ClassDB::register_class<Player>();
	ClassDB::register_class<EntityPlayer>();
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level){
	if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
		return;
	}
	client->shutdown();
	shutdownThreadpool();
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