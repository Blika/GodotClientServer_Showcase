#pragma once

#include "vector3.hpp"
#include "rotation.hpp"

namespace godotserver{
    class Transform{
        public:
            Vector3* pos;
            Rotation* rot;
            //scale

			Transform();
			Transform(Vector3* v, Rotation* r): pos{v}, rot{r}{}
			Transform(float vx, float vy, float vz, float rx, float ry, float rz){
                pos = new Vector3(vx,vy,vz);
                rot = new Rotation(rx,ry,rz);
            }
			Transform(float data[6]){
                pos = new Vector3(data[0],data[1],data[2]);
                rot = new Rotation(data[3],data[4],data[5]);
            }
			~Transform(){
                delete pos;
                delete rot;
            }
    };
}