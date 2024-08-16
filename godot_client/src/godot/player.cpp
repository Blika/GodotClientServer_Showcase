#include "../register_types.h"
#include "player.hpp"
#include "model.hpp"
#include "../client/1client/client.hpp"
#include "../client/network/packets/player_actions.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Player::_bind_methods(){
	ClassDB::bind_method(D_METHOD("getRotationSpeed"), &Player::getRotationSpeed);
	ClassDB::bind_method(D_METHOD("setRotationSpeed", "s"), &Player::setRotationSpeed);
	ClassDB::bind_method(D_METHOD("getRunningMultiplier"), &Player::getRunningMultiplier);
	ClassDB::bind_method(D_METHOD("setRunningMultiplier", "s"), &Player::setRunningMultiplier);
	ClassDB::bind_method(D_METHOD("getDefaultSpeed"), &Player::getDefaultSpeed);
	ClassDB::bind_method(D_METHOD("setDefaultSpeed", "s"), &Player::setDefaultSpeed);
	ClassDB::bind_method(D_METHOD("getJumpVelocity"), &Player::getJumpVelocity);
	ClassDB::bind_method(D_METHOD("setJumpVelocity", "s"), &Player::setJumpVelocity);
	ClassDB::bind_method(D_METHOD("getVerticalSensivity"), &Player::getVerticalSensivity);
	ClassDB::bind_method(D_METHOD("setVerticalSensivity", "s"), &Player::setVerticalSensivity);
	ClassDB::bind_method(D_METHOD("getHorizontalSensivity"), &Player::getHorizontalSensivity);
	ClassDB::bind_method(D_METHOD("setHorizontalSensivity", "s"), &Player::setHorizontalSensivity);
	ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "rotation_speed"), "setRotationSpeed", "getRotationSpeed");
	ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "running_multiplier"), "setRunningMultiplier", "getRunningMultiplier");
	ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "default_speed"), "setDefaultSpeed", "getDefaultSpeed");
	ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "jump_velocity"), "setJumpVelocity", "getJumpVelocity");
	ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "sens_x", PROPERTY_HINT_RANGE, "0.1,1.0,0.01"), "setVerticalSensivity", "getVerticalSensivity");
	ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "sens_y", PROPERTY_HINT_RANGE, "0.1,1.0,0.01"), "setHorizontalSensivity", "getHorizontalSensivity");
}

Player::Player(){
	if(!Engine::get_singleton()->is_editor_hint()){
		client = (Client*) Engine::get_singleton()->get_singleton("client");
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

Node3D* Player::getCamera(){
	return get_node<Node3D>("Camera");
}

AnimationPlayer* Player::getAnimationPlayer(){
	return get_node<AnimationPlayer>("Model/mixamo_base/AnimationPlayer");
}

Model* Player::getModel(){
	return get_node<Model>("Model");
}

Vector3 Player::getCameraDirectionVector3(){
	Vector3 rot = getCamera()->get_rotation();
	double y = -Math::sin(Math::deg_to_rad(rot.x));
	double xz = Math::cos(Math::deg_to_rad(rot.x));
	double x = -xz * Math::sin(rot.y);
	double z = xz * Math::cos(rot.y);
	Vector3 direction = Vector3(x,y,z);
	direction.normalize();
	return direction;
}

Vector2 Player::getCameraDirectionVector2(){
	Vector3 rot =  getCamera()->get_rotation();
	double x = -Math::sin(rot.y);
	double z = Math::cos(rot.y);
	Vector2 direction = Vector2(x,z);
	direction.normalize();
	return direction;
}

void Player::_ready(){
	if(Engine::get_singleton()->is_editor_hint()){
		return;
	}
	Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
	mouse_captured = true;
}

bool Player::isCCd(){
	return locked || crowd_controlled;
}

void Player::playKickAnimation(){
	if(getAnimationPlayer()->get_current_animation() != "kick"){
		getAnimationPlayer()->play("kick");
		locked = true;
		client->getPacketHandler()->handlePlayerAction(ACTION_KICK);
	}
}

void Player::playKnockDownAnimation(){
	if(getAnimationPlayer()->get_current_animation() != "knock_down"){
		getAnimationPlayer()->play("knock_down");
		crowd_controlled = true;
		//client->getPacketHandler()->handlePlayerAction(ACTION_KNOCK_DOWN);
	}
}

void Player::playGetUpAnimation(){
	if(getAnimationPlayer()->get_current_animation() != "get_up"){
		getAnimationPlayer()->play("get_up");
		getting_up = true;
		//client->getPacketHandler()->handlePlayerAction(ACTION_GET_UP);
	}
}

void Player::playMovingAnimation(){
	if(!running){
		if(getAnimationPlayer()->get_current_animation() != "walking"){
			getAnimationPlayer()->play("walking");
		}
	}else{
		if(getAnimationPlayer()->get_current_animation() != "running"){
			getAnimationPlayer()->play("running");
		}
	}
}

void Player::playIdleAnimation(){
	if(getAnimationPlayer()->get_current_animation() != "idle"){
		getAnimationPlayer()->play("idle");
	}
}

void Player::setRotationSpeed(double s){
	rotation_speed = s;
}
double Player::getRotationSpeed(){
	return rotation_speed;
}

void Player::setRunningMultiplier(double s){
	running_multiplier = s;
}
double Player::getRunningMultiplier(){
	return running_multiplier;
}

void Player::setDefaultSpeed(double s){
	default_speed = s;
	speed = default_speed;
}
double Player::getDefaultSpeed(){
	return default_speed;
}
void Player::setJumpVelocity(double s){
	jump_velocity = s;
}
double Player::getJumpVelocity(){
	return jump_velocity;
}
void Player::setVerticalSensivity(double s){
	sens_x = s;
}
void Player::setHorizontalSensivity(double s){
	sens_y = s;
}
double Player::getVerticalSensivity(){
	return sens_x;
}
double Player::getHorizontalSensivity(){
	return sens_y;
}

void Player::_process(double delta){
	if(Engine::get_singleton()->is_editor_hint()){
		return;
	}
	client->run();
}

void Player::_physics_process(double delta){
	if(Engine::get_singleton()->is_editor_hint()){
		return;
	}
	if(client == nullptr){
		return;
	}else if(!client->getPacketHandler()->isSpawned()){
		return;
	}
	if(Input::get_singleton()->is_action_just_pressed("ctrl")){
		if(!mouse_captured){
			Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
			mouse_captured = true;
		}else{
			Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
			mouse_captured = false;
		}
	}

	if(locked && getAnimationPlayer()->get_current_animation() != "kick"){
		locked = false;
	}
	if(getting_up && getAnimationPlayer()->get_current_animation() != "get_up"){
		getting_up = false;
		crowd_controlled = false;
	}
	if(!isCCd() && Input::get_singleton()->is_action_just_pressed("kick")){
		playKickAnimation();
	}
	if(Input::get_singleton()->is_action_pressed("run") && is_on_floor()){
		if(!running){
			running = true;
			speed = default_speed * running_multiplier;
			client->getPacketHandler()->handlePlayerAction(ACTION_START_SPRINT);
		}
	}else if(running){
		running = false;
		speed = default_speed;
		client->getPacketHandler()->handlePlayerAction(ACTION_STOP_SPRINT);
	}
	Vector3 velocity = get_velocity();
	if(!is_on_floor()){
		velocity.y -= gravity * delta;
	}
	if(!isCCd()){
		if(Input::get_singleton()->is_action_just_pressed("ui_accept") && is_on_floor()){
			velocity.y = jump_velocity;
		}

		if(Input::get_singleton()->is_action_pressed("ui_w")){
			playMovingAnimation();

			if(Input::get_singleton()->is_action_pressed("ui_a") && !Input::get_singleton()->is_action_pressed("ui_d")){
				double ry = getCamera()->get_rotation_degrees().y;
				ry += 45.f;
				while(ry >= 180.f){
					ry -= 360.f;
				}
				while(ry <= -180.f){
					ry += 360.f;
				}
				double sry = getModel()->get_rotation_degrees().y;
				if(sry != ry){
					double diff = ry - sry;
					while(diff >= 180.f){
						diff -= 360.f;
					}
					while(diff <= -180.f){
						diff += 360.f;
					}
					if(diff > rotation_speed){
						diff = rotation_speed;
					}else if(diff < -rotation_speed){
						diff = -rotation_speed;
					}
					getModel()->rotate_y(Math::deg_to_rad(diff));
				}
				//getModel()->rotate_y(Math::deg_to_rad(rotation_speed));
			}else if(Input::get_singleton()->is_action_pressed("ui_d") && !Input::get_singleton()->is_action_pressed("ui_a")){
				double ry = getCamera()->get_rotation_degrees().y;
				ry -= 45.f;
				while(ry >= 180.f){
					ry -= 360.f;
				}
				while(ry <= -180.f){
					ry += 360.f;
				}
				double sry = getModel()->get_rotation_degrees().y;
				if(sry != ry){
					double diff = ry - sry;
					while(diff >= 180.f){
						diff -= 360.f;
					}
					while(diff <= -180.f){
						diff += 360.f;
					}
					if(diff > rotation_speed){
						diff = rotation_speed;
					}else if(diff < -rotation_speed){
						diff = -rotation_speed;
					}
					getModel()->rotate_y(Math::deg_to_rad(diff));
				}
				//getModel()->rotate_y(Math::deg_to_rad(-rotation_speed));
			}else{
				double ry = getCamera()->get_rotation_degrees().y;
				double sry = getModel()->get_rotation_degrees().y;
				if(sry != ry){
					double diff = ry - sry;
					while(diff >= 180.f){
						diff -= 360.f;
					}
					while(diff <= -180.f){
						diff += 360.f;
					}
					if(diff > rotation_speed){
						diff = rotation_speed;
					}else if(diff < -rotation_speed){
						diff = -rotation_speed;
					}
					getModel()->rotate_y(Math::deg_to_rad(diff));
				}
			}
			Vector2 direction = getModel()->getDirectionVector2();
			velocity.x = direction.x * speed;
			velocity.z = -direction.y * speed;
		}else if(Input::get_singleton()->is_action_pressed("ui_s")){
			playMovingAnimation();
			if(Input::get_singleton()->is_action_pressed("ui_a") && !Input::get_singleton()->is_action_pressed("ui_d")){
				double ry = getCamera()->get_rotation_degrees().y;
				ry += 180.f;
				ry -= 45.f;
				while(ry >= 180.f){
					ry -= 360.f;
				}
				while(ry <= -180.f){
					ry += 360.f;
				}
				double sry = getModel()->get_rotation_degrees().y;
				if(sry != ry){
					double diff = ry - sry;
					while(diff >= 180.f){
						diff -= 360.f;
					}
					while(diff <= -180.f){
						diff += 360.f;
					}
					if(diff > rotation_speed){
						diff = rotation_speed;
					}else if(diff < -rotation_speed){
						diff = -rotation_speed;
					}
					getModel()->rotate_y(Math::deg_to_rad(diff));
				}
				//getModel()->rotate_y(Math::deg_to_rad(-rotation_speed));
			}else if(Input::get_singleton()->is_action_pressed("ui_d") && !Input::get_singleton()->is_action_pressed("ui_a")){
				double ry = getCamera()->get_rotation_degrees().y;
				ry += 180.f;
				ry += 45.f;
				while(ry >= 180.f){
					ry -= 360.f;
				}
				while(ry <= -180.f){
					ry += 360.f;
				}
				double sry = getModel()->get_rotation_degrees().y;
				if(sry != ry){
					double diff = ry - sry;
					while(diff >= 180.f){
						diff -= 360.f;
					}
					while(diff <= -180.f){
						diff += 360.f;
					}
					if(diff > rotation_speed){
						diff = rotation_speed;
					}else if(diff < -rotation_speed){
						diff = -rotation_speed;
					}
					getModel()->rotate_y(Math::deg_to_rad(diff));
				}
				//getModel()->rotate_y(Math::deg_to_rad(rotation_speed));
			}else{
				double ry = getCamera()->get_rotation_degrees().y;
				ry += 180.f;
				while(ry >= 180.f){
					ry -= 360.f;
				}
				while(ry <= -180.f){
					ry += 360.f;
				}
				double sry = getModel()->get_rotation_degrees().y;
				if(sry != ry){
					double diff = ry - sry;
					while(diff >= 180.f){
						diff -= 360.f;
					}
					while(diff <= -180.f){
						diff += 360.f;
					}
					if(diff > rotation_speed){
						diff = rotation_speed;
					}else if(diff < -rotation_speed){
						diff = -rotation_speed;
					}
					getModel()->rotate_y(Math::deg_to_rad(diff));
				}
			}
			Vector2 direction = getModel()->getDirectionVector2();
			velocity.x = direction.x * speed;
			velocity.z = -direction.y * speed;
		}else if(Input::get_singleton()->is_action_pressed("ui_a") && !Input::get_singleton()->is_action_pressed("ui_d")){
			playMovingAnimation();

			double ry = getCamera()->get_rotation_degrees().y;
			ry += 90.f;
			while(ry >= 180.f){
				ry -= 360.f;
			}
			while(ry <= -180.f){
				ry += 360.f;
			}
			double sry = getModel()->get_rotation_degrees().y;
			if(sry != ry){
				double diff = ry - sry;
				while(diff >= 180.f){
					diff -= 360.f;
				}
				while(diff <= -180.f){
					diff += 360.f;
				}
				if(diff > rotation_speed){
					diff = rotation_speed;
				}else if(diff < -rotation_speed){
					diff = -rotation_speed;
				}
				getModel()->rotate_y(Math::deg_to_rad(diff));
			}
			Vector2 direction = getModel()->getDirectionVector2();
			velocity.x = direction.x * speed;
			velocity.z = -direction.y * speed;
		}else if(Input::get_singleton()->is_action_pressed("ui_d") && !Input::get_singleton()->is_action_pressed("ui_a")){
			playMovingAnimation();

			double ry = getCamera()->get_rotation_degrees().y;
			ry -= 90.f;
			while(ry >= 180.f){
				ry -= 360.f;
			}
			while(ry <= -180.f){
				ry += 360.f;
			}
			double sry = getModel()->get_rotation_degrees().y;
			if(sry != ry){
				double diff = ry - sry;
				while(diff >= 180.f){
					diff -= 360.f;
				}
				while(diff <= -180.f){
					diff += 360.f;
				}
				if(diff > rotation_speed){
					diff = rotation_speed;
				}else if(diff < -rotation_speed){
					diff = -rotation_speed;
				}
				getModel()->rotate_y(Math::deg_to_rad(diff));
			}
			Vector2 direction = getModel()->getDirectionVector2();
			velocity.x = direction.x * speed;
			velocity.z = -direction.y * speed;
		}else{
			playIdleAnimation();
			/*velocity.x = Math::move_toward((double)velocity.x,(double)0.f,speed);
			velocity.z = Math::move_toward((double)velocity.z,(double)0.f,speed);*/
			velocity.x = 0.f;
			velocity.z = 0.f;
		}
	}else{
		velocity.x = 0.f;
		velocity.z = 0.f;
	}
	Vector3 d1 = get_position();
	set_velocity(velocity);
	move_and_slide();
	Vector3 d2 = get_position();
	float dvx = d2.x - d1.x;
	float dvy = d2.y - d1.y;
	float dvz = d2.z - d1.z;
	client->getPacketHandler()->handlePlayerMove(dvx, dvy, dvz);
}

void Player::_input(const Ref<InputEvent> &event){
	if(Engine::get_singleton()->is_editor_hint()){
		return;
	}

	if(mouse_captured){
		const InputEventMouseMotion *mouse_event = Object::cast_to<const InputEventMouseMotion>(*event);
		if(mouse_event){
			Vector3 rot = getCamera()->get_rotation_degrees();
			rot.y += -mouse_event->get_relative().x*sens_y;
			rot.x += -mouse_event->get_relative().y*sens_x;
			if(rot.x > 90.f){
				rot.x = 90.f;
			}else if(rot.x < -80.f){
				rot.x = -80.f;
			}
			getCamera()->set_rotation_degrees(rot);
		}
	}
}