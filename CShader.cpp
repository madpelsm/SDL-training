#include "CShader.h"
#include "CShaderProgram.h"
#include "glad\include\glad\glad.h"
#include <fstream>
#include <vector>
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#define FOR(q, n) for (int q = 0; q < n; q++)
#define ESZ(elem) (int)elem.size()

CShader::CShader() {}
bool CShader::loadShader(std::string sFile, int a_iType) {
  std::ifstream t(sFile);
  std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
  uiShader = glCreateShader(a_iType);

  const char *c_str = str.c_str();
  if (uiShader == 0) {
    std::cout << "error when creating shader" << std::endl;
  }
  glShaderSource(uiShader, 1, &c_str, nullptr);
  glCompileShader(uiShader);

  GLint isCompiled = 0;
  glGetShaderiv(uiShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(uiShader, maxLength, &maxLength, &errorLog[0]);

    // Provide the infolog in whatever manor you deem best.
    // Exit with failure.
    glDeleteShader(uiShader); // Don't leak the shader.
    return false;
  }
  iType = a_iType;
  bLoaded = true;
  return 1;
};
bool CShader::isLoaded() { return bLoaded; };
int CShader::getShaderID() { return uiShader; };
void CShader::deleteShader() {
  if (!isLoaded())
    return;
  bLoaded = false;
  glDeleteShader(uiShader);
};