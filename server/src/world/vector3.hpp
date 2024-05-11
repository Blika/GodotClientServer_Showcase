#pragma once

namespace godotserver{
    class Vector3{
        public:
            float x,y,z;

			Vector3();
			Vector3(float xx, float yy, float zz): x{xx}, y{yy}, z{zz}{}
			Vector3(float data[3]): x{data[0]}, y{data[1]}, z{data[2]}{}
			Vector3(int xx, int yy, int zz){
                x = static_cast<float>(xx);
                y = static_cast<float>(yy);
                z = static_cast<float>(zz);
            }
			~Vector3(){

            }
    };
}