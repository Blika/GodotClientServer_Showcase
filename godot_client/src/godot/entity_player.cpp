#include "entity_player.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void EntityPlayer::_bind_methods(){
}

EntityPlayer::EntityPlayer(){
    pot.zero();
    rot.zero();
}

EntityPlayer::~EntityPlayer(){
}

AnimationPlayer* EntityPlayer::getAnimationPlayer(){
	return get_node<AnimationPlayer>("Model/mixamo_base/AnimationPlayer");
}

void EntityPlayer::setRunning(bool v){
    running = v;
}

void EntityPlayer::playKickAnimation(){
	if(getAnimationPlayer()->get_current_animation() != "kick"){
		getAnimationPlayer()->play("kick");
	}
}

void EntityPlayer::playKnockDownAnimation(){
	if(getAnimationPlayer()->get_current_animation() != "knock_down"){
		getAnimationPlayer()->play("knock_down");
        crowd_controlled = true;

	}
}

void EntityPlayer::playGetUpAnimation(){
	if(getAnimationPlayer()->get_current_animation() != "get_up"){
		getAnimationPlayer()->play("get_up");
        crowd_controlled = false;
	}
}

void EntityPlayer::playMovingAnimation(){
    String anim = getAnimationPlayer()->get_current_animation();
    if(anim == "kick" || anim == "get_up" || anim == "knock_down" || crowd_controlled) return;
	if(!running){
		if(anim != "walking"){
			getAnimationPlayer()->play("walking");
		}
	}else{
		if(anim != "running"){
			getAnimationPlayer()->play("running");
		}
	}
}

void EntityPlayer::playIdleAnimation(){
    String anim = getAnimationPlayer()->get_current_animation();
    if(anim == "kick" || anim == "get_up" || anim == "knock_down" || crowd_controlled) return;
	if(anim != "idle"){
		getAnimationPlayer()->play("idle");
	}
}

void EntityPlayer::addPosDestination(Vector3 p){
    pot = p;
    hasPot = true;
}

void EntityPlayer::addRotDestination(Vector3 p){
    rot = p;
    hasRot = true;
}

void EntityPlayer::_physics_process(double delta){
	if(Engine::get_singleton()->is_editor_hint()){
		return;
    }
    if(hasPot){
        double t = delta * 7.f;
        Vector3 p = get_position();
        if(p.distance_to(pot) <= 0.1f){
            set_position(pot);
            hasPot = false;
            pot.zero();
            playIdleAnimation();
        }else{
            Vector3 n = p.lerp(pot, t);
            set_position(n);
            playMovingAnimation();
        }
    }else{
        playIdleAnimation();
    }
    if(hasRot){
        double t = delta * 7.f;
        Vector3 r = get_rotation();
        Vector3 n = r.lerp(rot, t);
        if(n.is_equal_approx(rot)){
            set_rotation(rot);
            hasRot = false;
            rot.zero();
        }else{
            set_rotation(n);
        }
    }
}