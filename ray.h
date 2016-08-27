#include "vec3.h"

/* A ray can be seen as a function p(t) = A + t*B
* Where p is some 3D Position alonge a line
* A is the rays origin and B is the rays direction
* The ray parameter, t is a real number
* by changing the value of t p(t) moves the point along the ray
* for positive values of t, we can only reach points on the line infront of A (the origin) 
* this is known as a half-line or half-ray
*
* But if we also allow negative values of t, we can move to anywhere on the line
* E.g.

*          <-(-B)---     (A)   ---(+B)->    
* <------------------------------------------------->
*       ^                 ^                ^
*      T = -1           T = 0            T = 1

* 
*/

class ray  {

  public:
    
    //Constructors
    ray(){}
    ray(const vec3& a, const vec3& b) {A = a; B = b;}
    
    //Accessors, note const this
    vec3 origin() const {return A;}
    vec3 direction() const {return B;}
    
    //Determines position along the ray depending on given value t
    vec3 point_at_parameter(float t) const {return A + t*B;}
    
  private:
  
    //Attributes
    vec3 A;
    vec3 B;



};
