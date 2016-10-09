#include "ray.h"
#pragma once

//Chapter 6 - We'll abstract out a camera class to encapsulate the simple axis-aligned camera from main.

//Chapter 10 - Adjustable camera
//Start by allowing the vertical field of view to be adjusted
//This is the angle you see through the portal or viewfinder
//Since our image isnt square the fov differs horizontally and vertically

//Vertical fov can be specified in degrees/radians (a)
//We keep the rays coming from the origin and heaing into the z=-1 plane

/*   ^ Y
 *   |     /|
 *   |    / |
 *   |   /  |
 *   |  /   | (h)
 *   | /    |
 *   |/(a/2)|
 *   o-----------------> -Z
 *    \(a/2)|
 *     \    |
 *      \   | (h)
 *       \  |
 *        \ |
 *         \|
 * 
 */
 
 //Where h = than(a/2) 
 
 /*Next we want to be able to place the camera and have it look at a certain point
  * 
  *We'll call the position where the camera is lookfrom and the 
  *position the camera is looking at, lookat
  * 
  *The roll or sideways-tilt of the camera can also be specified - this is the rotation
  *around the lookat-lookfrom axis 
  *
  *A way to specify an up will be needed here, we already have a place that the up-vector
  *needs to be in, the plane orthogonal to the view direction.
  * 
  *We can use any up-vector we like and project it onto this place to get an up-vector for the camera 
  *The name "view up" (vup) will be used to name the up-vector. 
  *        
  *         ^ (vup)
  *         |
  *        /|\
  *       / | \     ^ (v)
  *      /  |  \   /
  *     /   |   \ /         
  *    / \  |    \    [LOOKFROM]                             [LOOKAT]
  *   /   \ |   / \   
  *   \    \|  /   \
  * (w)<----|-------\---------------------------------------->X    
  *     \  / \       \
  *      \/   \     /
  *       \    \   /
  *        \    \ /
  *         \    \ (u)
  *          \  / \ 
  *                v
  * 
  *  We can describe our camera orientation using an othornormal basis (u,v,w)
  *  Orthonormal - A basis V whose vectors are orthonormal, that is, they are all unit vectors and orthogonal to each other.
  * 
  *  Keep in mind that vup, v, and w are all in the same place, note that before our fixed camera faced -Z
  *  Our arbitrary view camera now faces -w. We can use world up (0,1,0) to specify vup. This is convenient and 
  *  should help keep the camera level.
  */      

class camera{

  public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) { // vfov is top to bottom in degrees
		
            float theta = vfov*M_PI/180;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            lower_left_corner = origin  - half_width*u -half_height*v - w;
            horizontal = 2*half_width*u;
            vertical = 2*half_height*v;
        }
    

        ray get_ray(float s, float t) {
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin); 
        }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u,v,w;
};

class camera_fov{

  public:
    camera_fov(float vfov, float aspect){
     
		float theta = vfov*M_PI/180;
		float half_height = tan(theta/2);
		float half_width = aspect * half_height;
		origin = vec3(0.0,0.0,0.0);
		lower_left_corner = vec3(-half_width, -half_height, -1.0);
		horizontal = vec3(2*half_width, 0.0, 0.0);
		vertical = vec3(0.0, 2*half_height, 0.0);
      
    }
    
    ray get_ray(float u, float v) {
      return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }    

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

};

