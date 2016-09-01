#include "ray.h"
#pragma once

//Chapter 6 - We'll abstract out a camera class to encapsulate the simple axis-aligned camera from main.

class camera{

  public:
    camera(){
     
      //The origin is where rays originate from
      origin = vec3(0.0, 0.0, 0.0);    
      
      //Initial direction of rays is the lower left corner
      lower_left_corner = vec3(-2.0, -1.0, -1.0);
      
      //Modify the direction by adding the horizontal / vertical vectors to the direction 
      //horizontal / vertical multiplied by some factor to adjust ray direction (u / v)
      horizontal = vec3(4.0, 0.0, 0.0);
      vertical = vec3(0.0, 2.0, 0.0);
      
    }
    
    ray get_ray(float u, float v) {
      return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }    

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

};