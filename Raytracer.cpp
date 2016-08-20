#include <iostream>

int main()
{
	//Start  by generating ppm files

	int nx = 200; //width
	int ny = 100; //height

	//This produces the following:
	//P3 <-- This means colours are in ASCII
	//200 100 <-- 200 columns x 100 rows 
	//255 <-- Max possible values of 255 for a colour
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	//Loop through each column / row and write an RGB triplet
	//Top to bottom - left to right
	for (int j = ny-1; j >= 0; j--)	{
		for (int i = 0; i < nx; i++) {

			//Generate float components from 0.0 to 1.0
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2;

			//Convert to integer format suitable for ppm
			int ir = int(255.99 * r);
			int ig = int(255.99 * b);
			int ib = int(255.99 * g);

			//Output in format R G B
			std::cout << ir << " " << ig << " " << ib << "\n";

		}
	}
}