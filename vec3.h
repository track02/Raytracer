#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3  {

    
public:

    //Constructor - vec3 consists of 3 components 
    vec3() {}
    vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    
    //Accessors - may refer to the components as either x,y,z or r,g,b depending on vectors use
    //Note: Member functions make use of an implicit "this" refering to the object
    //they are invoked upon. Const after parameter listing denotes that this implicit this is const
    //meaning that the method cannot modify the object on which it was invoked* (Read-Only)

    //*there is an exception for mutable objects however
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }
    
    
    //Overload operators
    inline const vec3& operator+() const { return *this; } //Returns address of current vector
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } //Returns a new negative vector
    
    //[ ] is generally overloaded twice a const function for reading and a second for writing
    inline float operator[](int i) const { return e[i]; } //Index notation for accessing x,y,z / r,g,b (read)
    inline float& operator[](int i) { return e[i]; }; //Index notation for accessing x,y,z / r,g,b (write)
    
    //Declarations for operator overrides 
    //remember - const parameters cannot be modified, here they are used to update calling vector
    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);
    
    //Functions declarations
    //Length (magnitude) 
    inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
    
    //A unit vector is a normalised vector, magnitude of 1
    inline void make_unit_vector();
    
    
    float e[3];
};


//Overload input stream - write input to a vector
inline std::istream& operator>>(std::istream &is, vec3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

//Overload output stream - print out vector details
inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

//Normalises a vector
inline void vec3::make_unit_vector() {
    float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

//Add two vectors together (r1 + r2, g1 + g2, b1 + b2) and return new vector
inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

//subtract two vectors together (r1 - r2, g1 - g2, b1 - b2) and return new vector
inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

//multiply two vectors together (r1 * r2, g1 * g2, b1 * b2) and return new vector
inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

//divide two vectors  (r1 / r2, g1 / g2, b1 / b2) and return new vector
inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

//multiply vector by a scalar (r1 * s, g1  * s, b1 * s) and return new vector
inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

//divide vector by a scalar (r1 / s, g1  / s, b1 / s) and return new vector
inline vec3 operator/(vec3 v, float t) {
    return vec3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

//multiply vector by a scalar (r1 * s, g1  * s, b1 * s) and return new vector [reversed [parameter ordering]
inline vec3 operator*(const vec3 &v, float t) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

//Calculates the dot product of two vectors (r1 * r2 + g1 * g2 + b1 * b2)
inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1]  + v1.e[2] *v2.e[2];
}

//Calculates and returns cross product
//(g1 * b2 - b1 * g1)
//-(r1 * b2 - b1 * r2)
//(r1 * b2 - b1 * r2)
inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
                (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
                (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}


//Updates vector values by adding passed vector values
inline vec3& vec3::operator+=(const vec3 &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    return *this;
}

//Updates vector values by multiplying passed vector values
inline vec3& vec3::operator*=(const vec3 &v){
    e[0]  *= v.e[0];
    e[1]  *= v.e[1];
    e[2]  *= v.e[2];
    return *this;
}

//Updates vector values by dividing passed vector values
inline vec3& vec3::operator/=(const vec3 &v){
    e[0]  /= v.e[0];
    e[1]  /= v.e[1];
    e[2]  /= v.e[2];
    return *this;
}

//Updates vector values by subtracting passed vector value
inline vec3& vec3::operator-=(const vec3& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    return *this;
}

//Updates vector values by multiplying by scalar value
inline vec3& vec3::operator*=(const float t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    return *this;
}

//Updates vector values by dividing by scalar value
inline vec3& vec3::operator/=(const float t) {
    float k = 1.0/t;
    
    e[0]  *= k;
    e[1]  *= k;
    e[2]  *= k;
    return *this;
}

//Divides a vector by its length - normalising
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}
