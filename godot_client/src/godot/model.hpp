#pragma once

#include <godot_cpp/classes/node3d.hpp>

namespace godot {
    class Client;

	class Model : public Node3D {
		GDCLASS(Model, Node3D)

	private:
		Client* client;

	protected:
		static void _bind_methods();

	public:
		Model();
		~Model();
        
		Vector3 getDirectionVector3();
		Vector2 getDirectionVector2();
		void rotate_x(double angle);
		void rotate_y(double angle);
		void rotate_z(double angle);
	};

}