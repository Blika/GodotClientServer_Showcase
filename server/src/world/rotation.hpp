#pragma once

namespace godotserver{
    class Rotation{
        public:
            float x,y,z;

			Rotation();
			Rotation(float xx, float yy, float zz): x{xx}, y{yy}, z{zz}{}
			Rotation(float data[3]): x{data[0]}, y{data[1]}, z{data[2]}{}
			Rotation(int xx, int yy, int zz){
                x = static_cast<float>(xx);
                y = static_cast<float>(yy);
                z = static_cast<float>(zz);
            }
			~Rotation(){

            }
    };
}