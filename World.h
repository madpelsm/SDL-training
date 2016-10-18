#pragma once
#include <vector>
#include <string>

class World {
public:
	std::vector<float> vboArray;
	void readLevel(std::string fileLocation);
	void writeLevel(std::string fileLocation);
	void writeToVBOArray();
	

	World();
	~World();


};