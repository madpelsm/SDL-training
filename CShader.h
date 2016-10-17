#pragma once
#include <string>
#include <iostream>
class CShader {
public:
	bool loadShader(std::string sFile, int a_iType);
	void deleteShader();

	bool isLoaded();
	int getShaderID();

	CShader();

public:
	int uiShader; //ID of shader
	int iType; //GL_VERTEX_SHADER, GL_FRAGMENT_SHADER ...
	bool bLoaded; //if shader was loaded and compiled
};