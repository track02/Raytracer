#include <iostream>


#include "ray.h"

//Chapter 3 - using ray.h

/*
* Basic aim is to send rays through pixels and compute the color seen in that direction
* Calculate which ray goes from the eye to a pixel, compute what that ray intersects and
* the compute a colour for that intersection point
* for now we'll have a blank scene and with the rays hitting the background of the image
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
* We end up a quadratic equation with an unknown value of t
* Solving for t, we can use the discriminant to determine the number of roots (+'ve -> 2 / -'ve -> 0 / 0 -> 1)
*
*  -----------------------------> 0 Roots
*
*  ---- o X o ------------------> 1 Root
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


bool hit_sphere(const vec3& center, float radius, const ray& r){
  vec3 oc = r.origin() - center; //(A - C)
  float a = dot(r.direction(), r.direction()); //(B*B)
  float b = 2.0 * dot(oc, r.direction()); //2 * (B*(A-C))
  float c = dot(oc, oc) - radius*radius; //((A-C)*(A-C)) - R*R
  float discriminant = b*b - 4*a*c; //quadratic formula discriminant sqrt(b^2 - 4ac)
  return (discriminant>0);

}


//Color function returns the colour of the background as a basic gradient
//It blends white/blue depending on the up/down value of the rays y coordinate
//t = 0 -> white / t = 1 -> blue
//Known as linear interpolation (lerp), always take the form of (1-t)*start_value + t*end_value
//Where t can be between 1 and 0

//Chp 4 - Also checks for sphere hit
vec3 color(const ray& r){

  if (hit_sphere(vec3(0,0,-1), 0.5, r)) //If ray r hits a sphere at (0,0,-1) w/ Radius 5
    return vec3(1,0,0); //Return red

  vec3 unit_direction = unit_vector(r.direction()); //Convert the direction of the ray into a unit vector (magnitude of 1)
  float t = 0.5*(unit_direction.y() + 1.0); //Calculate some value for t depending on rays y value
  return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0); //Create a vector using t (color)
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


	//Loop through each column / row and write an RGB triplet
	//Top to bottom - left to right
	for (int j = ny-1; j >= 0; j--)	{
		for (int i = 0; i < nx; i++) {

      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      
      //Create a ray from the origin with a direction determined by i/j values
      ray r(origin, lower_left_corner + u*horizontal + v * vertical);  
      //Retrieve a colour for this ray
      vec3 col = color(r);
      
      //Convert to integer
      int ir = int(255.99 * col.r());
      int ig = int(255.99 * col.g());
      int ib = int(255.99 * col.b());

			//Output in format R G B
			std::cout << ir << " " << ig << " " << ib << "\n";

		}
	}
}