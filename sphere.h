#include "hitable.h"
#include <math.h>
#include "material.h"
#pragma once

//Chp 5

/* 
* Extract out sphere implementation from main and bundle into its own class
* allows multiple spheres to be used, spheres are derived from hitable as they are an object
* rays can interact with.
*/


//Chapter 4 - adding a sphere

/*
* equation for a sphere centered at the origin, with radius R is x*x + y*y + z*z = R*R
* so, for any (x,y,z) if x*x + y*y + z*z = R*R then (x,y,z) is on the sphere, otherwise its not
* If the sphere is centered at (cx,cy,cz) then the formula differs slightly:
* (x-cx)*(x-cx) + (y-cy)(y-cy) + (z-cz)(z-cz) = R*R
*
* We'll use our vec3 class to bundle values together and to simplify the equation
* The vector from the spheres center C (cx,cy,cz) to some point p (x,y,z) is given by (p - C)
* and dot product of((p-C),(p-C)) = (x-cx)*(x-cx) + (y-cy)(y-cy) + (z-cz)(z-cz)
* 
* So the equation for a sphere using vector is: dot((p-C),(p-C)) = R*R
* Any point p that satisfies this equation lies on the sphere
*
* We want to know if our ray p(t) = A + t*B ever hits the sphere, 
* if it does there is some t for which p(t) satisfies the sphere equation
* so we are looking for a any t where this is true:
*
* dot((p(t) - c),(p(t) - c)) = R*R
* dot((A + t*B - C), (A + t*B - C)) = R*R [Expanding out p(t)]
*
* If we expand the equation and move all terms to the left hand side, we get:
* t*t*dot(B,B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R*R = 0
*
* We end up with a quadratic equation with an unknown value of t
* Solving for t, we can use the discriminant to determine the number of roots (+'ve -> 2 / -'ve -> 0 / 0 -> 1)
*
*  -----------------------------> 0 Roots
*
*  ---- o X o ------------------> 1 Root (Glancing)
*     o        o       
* ---X----------X----------------> 2 Roots      
*    o          o      
*     o        o        
*        o  o             
* 
* We can hard code this test into a function, colouring red any pixel that hits
* the sphere we place at -1 on the z-axis
*
*/


//Chapter 5 (Pt.1) - Surface Normals

/*
* Use surface normals for shading, a surface normal is a vector perpendicular to the surface
* by convention it points away from the surface. For conveniance, normals will be of unit-length (magnitude 1)
* For our sphere the normal is in the direction of the ray-hitpoint - the center
*
*
*        o  o 
*     o        o       
*    o     c----p-----> p-c
*    o          o      
*     o        o        
*        o  o   
*
* 
* A common trick to visualise normals assuming N is a unit length-vector 
* with each component being between -1 and 1 is to map each component to the interval 0 to 1
* and then map x/y/z to r/g/b
* For the normal we need the hitpoint, not just whether the ray hit the sphere or not
* we'll assume the smallest t is the closest hitpoint
*/

//Chapter 8 - Sphere now takes a ptr to a material type denoting
//what the sphere is made of
//When a light ray hits the sphere, the received hit_record is passed this
//material pointer allowing the ray/material interaction to be determined

class sphere: public hitable {
  public:
    sphere() {}
    //Note : is initialization list syntax, center / radius are assigned values cen / r
    sphere(vec3 cen, float r, material* m) : center(cen), radius(r), mat_ptr(m) {}; 
    //hit function from hitable
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    vec3 center;
    float radius; 
    material* mat_ptr;

};


//Spheres implementation of hit
bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const{

  vec3 oc = r.origin() - center; //(A - C)
  float a = dot(r.direction(), r.direction()); //(B*B)
  float b = 2.0 * dot(oc, r.direction()); //2 * (B*(A-C))
  float c = dot(oc, oc) - radius*radius; //((A-C)*(A-C)) - R*R
  float discriminant = b*b - 4*a*c; //quadratic formula discriminant sqrt(b^2 - 4ac)
  
  if(discriminant > 0){
    float temp = (-b - sqrt(b*b-4*a*c))/(2*a); //first root
    if (temp < tmax && temp > tmin) { //within t interval, update the record
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
    temp = (-b + sqrt(b*b-4*a*c))/(2*a); //second root
      if (temp < tmax && temp > tmin) { //within t interval, update the record
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }

  return false;

}
