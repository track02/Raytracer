#include <iostream>
#include <math.h>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include <float.h>
#include <random>


/*
* Basic aim is to send rays through pixels and compute the color seen in that direction
* Calculate which ray goes from the eye to a pixel, compute what that ray intersects and
* the compute a colour for that intersection point
*/


float hit_sphere(const vec3& center, float radius, const ray& r){
  vec3 oc = r.origin() - center; //(A - C)
  float a = dot(r.direction(), r.direction()); //(B*B)
  float b = 2.0 * dot(oc, r.direction()); //2 * (B*(A-C))
  float c = dot(oc, oc) - radius*radius; //((A-C)*(A-C)) - R*R
  float discriminant = b*b - 4*a*c; //quadratic formula discriminant sqrt(b^2 - 4ac)
    
  if(discriminant < 0) //Ray hasn't hit, use -1 to indicate this
    return -1.0; 
  else //Discriminant >= 0, return smallest root (t) (assuming this is the closest)
    return (-b - sqrt(discriminant)) / (2.0 * a);  //Remember quadratic formula -b (+-) sqrt(b*b - 4ac) / 2a
  
}


//Color function returns the colour of the background as a basic gradient
//It blends white/blue depending on the up/down value of the rays y coordinate
//t = 0 -> white / t = 1 -> blue
//Known as linear interpolation (lerp), always take the form of (1-t)*start_value + t*end_value
//Where t can be between 1 and 0

vec3 color(const ray& r, hitable *world){

  hit_record rec; //Details of ray collision
  
  if(world->hit(r, 0.0, FLT_MAX, rec)) //If ray hits
    return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1); //Determine colour using normal
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
  
  //Random no. generator for anti-aliasing samples
  std::random_device rd; //random_device -> uniformly-distributed random no. generator
  std::uniform_real_distribution<double> dist(0.0, 0.99); //a distribution of nos. between 0-0.99
  
  
  int ns = 100; //no. of samples to take per pixel
  
	//Loop through each column / row and write an RGB triplet
	//Top to bottom - left to right
	for (int j = ny-1; j >= 0; j--)	{
		for (int i = 0; i < nx; i++) {

      //"empty" colour vector each pixel
      vec3 col(0,0,0);
      
      //Sum up ray colours for each random sample at each pixel
      for (int s = 0; s < ns; s++){
      
              float u = (float(i) + float(dist(rd))) / float(nx);
              float v = (float(j) + float(dist(rd)))/ float(ny);
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