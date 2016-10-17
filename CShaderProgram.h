#pragma once
#include "CShader.h"

class CShaderProgram {
public:
	void createProgram();
	void deleteProgram();

	bool addShaderToProgram(CShader * shShader);
	bool linkProgram();

	void useProgram();

	unsigned int getProgramID();

	CShaderProgram();

private:
	unsigned int uiProgram;
	bool bLinked=0;
};