/*
 * Airplane.h
 *
 *  Created on: Mar 24, 2015
 *      Author: ryancsmith
 */
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

/*
 * Coordinates class. Provides structure and methods for Coordinates objects.
 */
class Coordinates {

public:
	double x;
	double y;

	/*
	 * No argument constructor. Sets x and y to 0.
	 */
	Coordinates(){
		x = 0;
		y = 0;
	}

	/*
	 * Constructor. Sets x and y to the given values.
	 * Parameters: double - value of x, double - value of y.
	 */
	Coordinates(double x, double y){
		this->x = x;
		this->y = y;
	}
};

/*
 * Airplane class. Provides structure and methods for Airplane objects.
 */
class Airplane {

private:
	Coordinates position;
	double bearing;
	std::string id;
	/*Static ID variable, assigned to nameless planes created with no-argument constructor.*/
	static int invaderID;

public:
	/*No argument constructor, sets coordinates and bearing to 0, then assigns ID to invading plane.*/
	Airplane(){
		position = Coordinates(0, 0);
		bearing = 0;
		//stringstream used here for string value of int instead of to_string to avoid c++11 compilation
		std::stringstream ss;
		ss << "invader #" << invaderID++;
		id = ss.str();
	}

	/*Constructor.
	 * Parameters: Coordinates - position of plane, double - bearing of plane, string - plane ID
	 */
	Airplane(Coordinates position, double bearing, std::string id){
		this->position = position;
		this->bearing = bearing;
		this->id = id;
	}

	/*
	 * Determines the distance between this Airplane and given Airplane.
	 * Return: double - distance between the two planes.
	 * Parameters: Airplane - the plane from which distance is to be calculated.
	 */
	double distance(Airplane plane2){
		double plane2x = plane2.getPosition().x;
		double plane2y = plane2.getPosition().y;
		return sqrt(pow((plane2x - position.x), 2) + pow((plane2y - position.y), 2));
	}

	/*
	 * Updates the position of this Airplane by one "unit".
	 */
	void updatePosition(){
		double radians = bearing * (M_PI/180);
		position.y = position.y + (sin(radians));
		position.x = position.x + (cos(radians));
	}

	/*
	 * Sets the position of this Airplane to the given Coordinates.
	 * Parameters: Coordinates - new position of plane.
	 */
	void setPosition(Coordinates position){
		this->position = position;
	}

	/*
	 * Returns the position of this Airplane.
	 * Return: Coordinates - position of this Airplane.
	 */
	Coordinates getPosition(){
		return position;
	}

	/*
	 * Sets the bearing of this Airplane to the given double.
	 * Parameters: double - the new bearing of the Airplane.
	 */
	void setBearing(double bearing){
		this->bearing = bearing;
	}

	/*
	 * Returns the bearing of this Airplane.
	 * Return: double - the bearing of this Airplane.
	 */
	double getBearing(){
		return bearing;
	}

	/*
	 * Sets the ID of this Airplane.
	 * Parameters: string - the new ID of the Airplane.
	 */
	void setID(std::string id){
		this->id = id;
	}

	/*
	 * Returns the ID of the Airplane.
	 * Return: string - the ID of the Airplane.
	 */
	std::string getID(){
		return id;
	}

	/*
	 * Updates the bearing of this Airplane by the given increment/decrement.
	 * Return: false if the given int is not within the allowable range, (-10, 10) inclusive, true otherwise.
	 * Parameters - int - the degree by which to increment/decrement the bearing.
	 */
	bool updateBearing(int change){
		if (change > 10 || change < -10)
			return false;
		if (bearing + change > 360)
			bearing = fmod((bearing + change), 360);
		else if (bearing + change < 0)
			bearing = 360 - (bearing + change);
		else
			bearing = bearing + change;
		return true;
	}
};
/*Initializes value of static ID variable to 0.*/
int Airplane::invaderID = 0;

/*
 * Helper functions for detectCollision(). Checks every pair of planes to see if distance < safeDistance.
 * Return: true if distance between 2 planes in array < safeDistance, false otherwise.
 * Parameters: array of Airplane objects, int - minimum safe distance between planes.
 */
bool checkPairs(Airplane planes[], int safeDistance){
	//the following 2 loops will compare every possible pair in the set
	for (int a = 0; a < 5 - 1; a++){
		for (int b = a + 1; b < 5; b++){
			if (planes[a].distance(planes[b]) < safeDistance)
				return true;
		}
	}
	return false;
}

/*
 * Determines if there will be a collision between planes in the next 10 updates of position.
 * Return: true if a potential collision is detected, false otherwise.
 * Parameters: array of Airplane objects, int - minimum safe distance between planes.
 */
bool detectCollision(Airplane planeArray[], int safeDistance){
	//make a copy of all planes in the array in order to test without affecting actual positions.
	Airplane planes[5];
	for (int b = 0; b < 5; b++){
		planes[b] = planeArray[b];
	}
	//this outer loop will operate 10 times updating the position of every plane each time
	for (int i = 0; i < 10; i++){
		if (checkPairs(planes, safeDistance))
			return true;
		for (int z = 0; z < 5; z ++){
			planes[z].updatePosition();
		}
	}
	if (checkPairs(planes, safeDistance))
		return true;
	return false;
}

/*
 * Helper function for detectCollision() - exists only to allow a vector to be passed to detectCollision().
 * Makes an array from the vector then relays detectCollisions result.
 * Return: true if a potential collision is detected, false otherwise.
 * Parameters: vector of Airplane objects, int - minimum safe distance between planes.
 */
bool detectCollision(std::vector<Airplane> planes, int safeDistance){
	Airplane planeArray[5];
	//array makes an actual copy of each plane for testing
	for (int i = 0; i < 5; i++){
		planeArray[i] = planes.at(i);
	}
	return (detectCollision(planeArray, safeDistance));
}

/*
 * Recursive helper function for avoidCollision().
 * Stores potential coordinate change values in a vector. Runs base case with every possible combination of coordinate changes.
 * Return: true if there is some set of coordinate changes that can avoid collision, false otherwise.
 * Parameters: vector of Airplane objects - this is a full copy each time so that calculations can be performed on original conditions
 *             vector of ints - potenial coordinate changes, this is passed by reference to save space since values are overwritten each iteration
 *             int - minimum safe distance between planes, int - number of planes in the array.
 */
bool avoidHelper (std::vector<Airplane> planeCopy, std::vector<int>& coordChanges, int safeDistance, int numPlanes){
	//base case, try detectCollisions with new bearing adjustments and return result
	if (numPlanes == 0){
		for (int i = 0; i < planeCopy.size(); i++){
			planeCopy.at(i).updateBearing(coordChanges.at(i));
		}
		if (detectCollision(planeCopy, safeDistance))
			return false;
		else
			return true;
	}
	else{
		//fill the vector with every possible number for every possible plane recursively, return any true results
		for (int j = -10; j < 11; j++){
			coordChanges[numPlanes - 1] = j;
			if (avoidHelper(planeCopy, coordChanges, safeDistance, numPlanes - 1))
				return true;
		}
	}
	return false;
}

/*
 * Checks to see if planes are currently on collision course.
 * If so, tries every possible combination of bearing updates to see if the collision can be avoided.
 * Return: true if there is some set of coordinate changes that can avoid collision, false otherwise.
 * Parameters: array of Airplane objects, int - minimum safe distance between planes, int - number of planes in the array.
 * (*Note: this method itself is implemented for arrays of any size, however, methods called by this method only
 *         allow arrays of size 5)
 */
bool avoidCollision(Airplane planes[], int safeDistance, int numPlanes){
	//make full copy of plane objects
	std::vector<Airplane> planeCopy;
	for (int j = 0; j < numPlanes; j++){
		planeCopy.push_back(planes[j]);
	}
	//call detectCollisions to see if planes are on a collision course (this will NOT change their actual positions) - return true if no collision
	if (detectCollision(planeCopy, safeDistance) == false)
			return true;

	//if planes are on collision course, proceed to see if we can help
	//create vector representing possible coordinate changes for each plane, load with 0's
	std::vector<int> coordChanges;
	for (int i = 0; i < numPlanes; i++){
		coordChanges.push_back(0);
	}
	//introduce setup to recursive helper
	return avoidHelper(planeCopy, coordChanges, safeDistance, numPlanes);

}
