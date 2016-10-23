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
  
  
  /* Chapter 11 - Defocus Blur (Depth of Field)
   * In a real camera a lens is in place between the film and an object plane we're looking at
   * The distance to that plane where things are in focus is controlled by the distance between 
   * the lens and the film. The aperture of a camera controls have big the lens is effectively, if 
   * more light is needed the aperture is made bigger.
   * 
   * For our virtual camera we have perfect film/sensor and never need more light so
   * we only have an apeture when we want defocus blur.
   * 
   * For our raytracer we'll simulate the order: sensor, lens, aperture and figure out where
   * to send the rays and flip the image once computer (the image is projected upside-down on the film)
   * In graphics a thin lens approximation is usually used.
   * 
   * We'll start by sending rays from the surface of the lens and towards a virtual film plane.
   * by find the projection of the film on the plane that is in focus (at the distance focus_dist).
   * 
   * 
   * 
   *   |                       |
   *   |           ()          |
   *   |        / (  ) \       |
   *   |          (  )         |
   *   |      /   (  )   \     |
   *   |          (  )         |
   *   |    /     (  )     \   |
   *   |          (  )         |
   *   |  /       (  )       \ |
   *   |          (  )         |
   *   |  \       (  )       / |
   *   |          (  )         |
   *   |    \     (  )     /   |
   *   |          (  )         |
   *   |      \   (  )   /     |
   *   |          (  )         |
   *   |        \ (  ) /       |
   *   |           ()          |
   *   |                       |
   * (film)      (lens)    (focus plane)     
   *              
   *    <-----------|------------>
   *        Inside     Outside
   * 
   * 
   * 
   *       ) \   |
   *        ) \  |
   *        )  \ |
   * Lens   )___\| Virtual film plane at the focus plane.
   *        )   /|
   * 	    )  / |
   * 	    ) /  |
   *       ) /   |
   * 
   * For this we just need to have the ray origins be on a disk around
   * loookfrom rather than from a point.
   */

vec3 random_in_unit_disk(){
	vec3 p;
	do {
		p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1,1,0);
	}while(dot(p,p) >= 1.0);
	return p;	
}

class camera{

  public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) { // vfov is top to bottom in degrees
			lens_radius = aperture / 2;
            float theta = vfov*M_PI/180;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
            horizontal = 2*half_width*focus_dist*u;
            vertical = 2*half_height*focus_dist*v;
        }
    

        ray get_ray(float s, float t) {
			vec3 rd = lens_radius*random_in_unit_disk();
			vec3 offset = u *rd.x() * v * rd.y();
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin - offset); 
        }

    vec3 origin;
    float lens_radius;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u,v,w;
};

