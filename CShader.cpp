#include "CShader.h"
#include <vector>
#include "glad\include\glad\glad.h"
#include "CShaderProgram.h"
#include <fstream>
#pragma warning (disable : 4996)
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#define FOR(q, n) for(int q=0;q<n;q++)
#define ESZ(elem) (int)elem.size()

CShader::CShader() {

}
bool CShader::loadShader(std::string sFile, int a_iType) {
	std::ifstream t(sFile);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	uiShader = glCreateShader(a_iType);

	const char *c_str = str.c_str();
	glShaderSource(uiShader, 1, &c_str, NULL);
	glCompileShader(uiShader);


	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_FALSE)return false;
	iType = a_iType;
	bLoaded = true;

	return 1;
};
bool CShader::isLoaded() {
	return bLoaded;
};
unsigned int CShader::getShaderID() {
	return uiShader;
};
void CShader::deleteShader() {
	if (!isLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
};