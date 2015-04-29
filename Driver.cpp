/*
 * Part5.cpp
 *
 *  Created on: Mar 24, 2015
 *      Author: Ryan Smith
 */

#include <string>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "Airplane.h"

int main(){
	/*
	 * These two tests both take sets of planes that are on a collision course.
	 * -These collisions courses are verified by an assert statement.
	 *
	 * The first set can prevent a collision in the next 10 steps by decreasing the bearing of plane2 by 10 degrees
	 * and increasing the bearing of plane1 by 10 degrees (smaller number may be possible, i just checked the max).
	 * -It uses an assert statement to verify that the collision is preventable.
	 *
	 * The second set cannot be adjusted satisfactorily.
	 * -It also uses an assert statement to verify that there is nothing to be done.
	 *
	 */

	Coordinates coords1;
	Coordinates coords2(14, 0);
	Coordinates coords3(-10, 10);
	Coordinates coords4(-10, -10);
	Coordinates coords5(10, -10);

	Airplane plane1(coords1, 90, "Plane One");
	Airplane plane2(coords2, 120, "Plane Two");
	Airplane plane3(coords3, 135, "Plane Three");
	Airplane plane4(coords4, 225, "Plane Four");
	Airplane plane5(coords5, 315, "Plane Five");
	Airplane planes[5] = {plane1, plane2, plane3, plane4, plane5};
	assert (detectCollision(planes, 10) == true);
	assert(avoidCollision(planes, 10, 5));
	std::cout << "Collision avoided for planes on collision course!" << std::endl;

	Airplane plane6(coords1, 90, "Plane Six");
	Airplane plane7(coords2, 225, "Plane Seven");
	Airplane plane8(coords3, 315, "Plane Eight");
	Airplane plane9(coords4, 45, "Plane Nine");
	Airplane plane10(coords5, 135, "Plane Ten");
	Airplane planesTwo[5] = {plane6, plane7, plane8, plane9, plane10};
	assert (detectCollision(planesTwo, 10) == true);
	assert(avoidCollision(planesTwo, 10, 5) == false);
	std::cout << "Insane pilots! The planes could not be saved!" << std::endl;

}



