#include "Light.h"
#include <iostream>
#include "glad\include\glad\glad.h"
#include <string>
#include "glm\glm\glm.hpp"
OmniLight::OmniLight() {
	std::cout << "Light created" << std::endl;
	}
OmniLight::~OmniLight() {

	}
void OmniLight::setLightPos(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	}
void OmniLight::setLightColor(float r, float g, float b) {

	this->r = r;
	this->g = g;
	this->b = b;
	}
OmniLight::OmniLight(float x, float y, float z, float r, float g, float b) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
	this->g = g;
	this->b = b;
}
void OmniLight::addLightToProgram(int programID) {
	std::string ln = lightName.c_str();
	std::cout <<z << std::endl;
	int iLightPosID = glGetUniformLocation(programID,lightName.c_str());
	int iLightParam = glGetUniformLocation(programID, lightParam.c_str());
	glUniform3f(iLightPosID, x, y, z);
	glUniform3f(iLightParam, r, g, b);

}
