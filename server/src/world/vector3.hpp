#pragma once

#include "../utils/math.hpp"

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

            Vector3 cross(const Vector3 &p_with) const{
                Vector3 ret(
                        (y * p_with.z) - (z * p_with.y),
                        (z * p_with.x) - (x * p_with.z),
                        (x * p_with.y) - (y * p_with.x));

                return ret;
            }

            float dot(const Vector3 &p_with) const{
                return x * p_with.x + y * p_with.y + z * p_with.z;
            }

            Vector3 floor() const{
                return Vector3(Math::floor(x), Math::floor(y), Math::floor(z));
            }

            Vector3 ceil() const{
                return Vector3(Math::ceil(x), Math::ceil(y), Math::ceil(z));
            }

            Vector3 round() const{
                return Vector3(Math::round(x), Math::round(y), Math::round(z));
            }

            float distance_to(const Vector3 &p_to) const{
                return (p_to - *this).length();
            }

            float distance_squared_to(const Vector3 &p_to) const{
                return (p_to - *this).length_squared();
            }

            Vector3 project(const Vector3 &p_to) const{
                return p_to * (dot(p_to) / p_to.length_squared());
            }

            float angle_to(const Vector3 &p_to) const{
                return atan2(cross(p_to).length(), dot(p_to));
            }

            float signed_angle_to(const Vector3 &p_to, const Vector3 &p_axis) const{
                Vector3 cross_to = cross(p_to);
                float unsigned_angle = atan2(cross_to.length(), dot(p_to));
                float sign = cross_to.dot(p_axis);
                return (sign < 0) ? -unsigned_angle : unsigned_angle;
            }

            Vector3 direction_to(const Vector3 &p_to) const{
                Vector3 ret(p_to.x - x, p_to.y - y, p_to.z - z);
                ret.normalize();
                return ret;
            }

            Vector3& operator+=(const Vector3 &p_v){
                x += p_v.x;
                y += p_v.y;
                z += p_v.z;
                return *this;
            }

            Vector3 operator+(const Vector3 &p_v) const{
                return Vector3(x + p_v.x, y + p_v.y, z + p_v.z);
            }

            Vector3& operator-=(const Vector3 &p_v){
                x -= p_v.x;
                y -= p_v.y;
                z -= p_v.z;
                return *this;
            }

            Vector3 operator-(const Vector3 &p_v) const{
                return Vector3(x - p_v.x, y - p_v.y, z - p_v.z);
            }

            Vector3& operator*=(const Vector3 &p_v){
                x *= p_v.x;
                y *= p_v.y;
                z *= p_v.z;
                return *this;
            }

            Vector3 operator*(const Vector3 &p_v) const{
                return Vector3(x * p_v.x, y * p_v.y, z * p_v.z);
            }

            Vector3& operator/=(const Vector3 &p_v){
                x /= p_v.x;
                y /= p_v.y;
                z /= p_v.z;
                return *this;
            }

            Vector3 operator/(const Vector3 &p_v) const{
                return Vector3(x / p_v.x, y / p_v.y, z / p_v.z);
            }

            Vector3& operator*=(float p_scalar){
                x *= p_scalar;
                y *= p_scalar;
                z *= p_scalar;
                return *this;
            }

            Vector3 operator*(const float p) const{
                return Vector3(x * p, y * p, z * p);
            }

            Vector3& operator/=(const float p){
                x /= p;
                y /= p;
                z /= p;
                return *this;
            }

            Vector3 operator/(const float p_scalar) const{
                return Vector3(x / p_scalar, y / p_scalar, z / p_scalar);
            }

            Vector3 operator-() const{
                return Vector3(-x, -y, -z);
            }

            bool operator==(const Vector3 &p_v) const{
                return x == p_v.x && y == p_v.y && z == p_v.z;
            }

            bool operator!=(const Vector3 &p_v) const{
                return x != p_v.x || y != p_v.y || z != p_v.z;
            }

            bool operator<(const Vector3 &p_v) const{
                if(x == p_v.x){
                    if(y == p_v.y){
                        return z < p_v.z;
                    }
                    return y < p_v.y;
                }
                return x < p_v.x;
            }

            bool operator>(const Vector3 &p_v) const{
                if(x == p_v.x){
                    if(y == p_v.y){
                        return z > p_v.z;
                    }
                    return y > p_v.y;
                }
                return x > p_v.x;
            }

            bool operator<=(const Vector3 &p_v) const{
                if(x == p_v.x){
                    if(y == p_v.y){
                        return z <= p_v.z;
                    }
                    return y < p_v.y;
                }
                return x < p_v.x;
            }

            bool operator>=(const Vector3 &p_v) const{
                if(x == p_v.x){
                    if(y == p_v.y){
                        return z >= p_v.z;
                    }
                    return y > p_v.y;
                }
                return x > p_v.x;
            }

            float length() const{
                float x2 = x * x;
                float y2 = y * y;
                float z2 = z * z;

                return sqrt(x2 + y2 + z2);
            }

            float length_squared() const{
                float x2 = x * x;
                float y2 = y * y;
                float z2 = z * z;

                return x2 + y2 + z2;
            }

            void normalize(){
                float lengthsq = length_squared();
                if(lengthsq == 0){
                    x = y = z = 0;
                }else{
                    float length = sqrt(lengthsq);
                    x /= length;
                    y /= length;
                    z /= length;
                }
            }

            Vector3 normalized() const {
                Vector3 v = *this;
                v.normalize();
                return v;
            }

            bool is_equal_approx(const Vector3 &p_v) const {
                return Math::is_equal_approx(x, p_v.x) && Math::is_equal_approx(y, p_v.y) && Math::is_equal_approx(z, p_v.z);
            }

            bool is_normalized() const{
                return Math::is_equal_approx(length_squared(), 1, (float)UNIT_EPSILON);
            }

            Vector3 slide(const Vector3 &p_normal) const {
                return *this - p_normal * this->dot(p_normal);
            }

            Vector3 bounce(const Vector3 &p_normal) const {
                return -reflect(p_normal);
            }

            Vector3 reflect(const Vector3 &p_normal) const {
                return  p_normal * 2.0f * this->dot(p_normal) - *this;
            }

    };
}