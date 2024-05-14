#pragma once

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/project_settings.hpp>

namespace godot {
	class Client;
	class Model;

	class Player : public CharacterBody3D {
		GDCLASS(Player, CharacterBody3D)

	private:
		bool mouse_captured = false;
		bool ready;
		Client* client;
		bool locked = false;
		bool crowd_controlled = false;
		bool getting_up = false;
		bool running = false;
		double default_speed = 3.f;
		double rotation_speed = 3.f;
		double speed = 3.f;
		double running_multiplier = 1.65f;
		double jump_velocity = 4.5f;
		double sens_x = 0.2f;
		double sens_y = 0.2f;
		double gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

	protected:
		static void _bind_methods();

	public:
		Player();
		~Player();

		Node3D* getCamera();
		AnimationPlayer* getAnimationPlayer();
		Model* getModel();
		Vector3 getCameraDirectionVector3();
		Vector2 getCameraDirectionVector2();
		void _ready() override;
		bool isCCd();
		void playKickAnimation();
		void playKnockDownAnimation();
		void playGetUpAnimation();
		void playMovingAnimation();
		void playIdleAnimation();
		void setRotationSpeed(double s);
		double getRotationSpeed();
		void setRunningMultiplier(double s);
		double getRunningMultiplier();
		void setDefaultSpeed(double s);
		double getDefaultSpeed();
		void setJumpVelocity(double s);
		double getJumpVelocity();
		void setVerticalSensivity(double s);
		void setHorizontalSensivity(double s);
		double getVerticalSensivity();
		double getHorizontalSensivity();
		void _physics_process(double delta) override;
		void _input(const Ref<InputEvent> &event) override;
	};

}