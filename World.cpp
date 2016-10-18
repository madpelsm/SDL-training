#include <string>
#include <vector>
#include <iostream>
#include "World.h"
#include <fstream>
#include <sstream>

void World::readLevel(std::string fileLocation) {
	std::string line;
	std::ifstream myfile(fileLocation);
	if (myfile.is_open())
	{

		while (!myfile.eof())
		{
			getline(myfile, line);
		}
		myfile.close();

		std::stringstream ss(line);
		int i;
		int index = 0;
		while (ss >> i)
		{
			this->vboArray.push_back(i);

			if (ss.peek() == ',')
				ss.ignore();
		}
		for (i = 0; i< this->vboArray.size(); i++)
			std::cout << this->vboArray.at(i) << std::endl;
	}
	else std::cout << "Unable to open file" << std::endl;


}
void World::writeLevel(std::string fileLocation) {
	std::ofstream levelWriter;
	if (this->vboArray.size() != 0) {
		levelWriter.open(fileLocation);
		for (const float &vboElem :vboArray) {
			levelWriter << vboElem<<",";
		}
		levelWriter.close();
	}
}
void World::writeToVBOArray() {
	
}

World::World() {
	std::cout << "World created" << std::endl;
}
World::~World() {

}