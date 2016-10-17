#pragma once
#include "CShader.h"

class CShaderProgram {
public:
  void createProgram();
  void deleteProgram();

  bool addShaderToProgram(CShader *shShader);
  bool linkProgram();

  void useProgram();

  int getProgramID();

  CShaderProgram();

public:
  int uiProgram;
  bool bLinked = 0;
};