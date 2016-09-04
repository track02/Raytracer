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

class material{
	public:
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

