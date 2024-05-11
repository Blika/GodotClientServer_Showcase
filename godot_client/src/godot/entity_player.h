#pragma once

#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

	class EntityPlayer : public CharacterBody3D {
		GDCLASS(EntityPlayer, CharacterBody3D)

	private:

	protected:
		static void _bind_methods();

	public:
		EntityPlayer();
		~EntityPlayer();
	};

}