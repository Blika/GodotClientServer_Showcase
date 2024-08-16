
#include "model.hpp"
#include "../client/1client/client.hpp"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Model::_bind_methods(){
}

Model::Model(){
	if(!Engine::get_singleton()->is_editor_hint()){
		client = (Client*) Engine::get_singleton()->get_singleton("client");
	}
}

Model::~Model(){
}

Vector3 Model::getDirectionVector3(){
	Vector3 rot = get_rotation();
	double y = -Math::sin(rot.x);
	double xz = Math::cos(rot.x);
	double x = -xz * Math::sin(rot.y);
	double z = xz * Math::cos(rot.y);
	Vector3 direction = Vector3(x,y,z);
	direction.normalize();
	return direction;
}

Vector2 Model::getDirectionVector2(){
	Vector3 rot = get_rotation();
	double x = -Math::sin(rot.y);
	double z = Math::cos(rot.y);
	Vector2 direction = Vector2(x,z);
	direction.normalize();
	return direction;
}

void Model::rotate_x(double angle){
	Node3D::rotate_x(angle);
	if(angle != 0.f && client != nullptr){
		Vector3 rot = get_rotation();
		float rx = rot.x;
		float ry = rot.y;
		float rz = rot.z;
		client->getPacketHandler()->handlePlayerRotate(rx, ry, rz);
	}
}

void Model::rotate_y(double angle){
	Node3D::rotate_y(angle);
	if(angle != 0.f && client != nullptr){
		Vector3 rot = get_rotation();
		float rx = rot.x;
		float ry = rot.y;
		float rz = rot.z;
		client->getPacketHandler()->handlePlayerRotate(rx, ry, rz);
	}
}

void Model::rotate_z(double angle){
	Node3D::rotate_z(angle);
	if(angle != 0.f && client != nullptr){
		Vector3 rot = get_rotation();
		float rx = rot.x;
		float ry = rot.y;
		float rz = rot.z;
		client->getPacketHandler()->handlePlayerRotate(rx, ry, rz);
	}
}
