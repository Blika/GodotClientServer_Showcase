#pragma once

#include <cmath>

#define CMP_EPSILON 0.00001f
#define CMP_EPSILON2 (CMP_EPSILON * CMP_EPSILON)
#define UNIT_EPSILON 0.001
#define Math_PI 3.1415926535897932384626433833

namespace Math{

    inline double deg_to_rad(double p_y){
        return p_y * Math_PI / 180.0;
    }
    inline float deg_to_rad(float p_y){
        return p_y * static_cast<float>(Math_PI) / 180.f;
    }

    inline double rad_to_deg(double p_y){
        return p_y * 180.0 / Math_PI;
    }
    inline float rad_to_deg(float p_y){
        return p_y * 180.f / static_cast<float>(Math_PI);
    }

    inline double round(double p_val){
        return (p_val >= 0) ? floor(p_val + 0.5) : -floor(-p_val + 0.5);
    }
    inline float round(float p_val){
        return (p_val >= 0) ? floor(p_val + 0.5f) : -floor(-p_val + 0.5f);
    }

    inline double floor(double p_x){
        return ::floor(p_x);
    }
    inline float floor(float p_x){
        return ::floorf(p_x);
    }

    inline double ceil(double p_x){
        return ::ceil(p_x);
    }
    inline float ceil(float p_x){
        return ::ceilf(p_x);
    }

    inline double exp(double p_x){
        return ::exp(p_x);
    }
    inline float exp(float p_x){
        return ::expf(p_x);
    }

    inline double sin(double p_x){
        return ::sin(p_x);
    }
    inline float sin(float p_x){
        return ::sinf(p_x);
    }

    inline double cos(double p_x){
        return ::cos(p_x);
    }
    inline float cos(float p_x){
        return ::cosf(p_x);
    }

    inline double tan(double p_x){
        return ::tan(p_x);
    }
    inline float tan(float p_x){
        return ::tanf(p_x);
    }

    inline double sinh(double p_x){
        return ::sinh(p_x);
    }
    inline float sinh(float p_x){
        return ::sinhf(p_x);
    }

    inline float sinc(float p_x){
        return p_x == 0 ? 1 : ::sin(p_x) / p_x;
    }
    inline double sinc(double p_x){
        return p_x == 0 ? 1 : ::sin(p_x) / p_x;
    }

    inline float sincn(float p_x){
        return (float)sinc(Math_PI * p_x);
    }
    inline double sincn(double p_x){
        return sinc(Math_PI * p_x);
    }

    inline double cosh(double p_x){
        return ::cosh(p_x);
    }
    inline float cosh(float p_x){
        return ::coshf(p_x);
    }

    inline double tanh(double p_x){
        return ::tanh(p_x);
    }
    inline float tanh(float p_x){
        return ::tanhf(p_x);
    }

    inline double asin(double p_x){
        return ::asin(p_x);
    }
    inline float asin(float p_x){
        return ::asinf(p_x);
    }

    inline double acos(double p_x){
        return ::acos(p_x);
    }
    inline float acos(float p_x){
        return ::acosf(p_x);
    }

    inline double atan(double p_x){
        return ::atan(p_x);
    }
    inline float atan(float p_x){
        return ::atanf(p_x);
    }

    inline double atan2(double p_y, double p_x){
        return ::atan2(p_y, p_x);
    }
    inline float atan2(float p_y, float p_x){
        return ::atan2f(p_y, p_x);
    }

    inline double sqrt(double p_x){
        return ::sqrt(p_x);
    }
    inline float sqrt(float p_x){
        return ::sqrtf(p_x);
    }

    inline double pow(double p_x, double p_y){
        return ::pow(p_x, p_y);
    }
    inline float pow(float p_x, float p_y){
        return ::powf(p_x, p_y);
    }

    inline double log(double p_x){
        return ::log(p_x);
    }
    inline float log(float p_x){
        return ::logf(p_x);
    }

    inline bool is_equal_approx(float a, float b){
        // Check for exact equality first, required to handle "infinity" values.
        if (a == b) {
            return true;
        }
        // Then check for approximate equality.
        float tolerance = (float)CMP_EPSILON * abs(a);
        if (tolerance < (float)CMP_EPSILON) {
            tolerance = (float)CMP_EPSILON;
        }
        return abs(a - b) < tolerance;
    }

    inline bool is_equal_approx(float a, float b, float tolerance){
        // Check for exact equality first, required to handle "infinity" values.
        if (a == b) {
            return true;
        }
        // Then check for approximate equality.
        return abs(a - b) < tolerance;
    }

    inline bool is_zero_approx(float s){
        return abs(s) < (float)CMP_EPSILON;
    }

    inline bool is_equal_approx(double a, double b){
        // Check for exact equality first, required to handle "infinity" values.
        if (a == b) {
            return true;
        }
        // Then check for approximate equality.
        double tolerance = CMP_EPSILON * abs(a);
        if (tolerance < CMP_EPSILON) {
            tolerance = CMP_EPSILON;
        }
        return abs(a - b) < tolerance;
    }

    inline bool is_equal_approx(double a, double b, double tolerance){
        // Check for exact equality first, required to handle "infinity" values.
        if (a == b) {
            return true;
        }
        // Then check for approximate equality.
        return abs(a - b) < tolerance;
    }

    inline bool is_zero_approx(double s){
        return abs(s) < CMP_EPSILON;
    }
}