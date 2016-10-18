#pragma once
#include "glad\include\glad\glad.h"
#include <string>
struct OmniLight {
	std::string lightName="lightPos", lightParam="lightParams";
	float x, y, z, r, g, b;
	OmniLight();
	OmniLight(float x, float y, float z, float r, float g, float b);
	~OmniLight();
	void setLightPos(float x, float y, float z);
	void setLightColor(float r, float g, float b);
	void addLightToProgram(int programID);
};