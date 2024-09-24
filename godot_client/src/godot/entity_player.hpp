#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>

namespace godot {

	class EntityPlayer : public CharacterBody3D {
		GDCLASS(EntityPlayer, CharacterBody3D)

	private:
		bool running = false;
		Vector3 rot;
		Vector3 pot;
		bool hasPot = false;
		bool hasRot = false;
		bool crowd_controlled = false;

	protected:
		static void _bind_methods();

	public:
		EntityPlayer();
		~EntityPlayer();

		AnimationPlayer* getAnimationPlayer();
		void setRunning(bool v);
		void playKickAnimation();
		void playKnockDownAnimation();
		void playGetUpAnimation();
		void playMovingAnimation();
		void playIdleAnimation();
		void addPosDestination(Vector3& d);
		void addRotDestination(Vector3& d);
		void _physics_process(double delta) override;
	};

}