#include "ray.h"
#pragma once


class material;

//Chp 5

/* 
* hitable is an abstract class used to model an object or "thing" a ray can collide with or hit
* most ray tracers use a valid interval for t, so a hit is only allowed if tmin < t < tmax
* initially this interval is any positive t
*/

//Bundle up details of the hit in a hit_record structure
struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
  material *mat_ptr;
};

class hitable {

  public:
  
    //declaration of hit function
    //Note: =0 denotes a pure virtual function this must be implemented derived class
    //pure virtual -> abstract / virtual -> polymorphic 
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const = 0;

};
