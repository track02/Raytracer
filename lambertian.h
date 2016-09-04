#include material.h


/* Chapter 8
 * 
 * For diffuse (lambertian) materials we want to either
 * always scatter rays and attenuate by the materials reflectance R
 * or absorb the 1-R fraction of the rays.
 * We may use a mixture of the two strategies. 
 * 
 * This is implemented as a simple class which derives from the virtual abstract material class
 */
 
 class lambertian : public material{
	 public:
		lambertian(const vec3& a) albedo(a) {}
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target-rec.p);
		attenuation = albedo;
		return true;
	}
	
	vec3 albedo;
};
  
 
