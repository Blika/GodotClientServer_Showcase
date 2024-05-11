#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/project_settings.hpp>

namespace godot {
	class Client;

	class Player : public CharacterBody3D {
		GDCLASS(Player, CharacterBody3D)

	private:
		Client* client;

		double speed = 5.f;
		double jump_velocity = 4.5f;
		double gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

	protected:
		static void _bind_methods();

	public:
		Player();
		~Player();

		void _physics_process(double delta) override;
		void _input(const Ref<InputEvent> &event) override;
	};

}