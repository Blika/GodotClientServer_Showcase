#include "../register_types.h"
#include "player.h"
#include "../client/1client/client.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Player::_bind_methods(){
}

Player::Player(){
	if(!Engine::get_singleton()->is_editor_hint()){
		client = (Client*) Engine::get_singleton()->get_singleton("client");
		while(!client->serverFound){
			std::cout << "waiting for server" << '\n';
		}
		client->getPacketHandler()->setPlayer(this);
		Vector3 pos = get_position();
		Vector3 rot = get_rotation();
		String name = "test";
		name += client->rnd(1,1000);
		client->getPacketHandler()->handlePlayerJoin(pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, name.utf8());
	}
}

Player::~Player(){
}

void Player::_physics_process(double delta){
	if(Engine::get_singleton()->is_editor_hint()){
		return;
	}
	Vector3 velocity = get_velocity();
	Vector3 pos = get_position();
	Vector3 rot = get_rotation();
	if(!is_on_floor()){
		velocity.y -= gravity * delta;
	}
	set_velocity(velocity);
	move_and_slide();
	Vector3 p = get_position();
	Vector3 r = get_rotation();
	if(p != pos || r != rot){
		float dvx = p.x - pos.x;
		float dvy = p.y - pos.y;
		float dvz = p.z - pos.z;
		float drx = r.x - rot.x;
		float dry = r.y - rot.y;
		float drz = r.z - rot.z;
		client->getPacketHandler()->handlePlayerMove(dvx, dvy, dvz, drx, dry, drz);
	}
}

void Player::_input(const Ref<InputEvent> &event){
	if(Engine::get_singleton()->is_editor_hint()){
		return;
	}
	const InputEventKey *key_event = Object::cast_to<const InputEventKey>(*event);
	if(key_event){
		Vector3 velocity = get_velocity();
		//UtilityFunctions::print(key_event->get_keycode());
		if(key_event->get_keycode() == 32 && is_on_floor()){
			velocity.y = jump_velocity;
		}
		set_velocity(velocity);
	}
}