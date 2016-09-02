#include <iostream>
#include <math.h>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include <float.h>
#include <stdlib.h>



/*
* Basic aim is to send rays through pixels and compute the color seen in that direction
* Calculate which ray goes from the eye to a pixel, compute what that ray intersects and
* the compute a colour for that intersection point
*/


/* Chapter 7 - Diffuse Materials
* Now that objects and multiple rays per pixel are implemented we can start simulating materials
* Beginning with diffuse (matte) materials, we will treat shapes and materials
* as individual items which can be mixed and matched e.g. we assign a material to a sphere
* another option is to have the material  be dependent on the shape (useful if geometry is linked to material)
*
* Diffuse objects that don't emit light take on the colour of their surroundings
* and then modulate the surrounding colour with their own intrinsic colour
*
* Light that reflects off a diffuse surface has its direction randomised
* Rays may also be absorbed rather than reflected, the darker the surface the more likely absorption
*
* We need to implement an algorithm that randomises direction, one way is as follows
* 1. Pick a random point s, from within a a sphere tangent to the ray hitpoint, p
* 2. Send a ray from the hitpoint, p to the random point s,
* 
* The sphere has a radius of N, with the center (p+N)
* 
* We need a way to pick a random point in a unit radius sphere centered at the origin
* to do this we'll use a rejection method, picking a random point in the unit cube (x,y,z range -1 to 1)
* we reject the point and try again if it lies outside the unitsphere
*
*/


/*        , - ~ ~ ~ - ,
*    , '                ' ,
*  ,                        ,
* ,                          ,
*,            (p+N)           ,
*,             X     s        '
*,             |    /         ,
* , (radius N) |   /          ,
*  ,           |  /          ,
*    ,         | /        , '
*      ' - , _ |/ _ , - '
* -------------p---------------
*             (hitpoint)
*/

//This function returns our random point (s) that falls within the unit sphere
vec3 random_in_unit_sphere() {
  vec3 p;
  std::random_device rd; //random_device -> uniformly-distributed random no. generator
  std::uniform_real_distribution<double> dist(0.0, 0.99); //a distribution of nos. between 0-0.99
  do{
    p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
  }while (p.squared_length() >= 1.0);
  return p;
}

//Color function returns the colour of the background as a basic gradient
//It blends white/blue depending on the up/down value of the rays y coordinate
//t = 0 -> white / t = 1 -> blue
//Known as linear interpolation (lerp), always take the form of (1-t)*start_value + t*end_value
//Where t can be between 1 and 0


//Chapter 7 - Updated to simulate diffuse materials
vec3 color(const ray& r, hitable *world){

  hit_record rec; //Details of ray collision
  
  if(world->hit(r, 0.0, FLT_MAX, rec)){ //If ray hits
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    return 0.5*color(ray(rec.p, target-rec.p), world);
  }
  else{
    //No - determine background colour
    vec3 unit_direction = unit_vector(r.direction()); //Convert the direction of the ray into a unit vector (magnitude of 1)
    float t = 0.5*(unit_direction.y() + 1.0); //Calculate some value for t depending on rays y value
    return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0); //Create a vector using t (color)
  }
}

int main()
{
	//Start  by generating ppm files

  //Image dimensions
	int nx = 200; //width
	int ny = 100; //height

	//This produces the following:
	//P3 <-- This means colours are in ASCII
	//200 100 <-- 200 columns x 100 rows 
	//255 <-- Max possible values of 255 for a colour
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	
	//The origin is the eye/camera where rays originate from
	vec3 origin(0.0, 0.0, 0.0);
	
	//Initial direction of rays is the lower left corner
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	
	//Modify the direction by adding the horizontal / vertical vectors 
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	
	//List of 2 hitable objects
	hitable* list[2]; 
	list[0] = new sphere(vec3(0,0,-1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2); //Create a "world" containing these objects


  //Chapter 6 - Anti-aliasing
  /*
  * Usually pictures taken with a camera do not have jagged edges
  * this is because the edge pixels are a blend of foreground and background
  * We can achieve the same effect by averaging a bunch of samples inside each pixel
  *
  * For a given pixel we have several samples within that pixel and send rays through each
  * of the samples, the colours of these rays is then averaged
  */
  
  camera cam;

  
  int ns = 100; //no. of samples to take per pixel
  
	//Loop through each column / row and write an RGB triplet
	//Top to bottom - left to right
	for (int j = ny-1; j >= 0; j--)	{
		for (int i = 0; i < nx; i++) {

      //"empty" colour vector each pixel
      vec3 col(0,0,0);
      
      //Sum up ray colours for each random sample at each pixel
      for (int s = 0; s < ns; s++){
      
              float u = (float(i) + float(drand48()) / float(nx);
              float v = (float(j) + float(drand48())/ float(ny);
              ray r = cam.get_ray(u, v);
              
              col += color(r, world);
      
      }
      
      //Divide colour by total no. samples for an average
      col /= ns;
      
      //Convert to integer
      int ir = int(255.99 * col.r());
      int ig = int(255.99 * col.g());
      int ib = int(255.99 * col.b());

			//Output in format R G B
			std::cout << ir << " " << ig << " " << ib << "\n";

		}
	}
}