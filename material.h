#include "ray.h"
#include "hitable.h"
#include <stdlib.h>
#pragma once

struct hit_record;


//Chapter 8 - Metal Materials

/*
 * If we want different objects to have different materials
 * there are two potential implementations
 * 1) A universal material with lots of parameters 
 *    and specific material types which zero out specific parameters
 * 2) An abstract class which encapsulates behaviour
 * 
 * We'll go for option two, our material needs to do the following
 * 	a. produce a scattered ray or say it absorbed the incident (original) ray
 *  b. if scatter say how much the ray should be attenuated (reduced in intensity)
 */

/*
 * The hit record is used to bundle together arguments, hitables (objects) and materials
 * need to know of each other so there is some circularity of references.
 * 
 * So material will tell use how rays interact with the surface, when a ray hits a surfacce
 * the material pointer in the hit_record will be set to point at the material pointer the sphere was
 * given when created in main.
 * 
 * When the color routine receives the hit record it can call member functions of the material pointer 
 * to find out what ray, if any was scattered.
 * 
 */

//This function returns our random point (s) that falls within the unit sphere
	vec3 random_in_unit_sphere() {
	vec3 p;
	do{
		p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
	}while (p.squared_length() >= 1.0);
	return p;
}
 

//Reflect function - for reflective materials

/*
* Example using 2D Surface
* ray_in [v] = (1,-1), travelling right/down
* normal [n] = (0,1), vertical up
* ray_out [r] = (1,1), travelling right/up
*     
*     (v)  (n)   (r)
*      \    |    /
*       \   |   /
*        \  |  /
*         \ | /
*      ____\|/______ (surface)
*
* r = v-2(v*n)n
* = (1,-1) - 2*((1,-1)*(0,1))*(0,1)
* = (1,-1) - 2*(0,-1)*(0,1) 
* = (1,-1) - (-(0,2))
* = (1,-1) + (0,2)
* = (1,1)
*/


inline vec3 reflect(const vec3& v, const vec3& n){
	return v - 2*dot(v,n)*n;
}

class material{
	public:
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

/* Chapter 8
 * 
 * For diffuse (lambertian) materials we want to either
 * always scatter rays and attenuate by the materials reflectance R
 * or absorb the 1-R fraction of the rays.
 * We may use a mixture of the two strategies. 
 * 
 * This is implemented as a simple class which derives from the virtual abstract material class
 */
  
 //Attenuation - rays attenuate (decrease in intensity) as they are scattered or reflected
 //Albedo - reflective coefficient, multiply colour vector by albedo to reflect loss of ray intensity
 
 class lambertian : public material{
	 
	 public:
		lambertian(const vec3& a) : albedo(a) {}
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const{
			vec3 target = rec.p + rec.normal + random_in_unit_sphere();
			scattered = ray(rec.p, target-rec.p);
			attenuation = albedo;
			return true;
		}
	
	vec3 albedo;
};


//Reflection - Chapter 8
//For smooth metals rays are not randomly scattered instead they are reflected 


/*     (v)         (r)  
 *       \         /|
 *        \  (N)  / |
 *         \  |  /  | (B)
 *          \ | /   |
 *           \|/    |
 *        ----x-----^----------------------
 *             \    |
 *              \   |
 *               \  | (B)       
 *                \ |
 *                 \|
 * 
 * The reflected ray direction (r) is equal to the inital ray (v) + 2B
 * N (surfacenormal) is a unit vector, but v may not be
 * So the length of B should be dot(v,N)
 * Because v points inwards a minus sign is used, giving the following function
 */ 
 
 //Perfect reflection isn't likely, so we'll randomise the reflected direction by
 //using a small sphere and picking a new endpoint for the ray
 
 //Larger spheres produce more fuzzy reflections, to accomplish this a fuzziness parameter
 //will be added, that is just the radius of the sphere, where zero fuzziness is perfect reflection
 //Note: large spheres may cause light to scatter beneath the reflecting surface 
 //i.e. sphere radius is so large it passes inside the surface, in this case 
 //we'll have the surface absorb those scattered rays
 //A hard limit of 1 for the sphere radius will be used


class metal : public material {
	public:
		metal(const vec3& a, float f) : albedo(a) {if (f < 1) fuzz = f; else fuzz = 1;}
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const{			
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal); //direction of reflected ray
			scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere()); //Create a scattered ray using origin of r_in and reflected direction multiplied by fuzz value
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
			
		}
		
		vec3 albedo;
		float fuzz;
};

