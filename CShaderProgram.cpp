#include<string>
#include"CShaderProgram.h"
#include "glad\include\glad\glad.h"
CShaderProgram::CShaderProgram() {
}


bool CShaderProgram::linkProgram() {
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}
void CShaderProgram::deleteProgram() {
	if (!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}
void CShaderProgram::useProgram() {

	if (bLinked) {
		glUseProgram(uiProgram);
	}
}
int CShaderProgram::getProgramID() {
	return uiProgram;
}

void CShaderProgram::createProgram() {
	uiProgram = glCreateProgram();
};
bool CShaderProgram::addShaderToProgram(CShader* shShader) {
	if (!shShader->isLoaded())return false;
	glAttachShader(uiProgram, shShader->getShaderID());
	return true;
}